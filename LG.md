# Cài phụ đề cho TV LG bằng PC

Dùng Windows 10/11 để cài app nhận phụ đề lên TV LG webOS. Khi chơi, điện thoại vẫn phải chạy dịch và gửi phụ đề sang TV; PC chỉ dùng cài hoặc mở app TV.

[Tải bộ cài PC 1.2.5](https://github.com/kyozxx/PS-Translator/releases/download/lg-pc-1.2.5/ConsoleTranslator-LG-PC-1.2.5.zip) · [Nhận mã cài và hỗ trợ](https://t.me/pstranslator)

Tải file `ConsoleTranslator-LG-PC-1.2.5.zip`, không tải **Source code**. Bộ cài đang thử nghiệm; khả năng hiện phụ đề trên HDMI tùy mẫu TV/firmware.

## 1. Chuẩn bị trên TV, làm lần đầu

1. Tạo tài khoản tại [LG Developer](https://webostv.developer.lge.com).
2. Trong LG Apps / LG Content Store trên TV, cài **Developer Mode**, mở và đăng nhập.
3. Bật **Dev Mode Status**, chờ TV khởi động lại. Mở lại Developer Mode, bật **Key Server**.
4. Giữ màn hình này để xem **IP** và **Passphrase**. PC, điện thoại và TV cần cùng mạng Wi-Fi/LAN.

## 2. Cài từ Windows, không cần dòng lệnh

1. Tải ZIP phía trên, chuột phải → **Extract All / Giải nén tất cả**. Mở thư mục đã giải nén.
2. Nhấp đúp **Chay-Console-Translator.bat**. Nếu báo thiếu Node.js, bấm **Cài Node.js**, cài bản LTS từ trang chính thức rồi đóng và mở lại công cụ. Cần Node.js 22 trở lên.
3. Nhập IP và Passphrase đang hiện trên TV, bấm **Kết nối TV**. Kiểm tra đúng IP TV trước khi xác nhận khóa kết nối.
4. Nhập **mã cài PC** được cấp riêng qua Telegram, bấm **Cài đặt lên TV**, chờ thông báo hoàn tất. Cần Internet để kiểm tra mã và tải gói.

Không cần tìm hoặc chọn file IPK. Công cụ tự tải và gửi sang TV. PC nhớ IP/Passphrase sau khi kết nối thành công; nếu thông tin trên TV đổi, nhập lại rồi kết nối.

## 3. Mở app mỗi lần chơi

1. Bật PX5/máy chơi game trước, chuyển TV sang cổng HDMI có hình game.
2. Bật Key Server trong Developer Mode nếu chưa bật, sau đó quay về HDMI.
3. Mở công cụ Windows, bấm **Mở app trên TV**. Khi mã phụ đề hiện trên TV, có thể đóng công cụ PC.

Tránh bấm Home, Cài đặt hoặc mở app khác bằng remote TV khi chơi. Chỉ nên dùng tăng/giảm âm lượng. Nếu phụ đề đóng, bấm **Mở app trên TV** lại.

## 4. Ghép phụ đề từ iPhone

1. Mở **Dịch khi chơi TV**, bấm nút TV trên thanh công cụ. Tính năng phụ đề TV trên iOS cần **Console Translator PRO**; mã cài PC không thay cho PRO.
2. Chọn loại TV LG, chọn TV trong danh sách hoặc nhập địa chỉ hiện trên TV.
3. Nhập mã phụ đề **8 số** đang hiện trên TV, bấm **Kết nối**. Đây không phải Passphrase hay mã cài PC.
4. Giữ hình game trên iPhone, chọn đúng vùng chữ gốc và chạy dịch. Chỉnh chữ/giọng đọc trong phần phụ đề TV.

## Ba loại mã khác nhau

| Mã | Dùng ở đâu? |
| --- | --- |
| Mã cài PC do người hỗ trợ cấp | Nhập trong công cụ Windows để cài app TV. Mã beta Android không dùng thay được. |
| Passphrase 6 ký tự trên Developer Mode | Dùng kết nối TV, phân biệt chữ hoa/chữ thường. Dùng đúng giá trị đang hiện trên TV. |
| Mã phụ đề 8 số trên Console Translator TV | Nhập trên điện thoại để ghép phụ đề; mã hiện trên TV đổi mỗi phút. |

Mã cài PC hết hạn hoặc bị thu hồi sẽ chặn cài tiếp, app đã cài trên TV vẫn mở được. Thời hạn Developer Mode là riêng: TV cần kết nối mạng, mở Developer Mode và bấm **EXTEND** trước khi hết hạn. Nếu Developer Mode bị tắt, app cài ngoài có thể bị gỡ và phải cài lại.

## Nếu chưa được

- **Không kết nối TV:** kiểm tra cùng mạng, IP/Passphrase mới nhất, Key Server bật và Developer Mode còn hạn.
- **Không cài được:** kiểm tra mã cài PC còn hạn, Internet và giờ Windows đúng; gửi ảnh lỗi cho hỗ trợ trước khi gỡ bản đang dùng.
- **Không thấy phụ đề:** kiểm tra điện thoại đang chạy dịch, vùng quét có chữ gốc và đã ghép TV. Nếu vừa bấm Home, mở app TV lại.

[Hướng dẫn Developer Mode chính thức của LG](https://webostv.developer.lge.com/develop/getting-started/developer-mode-app) · [Telegram hỗ trợ](https://t.me/pstranslator)
