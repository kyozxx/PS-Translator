# pst_chiaki.dll: chiaki-ng's core (ThirdParty/chiaki-ng, not modified) plus a small C ABI and the
# Media Foundation H.264 decoder, for the Windows app. Build with build_chiaki.sh (MSYS2 MINGW64).
cmake_minimum_required(VERSION 3.20)
project(pst_chiaki C)

foreach(option CHIAKI_ENABLE_GUI CHIAKI_ENABLE_CLI CHIAKI_ENABLE_TESTS CHIAKI_ENABLE_SETSU
        CHIAKI_ENABLE_STEAMDECK_NATIVE CHIAKI_ENABLE_STEAM_SHORTCUT CHIAKI_ENABLE_FFMPEG_DECODER
        CHIAKI_ENABLE_PI_DECODER CHIAKI_LIB_ENABLE_OPUS CHIAKI_ENABLE_SPEEX CHIAKI_ENABLE_RUDP
        CHIAKI_USE_SYSTEM_NANOPB CHIAKI_USE_SYSTEM_JERASURE)
    set(${option} OFF CACHE STRING "" FORCE)
endforeach()

add_subdirectory(../../ThirdParty/chiaki-ng chiaki EXCLUDE_FROM_ALL)

add_library(pst_chiaki SHARED pst_chiaki.c)
target_link_libraries(pst_chiaki PRIVATE chiaki-lib opus SDL2 mfplat mfuuid wmcodecdspuuid ole32)
set_target_properties(pst_chiaki PROPERTIES PREFIX "")
target_link_options(pst_chiaki PRIVATE -static-libgcc -Wl,--no-insert-timestamp)
