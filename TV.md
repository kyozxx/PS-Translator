# Phụ đề trên TV – Hướng dẫn cài app TV

Phụ đề tiếng Việt hiện nổi ngay trên hình game ở TV. Máy chơi game vẫn cắm HDMI thẳng vào TV nên hình gốc, không trễ. Chỉ có chữ đi từ iPhone sang TV qua Wi-Fi.

## Cần có

- TV chạy hệ điều hành **Android TV** (có cửa hàng Google Play trên TV).
- **Không hỗ trợ** TV hệ điều hành khác và Android Box.
- TV và iPhone dùng chung một mạng Wi-Fi.
- App Console Translator trên iPhone có gói PRO.

## Bước 1: Tải app TV

Tải file **PS-Translator-TV.apk** (khoảng 170 KB):
https://github.com/kyozxx/PS-Translator/releases/download/tv-0.1.0/PS-Translator-TV.apk

Cách cài dễ nhất bằng remote TV:

1. Trên TV, mở Google Play, tìm và cài app **Downloader**.
2. Vào **Cài đặt TV → Ứng dụng → Quyền truy cập ứng dụng đặc biệt → Cài đặt ứng dụng không rõ nguồn**, bật cho **Downloader**.
3. Mở Downloader, gõ đường link tải ở trên rồi bấm **Go**.
4. Tải xong, bấm **Cài đặt**.

Mẹo: đường link dài khó gõ bằng remote. Bạn có thể tạo mã ngắn trên trang aftv.news rồi gõ mã số đó vào Downloader.

## Bước 2: Cấp quyền hiển thị phụ đề

1. Mở app **PS Translator TV**.
2. Bấm **Cấp quyền hiển thị trên ứng dụng khác**, bật quyền cho app.
3. Nếu app hỏi **bỏ qua tối ưu hóa pin**, chọn **Cho phép** để TV không tắt app khi chuyển sang cổng HDMI.

Nếu TV không có mục cấp quyền, bật bằng máy tính (cần bật Gỡ lỗi USB / Gỡ lỗi qua mạng trong Tùy chọn nhà phát triển của TV):

```
adb connect <IP của TV>:5555
adb shell appops set com.kyozx.consoletranslator.tv SYSTEM_ALERT_WINDOW allow
```

## Bước 3: Ghép nối với iPhone

1. Trên TV, mở app PS Translator TV. Màn hình hiện **mã 8 số** (đổi mỗi phút).
2. Trên iPhone: **Dịch khi chơi TV → nút TV** trên thanh công cụ.
3. Chọn TV trong danh sách (hoặc nhập địa chỉ IP đang hiện trên TV), nhập mã 8 số, bấm **Kết nối**.
4. Chuyển TV sang cổng HDMI của máy chơi game. Phụ đề hiện trên hình game.

Chỉ cần nhập mã một lần, lần sau iPhone tự kết nối lại.

Chỉnh cỡ chữ, màu chữ, nền, vị trí và độ rộng dòng ngay trên iPhone. Có nút **Gửi câu thử** để chỉnh mà không cần chơi.

## Lỗi thường gặp

| Lỗi | Cách xử lý |
|---|---|
| "Đã xảy ra sự cố khi phân tích cú pháp gói" | File tải chưa xong hoặc bị hỏng. Tải lại bằng Downloader, đừng gửi file qua tin nhắn. |
| iPhone không tìm thấy TV | Mở app TV trên TV, kiểm tra cùng Wi-Fi, hoặc nhập địa chỉ IP đang hiện trên TV. Kiểm tra iPhone đã cho phép **Mạng cục bộ** trong Cài đặt → Quyền riêng tư. |
| Sai mã | Mã đổi mỗi phút, nhập mã đang hiện trên TV. |
| Phụ đề không hiện trên hình game | Kiểm tra đã cấp quyền hiển thị trên ứng dụng khác. Một số TV mất quyền này sau khi khởi động lại, cấp lại là được. |
| Phụ đề biến mất sau một lúc | Cho phép app bỏ qua tối ưu hóa pin trong cài đặt TV. |
| Muốn tắt app TV | Mở app TV, bấm **Tắt app**. |
| Muốn xóa iPhone đã ghép | Mở app TV, bấm **Xóa iPhone đã ghép nối**. |

Hỗ trợ: Telegram https://t.me/pstranslator
