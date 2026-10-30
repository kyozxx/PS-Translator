# Cài phụ đề cho TV Samsung

TV Samsung từ 2017 trở lên. Máy tính và TV cùng Wi-Fi.

[Tải Windows](https://github.com/kyozxx/PS-Translator/releases/download/samsung-pc-1.0.18/ConsoleTranslator-Samsung-PC-1.0.18.zip) · [Tải Mac](https://github.com/kyozxx/PS-Translator/releases/download/samsung-mac-1.0.15/ConsoleTranslator-Samsung-Mac-1.0.15.zip) · [Lấy mã cài](https://t.me/consoletranslatorbot)

Windows 10/11. Mac Intel hoặc M1 trở lên, macOS 12+. Bản Mac đã được Apple công chứng.

## Cài trong 3 bước

1. **Mở bộ cài:** giải nén ZIP. Windows mở Chay-Console-Translator.bat. Mac kéo Console Translator Samsung vào Applications rồi mở.
2. **Chuẩn bị TV:** mở Home → Apps (Ứng dụng) → App Settings (Cài đặt ứng dụng). Bấm nút 123 (hoặc …) trên remote để hiện bàn phím số, chọn lần lượt 1 2 3 4 5. Khi bảng Developer Mode hiện ra, bật On. Điền IP máy tính hiện trong bộ cài vào Host PC IP, rồi rút điện TV 30 giây và cắm lại. [Xem video bật Developer Mode](https://www.youtube.com/watch?v=F8kkdIh1TGY).
3. **Cài lên TV:** nhập IP của TV vào bộ cài, bấm Kiểm tra kết nối TV. Gửi /samsung cho bot để lấy mã SSTV, nhập mã và bấm Cài đặt lên TV. Đăng nhập Samsung khi trình duyệt mở.

Cài xong, mở Console Translator TV trên TV, chọn HDMI rồi ghép điện thoại bằng mã 8 số. Từ lần sau không cần máy tính.

> **Lưu ý quan trọng khi mở máy chơi game:**
> - **Bật máy chơi game trước:** Bật máy PS5/Xbox/Switch trước để có sẵn tín hiệu hình ảnh, sau đó mới mở app Console Translator TV trên TV.
> - **Tắt tự chuyển cổng HDMI (HDMI-CEC):** Trên PS5 vào *Cài đặt → Hệ thống → HDMI* và tắt **Bật liên kết thiết bị HDMI** (hoặc tắt **Anynet+** trên TV Samsung). Nếu không tắt, khi bật máy chơi game TV sẽ tự động nhảy sang cổng HDMI ngoài làm TV thoát khỏi app, khiến điện thoại báo đã kết nối nhưng màn hình không thể hiện phụ đề.
> - **Kiểm tra app đang chạy:** Bấm nút **Lên** trên remote Samsung. Nếu hiện bảng chọn cổng HDMI ở góc trên trái nghĩa là app vẫn đang hoạt động tốt đè lên hình game.

<details><summary>Mở máy chơi game bị mất phụ đề hoặc app nháy rồi vào thẳng game</summary>

Do tính năng HDMI-CEC (Anynet+ trên TV hoặc Liên kết thiết bị HDMI trên PS5) tự động chuyển cổng sang HDMI ngoài khiến TV thoát app. Khắc phục: 1. Tắt Bật liên kết thiết bị HDMI trên PS5 (Cài đặt → Hệ thống → HDMI) hoặc tắt Anynet+ trên TV. 2. Bật máy chơi game trước, sau đó mới mở app Console Translator TV trên TV. Bấm nút Lên trên remote Samsung để kiểm tra: nếu hiện bảng chọn cổng HDMI nghĩa là app đang chạy đè lên game bình thường.

</details>

<details><summary>Cập nhật bản cũ</summary>

Tải bộ cài mới ở trên rồi bấm Cài đặt lên TV. Công cụ tự gỡ bản cũ trước khi cài lại. Có thể cần chọn lại HDMI và ghép lại điện thoại.

</details>

<details><summary>Không kết nối được TV</summary>

Host PC IP trên TV là IP máy tính. Ô IP trong bộ cài là IP của TV, xem tại Cài đặt → Mạng → Trạng thái mạng. Sau khi bật Developer Mode phải rút điện TV 30 giây.

</details>

<details><summary>Mac báo lỗi Terminal hoặc không mở được app</summary>

Giải nén ZIP bằng Finder, kéo app vào Applications rồi mở. Không cần lệnh file hay bash. Tải lại bản mới ở đầu trang nếu đang dùng bản cũ. Thiếu Node.js hoặc Rosetta thì làm theo nút cài trong app.

</details>

<details><summary>Cài đặt báo lỗi</summary>

SS-REMOVE: gỡ app bằng remote rồi cài lại. Lỗi chứng chỉ: kiểm tra ngày giờ TV ở chế độ Tự động. Nếu vẫn lỗi, gửi ảnh thông báo và phiên bản bộ cài vào [nhóm hỗ trợ](https://t.me/pstranslator). Che mã SSTV trước khi gửi.

</details>

<details><summary>Không thấy bảng Developer Mode</summary>

Vào App Settings bên trong Apps rồi mới mở bàn phím số bằng nút 123 hoặc … trên remote. TV giao diện cũ thì thử ngay tại màn hình Apps. Không cần tải app Developer Mode. [Hướng dẫn Samsung](https://developer.samsung.com/smarttv/develop/getting-started/using-sdk/tv-device.html).

</details>
