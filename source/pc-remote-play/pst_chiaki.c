// pst_chiaki: a small C ABI over chiaki-ng's core for the Windows app (P/Invoke), plus the
// Media Foundation H.264 decoder. Same calls and settings as the Mac's ChiakiBridge.m.
// Audio (play on the PC): Opus packets decoded to 16-bit PCM in a short ring the app pulls from.
// Controller: the app sends the pad state it reads; chiaki forwards it to the console.
// View (play on the PC): a child window that draws the newest NV12 picture with the graphics card (SDL2 on
// Direct3D 11), so the PC never converts colours per frame.
// Video: chiaki hands H.264 samples to the decoder on its own thread; the newest NV12 picture is
// kept, and the app pulls it as BGRA when it wants one (like LatestFrameBroker on the Mac).

#define COBJMACROS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <initguid.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mferror.h>
#include <mftransform.h>
#include <wmcodecdsp.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <chiaki/common.h>
#include <chiaki/session.h>
#include <chiaki/regist.h>
#include <chiaki/discovery.h>
#include <chiaki/controller.h>
#include <opus/opus.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#define PST_API __declspec(dllexport)

// CODECAPI_AVLowLatencyMode (MF_LOW_LATENCY), missing from MinGW's headers: no frame reordering delay.
DEFINE_GUID(PST_LOW_LATENCY, 0x9c27891a, 0xed7a, 0x40e1, 0x88, 0xe8, 0xb2, 0x27, 0x27, 0xa0, 0x24, 0xee);

typedef void (*pst_log_cb)(int level, const char *message, void *user);
typedef void (*pst_regist_cb)(int ok, const uint8_t *regist_key, const uint8_t *morning, const char *nickname, void *user);
/// kind: 1 connected, 2 login PIN asked, 3 quit (message = reason), 4 log line (message).
typedef void (*pst_status_cb)(int kind, int level, const char *message, void *user);

static INIT_ONCE lib_once = INIT_ONCE_STATIC_INIT;
static ChiakiErrorCode lib_error = CHIAKI_ERR_SUCCESS;

static BOOL CALLBACK lib_init(PINIT_ONCE once, void *param, void **context)
{
    (void)once; (void)param; (void)context;
    lib_error = chiaki_lib_init();
    return TRUE;
}

static ChiakiErrorCode ensure_lib(void)
{
    InitOnceExecuteOnce(&lib_once, lib_init, NULL, NULL);
    return lib_error;
}

PST_API const char *pst_rp_version(void)
{
    const char *version = chiaki_rp_version_string(CHIAKI_TARGET_PS5_1);
    return version != NULL ? version : "unknown";
}

PST_API const char *pst_error_string(int error)
{
    return chiaki_error_string((ChiakiErrorCode)error);
}

// ---------------------------------------------------------------- Registration

typedef struct
{
    ChiakiRegist regist;
    ChiakiLog log;
    char host[256];
    pst_log_cb log_cb;
    pst_regist_cb done_cb;
    void *user;
} RegistContext;

static void regist_log(ChiakiLogLevel level, const char *message, void *user)
{
    RegistContext *context = user;
    if (message != NULL && message[0] != '\0') context->log_cb((int)level, message, context->user);
}

static void regist_event(ChiakiRegistEvent *event, void *user)
{
    RegistContext *context = user;
    if (event->type == CHIAKI_REGIST_EVENT_TYPE_FINISHED_SUCCESS && event->registered_host != NULL)
    {
        ChiakiRegisteredHost *host = event->registered_host;
        context->done_cb(1, (const uint8_t *)host->rp_regist_key, host->rp_key, host->server_nickname, context->user);
    }
    else
    {
        context->done_cb(0, NULL, NULL, NULL, context->user);
    }
}

/// Starts pairing; the result comes through done (on chiaki's thread). Free the handle with
/// pst_regist_free afterwards, never from inside the callbacks. Returns NULL with *error set.
/// is_ps4: a PS4 (current firmware, 7.0+, pairs by PSN account ID like the PS5), as the iOS bridge does.
PST_API void *pst_regist_start_model(const char *host, const uint8_t *account_id, uint32_t pin, int is_ps4,
                                     pst_log_cb log_cb, pst_regist_cb done_cb, void *user, int *error)
{
    *error = ensure_lib();
    if (*error != CHIAKI_ERR_SUCCESS) return NULL;
    RegistContext *context = calloc(1, sizeof *context);
    if (context == NULL) { *error = CHIAKI_ERR_MEMORY; return NULL; }
    strncpy(context->host, host, sizeof context->host - 1);
    context->log_cb = log_cb;
    context->done_cb = done_cb;
    context->user = user;
    chiaki_log_init(&context->log, CHIAKI_LOG_ALL, regist_log, context);

    ChiakiRegistInfo info;
    memset(&info, 0, sizeof info);
    info.target = is_ps4 ? CHIAKI_TARGET_PS4_10 : CHIAKI_TARGET_PS5_1;
    info.host = context->host;
    info.broadcast = false;
    info.psn_online_id = NULL;
    memcpy(info.psn_account_id, account_id, CHIAKI_PSN_ACCOUNT_ID_SIZE);
    info.pin = pin;
    info.console_pin = 0;
    info.holepunch_info = NULL;

    *error = chiaki_regist_start(&context->regist, &context->log, &info, regist_event, context);
    if (*error != CHIAKI_ERR_SUCCESS) { free(context); return NULL; }
    return context;
}

/// The PS5-only call of earlier builds.
PST_API void *pst_regist_start(const char *host, const uint8_t *account_id, uint32_t pin,
                               pst_log_cb log_cb, pst_regist_cb done_cb, void *user, int *error)
{
    return pst_regist_start_model(host, account_id, pin, 0, log_cb, done_cb, user, error);
}

PST_API void pst_regist_free(void *handle)
{
    RegistContext *context = handle;
    if (context == NULL) return;
    chiaki_regist_stop(&context->regist);
    chiaki_regist_fini(&context->regist);
    free(context);
}

// ---------------------------------------------------------------- H.264 decoder

typedef struct
{
    IMFTransform *transform;
    BOOL streaming;
    UINT32 width, height;      // coded size
    UINT32 visible_height;     // display aperture
    UINT32 stride;
    DWORD out_size;
    LONGLONG time;
} Decoder;

static HRESULT decoder_set_output(Decoder *decoder)
{
    for (DWORD i = 0;; i++)
    {
        IMFMediaType *type = NULL;
        HRESULT hr = IMFTransform_GetOutputAvailableType(decoder->transform, 0, i, &type);
        if (FAILED(hr)) return hr;
        GUID subtype;
        if (SUCCEEDED(IMFMediaType_GetGUID(type, &MF_MT_SUBTYPE, &subtype)) && IsEqualGUID(&subtype, &MFVideoFormat_NV12))
        {
            hr = IMFTransform_SetOutputType(decoder->transform, 0, type, 0);
            if (SUCCEEDED(hr))
            {
                UINT64 size = 0;
                IMFMediaType_GetUINT64(type, &MF_MT_FRAME_SIZE, &size);
                decoder->width = (UINT32)(size >> 32);
                decoder->height = (UINT32)size;
                decoder->visible_height = decoder->height;
                MFVideoArea area;
                if (SUCCEEDED(IMFMediaType_GetBlob(type, &MF_MT_MINIMUM_DISPLAY_APERTURE, (UINT8 *)&area, sizeof area, NULL))
                    && area.Area.cy > 0 && (UINT32)area.Area.cy <= decoder->height)
                    decoder->visible_height = (UINT32)area.Area.cy;
                UINT32 stride = 0;
                decoder->stride = SUCCEEDED(IMFMediaType_GetUINT32(type, &MF_MT_DEFAULT_STRIDE, &stride)) && (INT32)stride > 0
                    ? stride : decoder->width;
                MFT_OUTPUT_STREAM_INFO info;
                if (SUCCEEDED(IMFTransform_GetOutputStreamInfo(decoder->transform, 0, &info))) decoder->out_size = info.cbSize;
            }
            IMFMediaType_Release(type);
            return hr;
        }
        IMFMediaType_Release(type);
    }
}

static HRESULT decoder_open(Decoder *decoder)
{
    HRESULT hr = CoCreateInstance(&CLSID_CMSH264DecoderMFT, NULL, CLSCTX_INPROC_SERVER, &IID_IMFTransform, (void **)&decoder->transform);
    if (FAILED(hr)) return hr;
    IMFAttributes *attributes = NULL;
    if (SUCCEEDED(IMFTransform_GetAttributes(decoder->transform, &attributes)))
    {
        IMFAttributes_SetUINT32(attributes, &PST_LOW_LATENCY, TRUE);
        IMFAttributes_Release(attributes);
    }
    IMFMediaType *input = NULL;
    hr = MFCreateMediaType(&input);
    if (FAILED(hr)) return hr;
    IMFMediaType_SetGUID(input, &MF_MT_MAJOR_TYPE, &MFMediaType_Video);
    IMFMediaType_SetGUID(input, &MF_MT_SUBTYPE, &MFVideoFormat_H264);
    IMFMediaType_SetUINT32(input, &MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive);
    hr = IMFTransform_SetInputType(decoder->transform, 0, input, 0);
    IMFMediaType_Release(input);
    if (FAILED(hr)) return hr;
    hr = decoder_set_output(decoder);
    if (FAILED(hr)) return hr;
    IMFTransform_ProcessMessage(decoder->transform, MFT_MESSAGE_NOTIFY_BEGIN_STREAMING, 0);
    IMFTransform_ProcessMessage(decoder->transform, MFT_MESSAGE_NOTIFY_START_OF_STREAM, 0);
    decoder->streaming = TRUE;
    return S_OK;
}

static void decoder_close(Decoder *decoder)
{
    if (decoder->transform != NULL)
    {
        IMFTransform_Release(decoder->transform);
        decoder->transform = NULL;
    }
    decoder->streaming = FALSE;
}

// ---------------------------------------------------------------- Session

typedef struct
{
    ChiakiSession session;
    ChiakiLog log;
    BOOL initialized;
    BOOL started;
    char host[256];
    pst_status_cb status_cb;
    void *user;

    Decoder decoder;
    BOOL com_ready;
    BOOL decoder_failed;

    // Newest decoded picture (NV12), guarded by lock.
    SRWLOCK lock;
    uint8_t *nv12;
    size_t nv12_capacity;
    UINT32 width, height, stride, plane_height;
    uint64_t sequence;

    // Sound, only when asked for (playing on the PC). Ring of interleaved PCM, guarded by audio_lock.
    OpusDecoder *opus;
    int audio_channels, audio_rate;
    CRITICAL_SECTION audio_lock;
    int16_t *ring;
    size_t ring_capacity, ring_read, ring_count;
} SessionContext;

// ---------------------------------------------------------------- Audio

static void audio_header(ChiakiAudioHeader *header, void *user)
{
    SessionContext *context = user;
    EnterCriticalSection(&context->audio_lock);
    if (context->opus != NULL) { opus_decoder_destroy(context->opus); context->opus = NULL; }
    int error = 0;
    context->audio_channels = header->channels > 0 ? header->channels : 2;
    context->audio_rate = header->rate > 0 ? (int)header->rate : 48000;
    context->opus = opus_decoder_create(context->audio_rate, context->audio_channels, &error);
    if (error != OPUS_OK) context->opus = NULL;
    // Half a second of sound at most.
    size_t capacity = (size_t)context->audio_rate * context->audio_channels / 2;
    int16_t *ring = realloc(context->ring, capacity * sizeof *ring);
    if (ring != NULL) { context->ring = ring; context->ring_capacity = capacity; }
    context->ring_read = context->ring_count = 0;
    LeaveCriticalSection(&context->audio_lock);
}

static void audio_frame(uint8_t *buf, size_t buf_size, void *user)
{
    SessionContext *context = user;
    int16_t pcm[5760 * 2];
    EnterCriticalSection(&context->audio_lock);
    if (context->opus != NULL && context->ring != NULL)
    {
        int max = (int)(sizeof pcm / sizeof pcm[0]) / context->audio_channels;
        int frames = opus_decode(context->opus, buf, (opus_int32)buf_size, pcm, max, 0);
        size_t samples = frames > 0 ? (size_t)frames * context->audio_channels : 0;
        for (size_t i = 0; i < samples; i++)
        {
            // Full: the oldest sample goes, so the sound never lags behind the picture.
            if (context->ring_count == context->ring_capacity)
            {
                context->ring_read = (context->ring_read + 1) % context->ring_capacity;
                context->ring_count--;
            }
            context->ring[(context->ring_read + context->ring_count) % context->ring_capacity] = pcm[i];
            context->ring_count++;
        }
    }
    LeaveCriticalSection(&context->audio_lock);
}

/// Takes up to max_samples interleaved 16-bit samples of the console's sound. Returns how many (0 = none yet);
/// channels and rate tell the format (both 0 until the stream said).
PST_API int pst_session_read_audio(void *handle, int16_t *out, int max_samples, int *channels, int *rate)
{
    SessionContext *context = handle;
    if (context == NULL || out == NULL || max_samples <= 0) return 0;
    EnterCriticalSection(&context->audio_lock);
    *channels = context->opus != NULL ? context->audio_channels : 0;
    *rate = context->opus != NULL ? context->audio_rate : 0;
    size_t n = context->ring_count < (size_t)max_samples ? context->ring_count : (size_t)max_samples;
    for (size_t i = 0; i < n; i++)
    {
        out[i] = context->ring[context->ring_read];
        context->ring_read = (context->ring_read + 1) % context->ring_capacity;
    }
    context->ring_count -= n;
    LeaveCriticalSection(&context->audio_lock);
    return (int)n;
}

/// The pad as the app reads it: buttons are ChiakiControllerButton bits, triggers 0-255, sticks -32768..32767.
PST_API int pst_session_set_controller(void *handle, uint32_t buttons, uint8_t l2, uint8_t r2,
                                       int16_t left_x, int16_t left_y, int16_t right_x, int16_t right_y)
{
    SessionContext *context = handle;
    if (context == NULL || !context->started) return CHIAKI_ERR_UNINITIALIZED;
    ChiakiControllerState state;
    chiaki_controller_state_set_idle(&state);
    state.buttons = buttons;
    state.l2_state = l2;
    state.r2_state = r2;
    state.left_x = left_x;
    state.left_y = left_y;
    state.right_x = right_x;
    state.right_y = right_y;
    return chiaki_session_set_controller_state(&context->session, &state);
}

static void keep_frame(SessionContext *context, IMFSample *sample)
{
    IMFMediaBuffer *buffer = NULL;
    if (FAILED(IMFSample_ConvertToContiguousBuffer(sample, &buffer))) return;
    BYTE *data = NULL;
    DWORD length = 0;
    if (SUCCEEDED(IMFMediaBuffer_Lock(buffer, &data, NULL, &length)))
    {
        Decoder *decoder = &context->decoder;
        size_t needed = (size_t)decoder->stride * decoder->height * 3 / 2;
        if (length >= needed)
        {
            AcquireSRWLockExclusive(&context->lock);
            if (context->nv12_capacity < needed)
            {
                uint8_t *grown = realloc(context->nv12, needed);
                if (grown != NULL) { context->nv12 = grown; context->nv12_capacity = needed; }
            }
            if (context->nv12_capacity >= needed)
            {
                memcpy(context->nv12, data, needed);
                context->width = decoder->width;
                context->height = decoder->visible_height;
                context->stride = decoder->stride;
                context->plane_height = decoder->height;
                context->sequence++;
            }
            ReleaseSRWLockExclusive(&context->lock);
        }
        IMFMediaBuffer_Unlock(buffer);
    }
    IMFMediaBuffer_Release(buffer);
}

static void drain_output(SessionContext *context)
{
    Decoder *decoder = &context->decoder;
    for (;;)
    {
        IMFSample *sample = NULL;
        IMFMediaBuffer *buffer = NULL;
        if (FAILED(MFCreateSample(&sample))) return;
        if (FAILED(MFCreateMemoryBuffer(decoder->out_size ? decoder->out_size : decoder->stride * decoder->height * 3 / 2, &buffer)))
        {
            IMFSample_Release(sample);
            return;
        }
        IMFSample_AddBuffer(sample, buffer);
        IMFMediaBuffer_Release(buffer);
        MFT_OUTPUT_DATA_BUFFER output;
        memset(&output, 0, sizeof output);
        output.pSample = sample;
        DWORD status = 0;
        HRESULT hr = IMFTransform_ProcessOutput(decoder->transform, 0, 1, &output, &status);
        if (output.pEvents != NULL) IMFCollection_Release(output.pEvents);
        if (hr == MF_E_TRANSFORM_STREAM_CHANGE)
        {
            IMFSample_Release(sample);
            if (FAILED(decoder_set_output(decoder))) return;
            continue;
        }
        if (SUCCEEDED(hr)) keep_frame(context, sample);
        IMFSample_Release(sample);
        if (FAILED(hr)) return; // MF_E_TRANSFORM_NEED_MORE_INPUT or an error
    }
}

static bool video_sample(uint8_t *buf, size_t buf_size, int32_t frames_lost, bool frame_recovered, void *user)
{
    (void)frames_lost; (void)frame_recovered;
    SessionContext *context = user;
    if (buf == NULL || buf_size == 0 || context->decoder_failed) return false;
    if (!context->com_ready)
    {
        // The decoder lives on chiaki's video thread.
        CoInitializeEx(NULL, COINIT_MULTITHREADED);
        context->com_ready = TRUE;
        if (FAILED(decoder_open(&context->decoder)))
        {
            context->decoder_failed = TRUE;
            decoder_close(&context->decoder);
            context->status_cb(4, CHIAKI_LOG_ERROR, "H.264 decoder unavailable", context->user);
            return false;
        }
    }
    IMFSample *sample = NULL;
    IMFMediaBuffer *buffer = NULL;
    if (FAILED(MFCreateSample(&sample))) return false;
    if (FAILED(MFCreateMemoryBuffer((DWORD)buf_size, &buffer))) { IMFSample_Release(sample); return false; }
    BYTE *data = NULL;
    IMFMediaBuffer_Lock(buffer, &data, NULL, NULL);
    memcpy(data, buf, buf_size);
    IMFMediaBuffer_Unlock(buffer);
    IMFMediaBuffer_SetCurrentLength(buffer, (DWORD)buf_size);
    IMFSample_AddBuffer(sample, buffer);
    IMFMediaBuffer_Release(buffer);
    IMFSample_SetSampleTime(sample, context->decoder.time);
    IMFSample_SetSampleDuration(sample, 333333);
    context->decoder.time += 333333;

    HRESULT hr = IMFTransform_ProcessInput(context->decoder.transform, 0, sample, 0);
    if (hr == MF_E_NOTACCEPTING)
    {
        drain_output(context);
        hr = IMFTransform_ProcessInput(context->decoder.transform, 0, sample, 0);
    }
    IMFSample_Release(sample);
    if (SUCCEEDED(hr)) drain_output(context);
    return SUCCEEDED(hr);
}

static void session_event(ChiakiEvent *event, void *user)
{
    SessionContext *context = user;
    switch (event->type)
    {
        case CHIAKI_EVENT_CONNECTED:
            context->status_cb(1, 0, "", context->user);
            break;
        case CHIAKI_EVENT_LOGIN_PIN_REQUEST:
            context->status_cb(2, 0, "", context->user);
            break;
        case CHIAKI_EVENT_QUIT:
            context->status_cb(3, 0, chiaki_quit_reason_string(event->quit.reason), context->user);
            break;
        default:
            break;
    }
}

static void session_log(ChiakiLogLevel level, const char *message, void *user)
{
    // Same as the Mac: info, warnings and errors only.
    if (level > CHIAKI_LOG_INFO || message == NULL) return;
    SessionContext *context = user;
    context->status_cb(4, (int)level, message, context->user);
}

/// Opens a local session to the console. resolution: 1=360p 2=540p 3=720p 4=1080p; fps 30 or 60.
/// is_ps4: the PS4 protocol (it streams H.264 only, which is what is asked for anyway). Returns NULL with *error set.
/// with_audio: playing on the PC, so the console's sound comes too (pst_session_read_audio).
PST_API void *pst_session_start_play(const char *host, const uint8_t *regist_key, const uint8_t *morning, const uint8_t *account_id,
                                     int resolution_preset, int frame_rate, int is_ps4, int with_audio,
                                     pst_status_cb status_cb, void *user, int *error)
{
    *error = ensure_lib();
    if (*error != CHIAKI_ERR_SUCCESS) return NULL;
    HRESULT hr = MFStartup(MF_VERSION, MFSTARTUP_LITE);
    if (FAILED(hr)) { *error = CHIAKI_ERR_UNKNOWN; return NULL; }
    SessionContext *context = calloc(1, sizeof *context);
    if (context == NULL) { MFShutdown(); *error = CHIAKI_ERR_MEMORY; return NULL; }
    InitializeSRWLock(&context->lock);
    InitializeCriticalSection(&context->audio_lock);
    strncpy(context->host, host, sizeof context->host - 1);
    context->status_cb = status_cb;
    context->user = user;
    chiaki_log_init(&context->log, CHIAKI_LOG_ALL, session_log, context);

    ChiakiConnectInfo info;
    memset(&info, 0, sizeof info);
    info.ps5 = !is_ps4;
    info.host = context->host;
    memcpy(info.regist_key, regist_key, 16);
    memcpy(info.morning, morning, 16);
    memcpy(info.psn_account_id, account_id, 8);
    ChiakiVideoResolutionPreset resolution = CHIAKI_VIDEO_RESOLUTION_PRESET_720p;
    switch (resolution_preset)
    {
        case 1: resolution = CHIAKI_VIDEO_RESOLUTION_PRESET_360p; break;
        case 2: resolution = CHIAKI_VIDEO_RESOLUTION_PRESET_540p; break;
        case 4: resolution = CHIAKI_VIDEO_RESOLUTION_PRESET_1080p; break;
        default: break;
    }
    chiaki_connect_video_profile_preset(&info.video_profile, resolution,
                                        frame_rate == 60 ? CHIAKI_VIDEO_FPS_PRESET_60 : CHIAKI_VIDEO_FPS_PRESET_30);
    // Playing at 60 pictures a second: twice the data (default 15000) keeps the picture as sharp as at 30.
    if (with_audio && frame_rate == 60) info.video_profile.bitrate = 30000;
    info.video_profile.codec = CHIAKI_CODEC_H264;
    info.video_profile_auto_downgrade = true;
    info.audio_video_disabled = with_audio ? 0 : CHIAKI_AUDIO_DISABLED;
    info.packet_loss_max = 0.10;
    info.enable_idr_on_fec_failure = true;

    *error = chiaki_session_init(&context->session, &info, &context->log);
    if (*error != CHIAKI_ERR_SUCCESS) { free(context); MFShutdown(); return NULL; }
    context->initialized = TRUE;
    chiaki_session_set_event_cb(&context->session, session_event, context);
    chiaki_session_set_video_sample_cb(&context->session, video_sample, context);
    if (with_audio)
    {
        ChiakiAudioSink sink = { .user = context, .header_cb = audio_header, .frame_cb = audio_frame };
        chiaki_session_set_audio_sink(&context->session, &sink);
    }

    *error = chiaki_session_start(&context->session);
    if (*error != CHIAKI_ERR_SUCCESS)
    {
        chiaki_session_fini(&context->session);
        free(context);
        MFShutdown();
        return NULL;
    }
    context->started = TRUE;
    return context;
}

/// Without sound (reading subtitles only), as before.
PST_API void *pst_session_start_model(const char *host, const uint8_t *regist_key, const uint8_t *morning, const uint8_t *account_id,
                                      int resolution_preset, int frame_rate, int is_ps4, pst_status_cb status_cb, void *user, int *error)
{
    return pst_session_start_play(host, regist_key, morning, account_id, resolution_preset, frame_rate, is_ps4, 0, status_cb, user, error);
}

/// The PS5-only call of earlier builds.
PST_API void *pst_session_start(const char *host, const uint8_t *regist_key, const uint8_t *morning, const uint8_t *account_id,
                                int resolution_preset, int frame_rate, pst_status_cb status_cb, void *user, int *error)
{
    return pst_session_start_model(host, regist_key, morning, account_id, resolution_preset, frame_rate, 0, status_cb, user, error);
}

PST_API void pst_session_stop(void *handle)
{
    SessionContext *context = handle;
    if (context == NULL) return;
    if (context->started)
    {
        chiaki_session_stop(&context->session);
        chiaki_session_join(&context->session);
    }
    if (context->initialized) chiaki_session_fini(&context->session);
    decoder_close(&context->decoder);
    if (context->opus != NULL) opus_decoder_destroy(context->opus);
    DeleteCriticalSection(&context->audio_lock);
    free(context->ring);
    free(context->nv12);
    free(context);
    MFShutdown();
}

PST_API void pst_session_goto_bed(void *handle)
{
    SessionContext *context = handle;
    if (context != NULL && context->started) chiaki_session_goto_bed(&context->session);
}

static inline uint8_t clamp8(int value) { return value < 0 ? 0 : value > 255 ? 255 : (uint8_t)value; }

/// Copies the newest picture newer than `after` as BGRA (top-down, width*4 per row).
/// Returns 1 when copied, 0 when nothing newer, -1 when bgra is too small (width/height still set).
PST_API int pst_session_copy_frame(void *handle, uint64_t after, uint8_t *bgra, int capacity,
                                   int *width, int *height, uint64_t *sequence)
{
    SessionContext *context = handle;
    if (context == NULL) return 0;
    AcquireSRWLockShared(&context->lock);
    int result = 0;
    if (context->sequence > after && context->nv12 != NULL)
    {
        int w = (int)context->width, h = (int)context->height;
        *width = w;
        *height = h;
        *sequence = context->sequence;
        if (bgra == NULL || capacity < w * h * 4) result = -1;
        else
        {
            // BT.709 limited range.
            const uint8_t *luma = context->nv12;
            const uint8_t *chroma = context->nv12 + (size_t)context->stride * context->plane_height;
            for (int y = 0; y < h; y++)
            {
                const uint8_t *row = luma + (size_t)y * context->stride;
                const uint8_t *uv = chroma + (size_t)(y / 2) * context->stride;
                uint8_t *out = bgra + (size_t)y * w * 4;
                for (int x = 0; x < w; x++)
                {
                    int c = (row[x] - 16) * 298;
                    int d = uv[x & ~1] - 128;
                    int e = uv[(x & ~1) + 1] - 128;
                    out[0] = clamp8((c + 541 * d + 128) >> 8);
                    out[1] = clamp8((c - 55 * d - 136 * e + 128) >> 8);
                    out[2] = clamp8((c + 459 * e + 128) >> 8);
                    out[3] = 255;
                    out += 4;
                }
            }
            result = 1;
        }
    }
    ReleaseSRWLockShared(&context->lock);
    return result;
}

// ---------------------------------------------------------------- View (graphics card)

typedef struct
{
    SessionContext *session;   // NULL: a moving test pattern (pst_view_open_demo)
    HWND parent;
    HANDLE thread;
    volatile LONG stop;
    volatile LONG ready;       // 1 drawing, -1 failed
} View;

/// The test pattern: a gradient that moves, in NV12 like the decoder's pictures.
static void demo_picture(uint8_t *nv12, int w, int h, int t)
{
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++)
            nv12[(size_t)y * w + x] = (uint8_t)(16 + ((x + t * 4) % w) * 219 / w);
    uint8_t *uv = nv12 + (size_t)w * h;
    for (int y = 0; y < h / 2; y++)
        for (int x = 0; x < w; x += 2)
        {
            uv[(size_t)y * w + x] = (uint8_t)(64 + (y * 128) / (h / 2));
            uv[(size_t)y * w + x + 1] = (uint8_t)(192 - (x * 128) / w);
        }
}

static DWORD WINAPI view_thread(void *param)
{
    View *view = param;
    SDL_SetMainReady();
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "direct3d11");
    SDL_SetHint(SDL_HINT_VIDEO_ALLOW_SCREENSAVER, "1");
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) { InterlockedExchange(&view->ready, -1); return 1; }
    SDL_Window *window = SDL_CreateWindow("pst-view", 0, 0, 16, 16, SDL_WINDOW_BORDERLESS | SDL_WINDOW_HIDDEN);
    SDL_Renderer *renderer = window ? SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) : NULL;
    if (renderer == NULL && window != NULL) renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        if (window) SDL_DestroyWindow(window);
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
        InterlockedExchange(&view->ready, -1);
        return 1;
    }
    SDL_SetYUVConversionMode(SDL_YUV_CONVERSION_BT709);
    SDL_SysWMinfo info;
    SDL_VERSION(&info.version);
    SDL_GetWindowWMInfo(window, &info);
    HWND child = info.info.win.window;
    // Inside the app's window; never takes the keyboard (F11 and the pad keep working there).
    SetWindowLongPtrW(child, GWL_STYLE, (GetWindowLongPtrW(child, GWL_STYLE) & ~(LONG_PTR)WS_POPUP) | WS_CHILD | WS_VISIBLE);
    SetWindowLongPtrW(child, GWL_EXSTYLE, GetWindowLongPtrW(child, GWL_EXSTYLE) | WS_EX_NOACTIVATE);
    SetParent(child, view->parent);
    EnableWindow(child, FALSE);
    InterlockedExchange(&view->ready, 1);

    SDL_Texture *texture = NULL;
    int tex_w = 0, tex_h = 0, last_w = -1, last_h = -1, tick = 0;
    uint64_t shown = 0;
    uint8_t *demo = NULL;
    while (!view->stop && IsWindow(view->parent))
    {
        SDL_PumpEvents();
        RECT r;
        GetClientRect(view->parent, &r);
        if (r.right != last_w || r.bottom != last_h)
        {
            last_w = r.right; last_h = r.bottom;
            SDL_SetWindowSize(window, last_w > 0 ? last_w : 1, last_h > 0 ? last_h : 1);
            SetWindowPos(child, HWND_TOP, 0, 0, last_w, last_h, SWP_NOACTIVATE | SWP_SHOWWINDOW | SWP_FRAMECHANGED);
            SDL_ShowWindow(window);
            // The graphics card's drawing buffer keeps the size it was made with: a new size gets a new renderer.
            if (texture) { SDL_DestroyTexture(texture); texture = NULL; }
            SDL_DestroyRenderer(renderer);
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (renderer == NULL) renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (renderer == NULL) break;
            shown = 0;
        }
        BOOL fresh = FALSE;
        if (view->session != NULL)
        {
            SessionContext *context = view->session;
            AcquireSRWLockShared(&context->lock);
            if (context->sequence != shown && context->nv12 != NULL)
            {
                int w = (int)context->width, h = (int)context->height & ~1;
                if (texture == NULL || w != tex_w || h != tex_h)
                {
                    if (texture) SDL_DestroyTexture(texture);
                    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_NV12, SDL_TEXTUREACCESS_STREAMING, w, h);
                    tex_w = w; tex_h = h;
                }
                if (texture != NULL)
                {
                    const uint8_t *luma = context->nv12;
                    const uint8_t *chroma = context->nv12 + (size_t)context->stride * context->plane_height;
                    SDL_UpdateNVTexture(texture, NULL, luma, (int)context->stride, chroma, (int)context->stride);
                    fresh = TRUE;
                }
                shown = context->sequence;
            }
            ReleaseSRWLockShared(&context->lock);
        }
        else
        {
            const int w = 1280, h = 720;
            if (demo == NULL) demo = malloc((size_t)w * h * 3 / 2);
            if (texture == NULL) { texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_NV12, SDL_TEXTUREACCESS_STREAMING, w, h); tex_w = w; tex_h = h; }
            if (demo != NULL && texture != NULL)
            {
                demo_picture(demo, w, h, tick++);
                SDL_UpdateNVTexture(texture, NULL, demo, w, demo + (size_t)w * h, w);
                fresh = TRUE;
            }
        }
        if (!fresh) { Sleep(2); continue; }
        int out_w = 0, out_h = 0;
        SDL_GetRendererOutputSize(renderer, &out_w, &out_h);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        if (texture != NULL && tex_w > 0 && tex_h > 0 && out_w > 0 && out_h > 0)
        {
            // Whole picture, black bars where the shapes differ.
            double scale = (double)out_w / tex_w < (double)out_h / tex_h ? (double)out_w / tex_w : (double)out_h / tex_h;
            SDL_Rect dst = { 0, 0, (int)(tex_w * scale), (int)(tex_h * scale) };
            dst.x = (out_w - dst.w) / 2;
            dst.y = (out_h - dst.h) / 2;
            SDL_RenderCopy(renderer, texture, NULL, &dst);
        }
        SDL_RenderPresent(renderer);
    }
    free(demo);
    if (texture) SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    return 0;
}

static void *view_open(SessionContext *session, void *parent)
{
    View *view = calloc(1, sizeof *view);
    if (view == NULL) return NULL;
    view->session = session;
    view->parent = (HWND)parent;
    view->thread = CreateThread(NULL, 0, view_thread, view, 0, NULL);
    if (view->thread == NULL) { free(view); return NULL; }
    // Wait for the graphics card to be ready (or not), so the app knows whether to draw itself instead.
    for (int i = 0; i < 300 && view->ready == 0; i++) Sleep(10);
    if (view->ready != 1)
    {
        InterlockedExchange(&view->stop, 1);
        WaitForSingleObject(view->thread, 3000);
        CloseHandle(view->thread);
        free(view);
        return NULL;
    }
    return view;
}

/// Draws the session's pictures into a child of `parent` (an HWND) with the graphics card. NULL when that is not
/// possible (the app then draws the pictures itself). Close it before the session stops.
PST_API void *pst_view_open(void *handle, void *parent)
{
    SessionContext *context = handle;
    if (context == NULL || parent == NULL) return NULL;
    return view_open(context, parent);
}

/// A moving test pattern instead of a session's pictures (tests and screenshots without a console).
PST_API void *pst_view_open_demo(void *parent)
{
    return parent == NULL ? NULL : view_open(NULL, parent);
}

PST_API void pst_view_close(void *handle)
{
    View *view = handle;
    if (view == NULL) return;
    InterlockedExchange(&view->stop, 1);
    WaitForSingleObject(view->thread, 3000);
    CloseHandle(view->thread);
    free(view);
}

// ---------------------------------------------------------------- Wake-up

static void quiet_log(ChiakiLogLevel level, const char *message, void *user)
{
    (void)level; (void)message; (void)user;
}

/// Wakes the console from rest mode (the PS4 listens on its own port and protocol).
PST_API int pst_wakeup_model(const char *host, uint64_t credential, int is_ps4)
{
    ChiakiErrorCode error = ensure_lib();
    if (error != CHIAKI_ERR_SUCCESS) return error;
    ChiakiLog log;
    chiaki_log_init(&log, CHIAKI_LOG_ALL, quiet_log, NULL);
    return chiaki_discovery_wakeup(&log, NULL, host, credential, !is_ps4);
}

PST_API int pst_wakeup(const char *host, uint64_t credential)
{
    return pst_wakeup_model(host, credential, 0);
}

// ---------------------------------------------------------------- Self-test

static void selftest_status(int kind, int level, const char *message, void *user)
{
    (void)kind; (void)level; (void)message; (void)user;
}

/// Checks the decode path without a console: encodes `frames` pictures (dark left, bright right,
/// a moving bar) with Windows' H.264 encoder, feeds them to video_sample exactly as chiaki does,
/// and copies the newest decoded picture as BGRA. Returns the number of decoded pictures, or a
/// negative step number on failure.
PST_API int pst_decoder_selftest(int width, int height, int frames, uint8_t *bgra, int capacity)
{
    if (FAILED(MFStartup(MF_VERSION, MFSTARTUP_LITE))) return -1;
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
    int result = -2;
    IMFTransform *encoder = NULL;
    SessionContext *context = calloc(1, sizeof *context);
    if (context == NULL) goto done;
    InitializeSRWLock(&context->lock);
    context->status_cb = selftest_status;
    if (FAILED(CoCreateInstance(&CLSID_CMSH264EncoderMFT, NULL, CLSCTX_INPROC_SERVER, &IID_IMFTransform, (void **)&encoder))) goto done;

    result = -3;
    IMFMediaType *type = NULL;
    MFCreateMediaType(&type);
    IMFMediaType_SetGUID(type, &MF_MT_MAJOR_TYPE, &MFMediaType_Video);
    IMFMediaType_SetGUID(type, &MF_MT_SUBTYPE, &MFVideoFormat_H264);
    IMFMediaType_SetUINT32(type, &MF_MT_AVG_BITRATE, 4000000);
    IMFMediaType_SetUINT64(type, &MF_MT_FRAME_SIZE, ((UINT64)width << 32) | (UINT32)height);
    IMFMediaType_SetUINT64(type, &MF_MT_FRAME_RATE, ((UINT64)30 << 32) | 1);
    IMFMediaType_SetUINT32(type, &MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive);
    HRESULT hr = IMFTransform_SetOutputType(encoder, 0, type, 0);
    IMFMediaType_Release(type);
    if (FAILED(hr)) goto done;
    MFCreateMediaType(&type);
    IMFMediaType_SetGUID(type, &MF_MT_MAJOR_TYPE, &MFMediaType_Video);
    IMFMediaType_SetGUID(type, &MF_MT_SUBTYPE, &MFVideoFormat_NV12);
    IMFMediaType_SetUINT64(type, &MF_MT_FRAME_SIZE, ((UINT64)width << 32) | (UINT32)height);
    IMFMediaType_SetUINT64(type, &MF_MT_FRAME_RATE, ((UINT64)30 << 32) | 1);
    IMFMediaType_SetUINT32(type, &MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive);
    hr = IMFTransform_SetInputType(encoder, 0, type, 0);
    IMFMediaType_Release(type);
    if (FAILED(hr)) goto done;
    IMFTransform_ProcessMessage(encoder, MFT_MESSAGE_NOTIFY_BEGIN_STREAMING, 0);
    IMFTransform_ProcessMessage(encoder, MFT_MESSAGE_NOTIFY_START_OF_STREAM, 0);

    result = -4;
    MFT_OUTPUT_STREAM_INFO info;
    if (FAILED(IMFTransform_GetOutputStreamInfo(encoder, 0, &info))) goto done;
    DWORD out_size = info.cbSize ? info.cbSize : (DWORD)(width * height * 2);
    DWORD picture = (DWORD)(width * height * 3 / 2);
    for (int i = 0; i <= frames; i++)
    {
        if (i < frames)
        {
            IMFSample *input = NULL;
            IMFMediaBuffer *buffer = NULL;
            MFCreateSample(&input);
            MFCreateMemoryBuffer(picture, &buffer);
            BYTE *data = NULL;
            IMFMediaBuffer_Lock(buffer, &data, NULL, NULL);
            for (int y = 0; y < height; y++)
                for (int x = 0; x < width; x++)
                    data[y * width + x] = (BYTE)(x < width / 2 ? 30 : 220) + ((x / 16 == i % (width / 16)) ? 20 : 0);
            memset(data + width * height, 128, (size_t)width * height / 2);
            IMFMediaBuffer_Unlock(buffer);
            IMFMediaBuffer_SetCurrentLength(buffer, picture);
            IMFSample_AddBuffer(input, buffer);
            IMFMediaBuffer_Release(buffer);
            IMFSample_SetSampleTime(input, (LONGLONG)i * 333333);
            IMFSample_SetSampleDuration(input, 333333);
            hr = IMFTransform_ProcessInput(encoder, 0, input, 0);
            IMFSample_Release(input);
            if (FAILED(hr)) goto done;
        }
        else
        {
            IMFTransform_ProcessMessage(encoder, MFT_MESSAGE_COMMAND_DRAIN, 0);
        }
        for (;;)
        {
            IMFSample *output = NULL;
            IMFMediaBuffer *buffer = NULL;
            MFCreateSample(&output);
            MFCreateMemoryBuffer(out_size, &buffer);
            IMFSample_AddBuffer(output, buffer);
            IMFMediaBuffer_Release(buffer);
            MFT_OUTPUT_DATA_BUFFER out;
            memset(&out, 0, sizeof out);
            out.pSample = output;
            DWORD status = 0;
            hr = IMFTransform_ProcessOutput(encoder, 0, 1, &out, &status);
            if (out.pEvents != NULL) IMFCollection_Release(out.pEvents);
            if (SUCCEEDED(hr))
            {
                IMFMediaBuffer *encoded = NULL;
                IMFSample_ConvertToContiguousBuffer(output, &encoded);
                BYTE *bytes = NULL;
                DWORD length = 0;
                IMFMediaBuffer_Lock(encoded, &bytes, NULL, &length);
                video_sample(bytes, length, 0, false, context);
                IMFMediaBuffer_Unlock(encoded);
                IMFMediaBuffer_Release(encoded);
            }
            IMFSample_Release(output);
            if (FAILED(hr)) break;
        }
    }
    result = (int)context->sequence;
    int w = 0, h = 0;
    uint64_t sequence = 0;
    if (result > 0 && pst_session_copy_frame(context, 0, bgra, capacity, &w, &h, &sequence) != 1) result = -5;
    if (result > 0 && (w != width || h != height)) result = -6;

done:
    if (encoder != NULL) IMFTransform_Release(encoder);
    if (context != NULL)
    {
        decoder_close(&context->decoder);
        free(context->nv12);
        free(context);
    }
    MFShutdown();
    return result;
}
