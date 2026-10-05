# Phụ đề trên TV – Hướng dẫn cài app TV

Phụ đề tiếng Việt hiện nổi ngay trên hình game ở TV. Máy chơi game vẫn cắm HDMI thẳng vào TV nên hình gốc, không trễ. Chỉ có chữ (và giọng đọc nếu bật) đi từ iPhone sang TV qua Wi-Fi.

> **Đang thử nghiệm nội bộ.** App TV chưa phát hành công khai. Nếu bạn nằm trong nhóm thử nghiệm, nhắn qua Telegram https://t.me/pstranslator để nhận file cài.

**Mục lục**
- [Cần có](#cần-có)
- [Android TV](#android-tv)
- [TV LG (webOS)](#tv-lg-webos)
- [Ghép nối với iPhone](#ghép-nối-với-iphone)
- [Lỗi thường gặp](#lỗi-thường-gặp)

## Cần có

- TV chạy **Android TV** hoặc **TV LG chạy webOS** (bản LG đang thử nghiệm).
- **Không hỗ trợ** TV hệ điều hành khác và Android Box.
- TV và iPhone dùng chung một mạng Wi-Fi.
- App Console Translator trên iPhone có gói PRO.
- Trên iPhone, chọn đúng loại TV trong màn **Phụ đề trên TV → Loại TV**.

---

## Android TV

### Bước 1: Cài app

Bạn nhận file **Console-Translator-TV.apk** từ nhóm thử nghiệm. Cách cài dễ nhất bằng remote TV:

1. Trên TV, mở Google Play, tìm và cài app **Downloader**.
2. Vào **Cài đặt TV → Ứng dụng → Quyền truy cập ứng dụng đặc biệt → Cài đặt ứng dụng không rõ nguồn**, bật cho **Downloader**.
3. Mở Downloader, gõ đường link tải được gửi cho bạn rồi bấm **Go**.
4. Tải xong, bấm **Cài đặt**.

Mẹo: đường link dài khó gõ bằng remote. Bạn có thể tạo mã ngắn trên trang aftv.news rồi gõ mã số đó vào Downloader.

### Bước 2: Cấp quyền hiển thị phụ đề

1. Mở app **Console Translator TV**.
2. Bấm **Cấp quyền hiển thị trên ứng dụng khác**, bật quyền cho app.
3. Nếu app hỏi **bỏ qua tối ưu hóa pin**, chọn **Cho phép** để TV không tắt app khi chuyển sang cổng HDMI.

### Bước 3: Mở app

Mở app Console Translator TV. Màn hình hiện **mã 8 số** và **địa chỉ TV**. Làm tiếp phần [Ghép nối với iPhone](#ghép-nối-với-iphone).

Muốn tắt app: bấm **Tắt app** trong app TV.

---

## TV LG (webOS)

Bản LG cài qua **Chế độ nhà phát triển** của LG, cần một máy Mac hoặc máy tính cùng Wi-Fi với TV. Bạn nhận file **com.consoletranslator.overlay_x.x.x_all.ipk** từ nhóm thử nghiệm.

### Bước 1: Bật Chế độ nhà phát triển trên TV (làm một lần)

1. Tạo tài khoản miễn phí tại https://webostv.developer.lge.com (Sign In → Create Account).
2. Trên TV, mở **LG Content Store**, tìm và cài app **Developer Mode**.
3. Mở Developer Mode, đăng nhập tài khoản vừa tạo.
4. Bật **Dev Mode Status** (TV khởi động lại), mở lại app rồi bật **Key Server**.

Chế độ nhà phát triển có thời hạn (hiện trong app Developer Mode). Khi sắp hết hạn, mở app và bấm **Extend**, nếu không app cài thêm sẽ bị xóa.

### Bước 2: Cài app từ máy tính

Máy tính cần có Node.js (tải tại nodejs.org).

1. Tải file `.ipk` nhận được vào thư mục **Tải về (Downloads)** của máy tính.
2. Mở Terminal, kiểm tra file đã có và chỉ có một bản (xóa các bản cũ hoặc trùng tên):
```
ls ~/Downloads | grep consoletranslator
```
3. Chạy lần lượt:

```
npx -y -p @webos-tools/cli ares-setup-device
```
Chọn **add**, nhập: tên `lg-tv`, IP của TV (xem trong app Developer Mode), port `9922`, user `prisoner`.

```
npx -y -p @webos-tools/cli ares-novacom --device lg-tv --getkey
```
Nhập passphrase đang hiện trong app Developer Mode trên TV.

Nếu đã cài bản cũ, gỡ trước:
```
npx -y -p @webos-tools/cli ares-install --device lg-tv --remove com.consoletranslator.overlay
```

Cài bản mới (thay `x.x.x` bằng phiên bản trong tên file), phải hiện `Success`:
```
npx -y -p @webos-tools/cli ares-install --device lg-tv ~/Downloads/com.consoletranslator.overlay_x.x.x_all.ipk
```

### Bước 3: Mở app khi đang chơi

1. Chuyển TV sang cổng HDMI của máy chơi game.
2. Mở app **Console Translator TV** từ danh sách ứng dụng của TV, hoặc từ máy tính:
```
npx -y -p @webos-tools/cli ares-launch --device lg-tv com.consoletranslator.overlay
```
3. Góc phải màn hình hiện **mã 8 số** và **địa chỉ TV**, hình game vẫn chạy phía sau. Làm tiếp phần [Ghép nối với iPhone](#ghép-nối-với-iphone).

Lưu ý với TV LG:
- Bấm nút **Home** trên remote TV sẽ đóng phụ đề (cách webOS hoạt động). Mở lại app là chạy tiếp, iPhone tự kết nối lại.
- Không cần root TV.

---

## Ghép nối với iPhone

1. Trên iPhone: **Dịch khi chơi TV → nút TV** trên thanh công cụ.
2. Ở **Loại TV**, chọn **Android TV** hoặc **TV LG**.
3. Chọn TV trong danh sách (hoặc nhập địa chỉ đang hiện trên TV), nhập **mã 8 số**, bấm **Kết nối**.
4. Chuyển TV sang cổng HDMI của máy chơi game. Phụ đề hiện trên hình game.

Mã đổi mỗi phút, chỉ cần nhập một lần: lần sau iPhone tự kết nối lại.

Chỉnh cỡ chữ, màu chữ, nền, vị trí, độ rộng dòng và giọng đọc ngay trên iPhone. Có nút **Gửi câu thử** để chỉnh mà không cần chơi.

## Lỗi thường gặp

| Lỗi | Cách xử lý |
|---|---|
| Android TV: "Đã xảy ra sự cố khi phân tích cú pháp gói" | File tải chưa xong hoặc bị hỏng. Tải lại bằng Downloader, đừng gửi file qua tin nhắn. |
| LG: `IPK Extraction Failure` khi cài | Gỡ bản cũ trước (`ares-install --device lg-tv --remove com.consoletranslator.overlay`), xóa các file .ipk trùng tên trong thư mục, chỉ giữ một file rồi cài lại. |
| LG: không cài được, báo lỗi kết nối | Kiểm tra Developer Mode còn hạn, Key Server đang bật, IP đúng, máy tính cùng Wi-Fi với TV. |
| iPhone không tìm thấy TV | Mở app TV trên TV, kiểm tra cùng Wi-Fi, hoặc nhập địa chỉ IP đang hiện trên TV. Kiểm tra iPhone đã cho phép **Mạng cục bộ** trong Cài đặt → Quyền riêng tư. |
| Sai mã | Mã đổi mỗi phút, nhập mã đang hiện trên TV. |
| Android TV: phụ đề không hiện trên hình game | Kiểm tra đã cấp quyền hiển thị trên ứng dụng khác. Một số TV mất quyền này sau khi khởi động lại, cấp lại là được. |
| Phụ đề biến mất sau một lúc | Android TV: cho phép app bỏ qua tối ưu hóa pin. TV LG: mở lại app (thường do đã bấm Home trên remote). |
| Muốn xóa iPhone đã ghép | Android TV: bấm **Xóa iPhone đã ghép nối** trong app TV. iPhone: bấm **Ngắt** trong màn Phụ đề trên TV. |

Hỗ trợ: Telegram https://t.me/pstranslator
