# PS Translator

# Cập nhật thông tin sớm nhất tại: https://t.me/pstranslator

Dịch phụ đề game và video sang tiếng Việt ngay trên iPhone, có đọc thành giọng nói.

<p align="center">
  <img src="screenshots/01-home.png" width="200">
  <img src="screenshots/02-settings.png" width="200">
  <img src="screenshots/04-pairing.png" width="200">
</p>

## Tính năng

- **Chơi từ xa PS5**: xem và dịch phụ đề game trực tiếp qua mạng Wi-Fi.
- **Xem YouTube & web**: dịch phụ đề cứng trong video bằng nhận dạng chữ (OCR), hoặc dùng phụ đề CC.
- **Nhiều bộ dịch**: Dịch thuật Apple (ngoại tuyến, miễn phí), Apple Intelligence, CloudAPI, Google Gemini.
- **Đọc phụ đề**: giọng Piper tiếng Việt có sẵn trong app, hoặc giọng Apple.
- **Tùy chỉnh phụ đề**: cỡ chữ, màu chữ, màu nền, vị trí (nút "Aa").
- **Hồ sơ game**: lưu vùng quét và phong cách dịch riêng cho từng game.

## Tải về

### Có máy tính

Vào mục [**Releases**](../../releases/latest) và tải file theo phiên bản iOS của máy:

| File | Dành cho | Ghi chú |
|---|---|---|
| `PS-Translator.ipa` | iOS 26 trở lên | Đầy đủ tính năng |
| `PS-Translator-iOS18.ipa` | iOS 18 đến iOS 25 | **Không hỗ trợ Apple Intelligence**, còn lại giống bản chính |

Không tải file **Source code**, đó là file GitHub tự tạo.

### Không có máy tính

Nếu không có PC hoặc Mac để cài file IPA, hãy tham gia Telegram:

**https://t.me/pstranslator**

Link **TestFlight** sẽ được cập nhật tại đây khi có bản thử nghiệm và còn suất tham gia.

## Cách cài file IPA bằng máy tính

App được cài ngoài App Store bằng một trong hai công cụ miễn phí:

1. Cài [Sideloadly](https://sideloadly.io) (Windows hoặc macOS) hoặc [AltStore](https://altstore.io).
2. Cắm iPhone vào máy tính, kéo file `PS-Translator.ipa` vào công cụ và đăng nhập Apple ID của bạn.
3. Trên iPhone: **Cài đặt → Cài đặt chung → VPN & Quản lý thiết bị**, chọn Apple ID của bạn rồi bấm **Tin cậy**.
4. Bật **Chế độ nhà phát triển** nếu iPhone yêu cầu: **Cài đặt → Quyền riêng tư & Bảo mật → Chế độ nhà phát triển**.

Với Apple ID miễn phí, app hết hạn sau **7 ngày** và cần cài lại. AltStore có thể tự làm mới ứng dụng.

Nếu không có máy tính hoặc không muốn sideload thủ công, hãy vào Telegram để nhận link TestFlight:

**https://t.me/pstranslator**

## Lưu ý

- **Dịch thuật Apple**: cần app [Dịch thuật](https://apps.apple.com/app/id1514844618) của Apple và tải gói Tiếng Anh + Tiếng Việt trong app đó.
- **Apple Intelligence**: chỉ có ở bản iOS 26 và trên các thiết bị được Apple hỗ trợ.
- **Ghép nối PS5**: làm theo nút **?** trong màn Ghép nối. Account ID được lấy bằng cách đăng nhập trong Safari, app không nhận mật khẩu của bạn.
- Đây là bản **thử nghiệm**. Nếu gặp lỗi, hãy tạo một mục [Issues](../../issues) kèm mô tả và ảnh chụp màn hình.
- Thông báo bản mới và link TestFlight sẽ được cập nhật tại **https://t.me/pstranslator**.

## Các lỗi thường gặp và cách khắc phục

### 1. Báo lỗi "PS5 từ chối đăng ký" khi ghép nối
- **Nhập đúng IP**: Trên PS5 vào **Cài đặt (Settings) → Mạng (Network) → Trạng thái kết nối (Connection Status) → Xem trạng thái kết nối (View Connection Status)** để lấy **Địa chỉ IPv4**. Sau đó nhập chính xác địa chỉ này vào mục IP trong app.
- **Trùng khớp tài khoản**: Tài khoản bạn đăng nhập trên Safari để lấy **Account ID** phải là tài khoản User đang được mở và điều khiển trên PS5 lúc ghép nối.
- **Bật Remote Play**: Trên PS5 vào **Cài đặt (Settings) → Hệ thống (System) → Chơi từ xa (Remote Play) → Cho phép chơi từ xa (Enable Remote Play)**.
- **Nhập mã PIN mới**: Trên PS5 chọn **Liên kết thiết bị (Link Device)**, giữ nguyên màn hình hiển thị mã và nhập ngay **8 số** đó vào app rồi bấm **Ghép nối**.
- **Chung mạng**: Ở lần ghép nối đầu tiên, iPhone và PS5 nên kết nối cùng một mạng Wi-Fi/LAN nội bộ.

### 2. Kết nối Remote Play xong nhưng tay cầm bị ngắt hoặc không điều khiển được
- **Nhấn giữ nút PS trên tay cầm**.
- Chọn lại **User bạn muốn chơi**.
- Sau khi chuyển đúng User, tiếp tục chơi game bình thường bằng tay cầm.

**Khuyến nghị:** nên dùng **một tài khoản phụ để kết nối Remote Play**, sau đó trên tay cầm chuyển sang **tài khoản chính để chơi game**.

## Ủng hộ

PS Translator được phát hành miễn phí.

Nếu thấy ứng dụng hữu ích và muốn ủng hộ tác giả:

**MB Bank**  
**Số tài khoản: 008808**

<p align="center">
  <img src="https://img.vietqr.io/image/MB-008808-qr_only.png" width="280">
</p>

Bạn cũng có thể mở mục **Ủng hộ tác giả** ngay trong ứng dụng.

---

Tên sản phẩm và nhãn hiệu thuộc về chủ sở hữu tương ứng. Dự án không liên kết với Sony Interactive Entertainment.
