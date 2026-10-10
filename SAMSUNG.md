# Cài phụ đề cho TV Samsung bằng Windows

Dùng Windows 10/11 để cài app nhận phụ đề lên TV Samsung Smart TV (Tizen, 2017 trở lên). Khi chơi, điện thoại vẫn chạy dịch và gửi phụ đề sang TV; PC chỉ cần lúc cài.

[Tải bộ cài PC 1.0.1](https://github.com/kyozxx/PS-Translator/releases/download/samsung-pc-1.0.1/ConsoleTranslator-Samsung-PC-1.0.1.zip) · [Nhận mã cài và hỗ trợ](https://t.me/pstranslator)

Tải file `ConsoleTranslator-Samsung-PC-1.0.1.zip`, không tải **Source code**. Bộ cài đang thử nghiệm; khả năng hiện hình HDMI tùy mẫu TV/firmware.

## Khác TV LG và Android TV ở đâu

TV Samsung không cho app nằm đè lên cổng HDMI. App **Console Translator TV** tự hiện hình cổng HDMI bên trong nó rồi vẽ phụ đề lên trên. Vì vậy bạn mở app trên TV để chơi, và bấm **Home** hoặc đổi nguồn sẽ thoát cả hình lẫn phụ đề; mở lại app để chơi tiếp.

## 1. Chuẩn bị trên TV, làm lần đầu

1. Giải nén ZIP, nhấp đúp **Chay-Console-Translator.bat**. Dòng **IP máy tính này** nằm dưới nút Kiểm tra kết nối TV.
2. Trên TV mở **Ứng dụng (Apps)**, bấm lần lượt **1 2 3 4 5** trên remote. Remote không có phím số: bấm nút **123** để hiện bàn phím số trên màn hình.
3. Gạt **Developer mode** sang **On**, nhập **IP máy tính** vào ô **Host PC IP**, bấm **OK**.
4. **Rút dây nguồn TV 30 giây** rồi cắm lại. Tắt bằng remote không đủ.
5. Xem IP của TV: **Cài đặt → Chung → Mạng → Trạng thái mạng → Cài đặt IP**. PC, điện thoại và TV cần cùng mạng Wi-Fi/LAN.

## 2. Cài từ Windows, không cần dòng lệnh

1. Nếu công cụ báo thiếu Node.js, bấm **Cài Node.js**, cài bản LTS từ trang chính thức rồi mở lại công cụ. Cần Node.js 22 trở lên.
2. Nhập IP của TV, bấm **Kiểm tra kết nối TV**.
3. Dán đầy đủ **mã cài bắt đầu bằng `SSTV-`** được cấp riêng qua Telegram, bấm **Cài đặt lên TV**.
4. Trình duyệt mở trang Samsung: **đăng nhập tài khoản Samsung của bạn** (miễn phí, email đã xác minh). Samsung chỉ cho cài app ngoài cửa hàng khi gói cài được cấp chứng chỉ cho đúng TV của bạn; công cụ dùng lần đăng nhập này để xin chứng chỉ đó và không thấy mật khẩu.
5. Chờ thông báo **Cài đặt hoàn tất**. Cần Internet để kiểm tra mã và tải gói.

## 3. Mỗi lần chơi

1. Bật PX5/máy chơi game.
2. Trên TV mở **Ứng dụng → Console Translator TV**. Lần đầu chọn cổng HDMI của máy chơi game.
3. Chạy dịch trên điện thoại như bình thường. Không cần máy tính.

Phím remote trong app: **Lên** đổi cổng HDMI, **Đỏ** xóa điện thoại đã ghép, **Back** thoát.

## 4. Ghép phụ đề từ điện thoại

1. Mở **Phụ đề trên TV** trong app trên iPhone hoặc Android, chọn TV hoặc nhập địa chỉ hiện trên TV.
2. Nhập mã phụ đề **8 số** đang hiện trên TV, bấm **Kết nối**. Chỉ cần làm lần đầu. Đây không phải mã cài PC.

## Phân biệt các mã

| Mã | Dùng ở đâu? |
| --- | --- |
| `SSTV-YYYYMMDD-HHMMSS-…` | Nhập trong bộ cài Windows cho TV Samsung. |
| `LGTV-YYYYMMDD-HHMMSS-…` | Chỉ dùng cho bộ cài TV LG, không dùng cho Samsung. |
| `ANDROID-YYYYMMDD-HHMMSS-…` | Kích hoạt app beta trên điện thoại Android. |
| Mã phụ đề 8 số trên Console Translator TV | Nhập trên điện thoại để ghép phụ đề; mã đổi mỗi phút. |

Mã cài gắn với tài khoản Windows đã kích hoạt; đổi máy cần mã mới. Mã hết hạn hoặc bị thu hồi chỉ chặn cài tiếp, app đã cài trên TV vẫn mở được.

## Dữ liệu

- ZIP không chứa app TV. Công cụ tải app sau khi kiểm tra mã, ký riêng cho TV của bạn rồi cài.
- PC chỉ lưu IP của TV và khóa máy. Mã cài, đăng nhập Samsung và chứng chỉ không được lưu.

## Nếu chưa được

- **Bấm 1 2 3 4 5 không hiện gì:** phải đang ở đúng màn hình Ứng dụng; bấm nhanh liền mạch hoặc dùng bàn phím số trên màn hình.
- **Không kết nối TV:** kiểm tra cùng mạng, IP máy tính trong Developer Mode đúng IP hiện tại, và đã rút điện TV sau khi bật Developer Mode.
- **Không cài được:** kiểm tra mã SSTV còn hạn và kết nối Internet. Nếu vẫn lỗi, sao chép toàn bộ log (có mã lỗi `SS-…`) gửi hỗ trợ.
- **Không thấy cổng HDMI trong app:** cắm và bật máy chơi game trước khi mở app.
- **App biến mất sau một thời gian:** TV có thể tự tắt Developer Mode và gỡ app cài ngoài. Bật lại Developer Mode rồi cài lại.

[Telegram hỗ trợ](https://t.me/pstranslator)
