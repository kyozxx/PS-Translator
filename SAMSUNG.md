# Cài phụ đề cho TV Samsung bằng Windows hoặc Mac

Dùng Windows 10/11 hoặc macOS 12 trở lên để cài app nhận phụ đề lên TV Samsung Smart TV (Tizen, 2017 trở lên). Khi chơi, điện thoại vẫn chạy dịch và gửi phụ đề sang TV; PC chỉ cần lúc cài.

[Tải bộ cài PC 1.0.11](https://github.com/kyozxx/PS-Translator/releases/download/samsung-pc-1.0.11/ConsoleTranslator-Samsung-PC-1.0.11.zip) · [Tải bộ cài Mac 1.0.8](https://github.com/kyozxx/PS-Translator/releases/download/samsung-mac-1.0.8/ConsoleTranslator-Samsung-Mac-1.0.8.zip) · [Nhận mã cài và hỗ trợ](https://t.me/pstranslator)

Tải file `ConsoleTranslator-Samsung-PC-1.0.11.zip`, không tải **Source code**. Bộ cài đang thử nghiệm; khả năng hiện hình HDMI tùy mẫu TV/firmware.

## TV nào dùng được

- **Được (thử nghiệm):** TV Samsung từ năm 2017 trở lên, tức Tizen 3.0 trở lên. Một số TV đời 2018 đang gặp lỗi khi cài, chưa xử lý xong.
- **Không dùng được:** TV Samsung 2016 trở về trước (Tizen 2.x). Bộ cài sẽ báo `SS-OLD_TV` và dừng.

Xem đời TV Samsung: trên TV vào **Cài đặt → Hỗ trợ → Giới thiệu về TV**, xem mã model. Chữ cái sau số inch thường cho biết năm: K = 2016, M = 2017, N = 2018, R = 2019, T = 2020, A = 2021, B = 2022, C = 2023, D = 2024. Chắc chắn hơn: bấm **Kiểm tra kết nối TV** trong bộ cài, nhật ký hiện dòng "TV: Tizen ..."; Tizen 2.x là TV 2016 trở về trước.

Nếu bạn định mua gói PRO hoặc donate chỉ để dùng phụ đề trên TV Samsung, hãy cài app lên TV trước. Cài app TV không mất tiền; khi TV hiện được hình cổng HDMI và mã ghép nối 8 số thì hãy mua.

## Khác TV LG và Android TV ở đâu

TV Samsung không cho app nằm đè lên cổng HDMI. App **Console Translator TV** tự hiện hình cổng HDMI bên trong nó rồi vẽ phụ đề lên trên. Vì vậy bạn mở app trên TV để chơi, và bấm **Home** hoặc đổi nguồn sẽ thoát cả hình lẫn phụ đề; mở lại app để chơi tiếp.

## Ba bước cài từ Windows, không cần dòng lệnh

Giải nén ZIP, nhấp đúp **Chay-Console-Translator.bat**. Nếu công cụ báo thiếu Node.js, bấm **Cài Node.js**, cài bản LTS rồi quay lại. Công cụ hiện đúng ba bước dưới đây, đánh số 1, 2, 3.

### Bước 1. Bật Developer Mode trên TV (làm trên TV, chỉ lần đầu)

1. Trên TV mở **Ứng dụng (Apps)**, bấm lần lượt **1 2 3 4 5** trên remote. Remote không có phím số: bấm nút **123** để hiện bàn phím số trên màn hình.
2. Gạt **Developer mode** sang **On**.
3. Ở ô **Host PC IP** trên TV, nhập **dãy số lớn màu vàng trong ô Bước 1 của công cụ**. Đó là IP của máy tính, không phải của TV.
4. Bấm **OK**, rồi **rút dây nguồn TV 30 giây** và cắm lại. Tắt bằng remote không đủ.

Đây là bước hay bị bỏ sót nhất: chưa nhập IP máy tính vào TV thì bước 2 luôn báo không kết nối được.

### Bước 2. Kết nối TV

1. Xem IP của TV ngay trên TV: **Cài đặt → Chung → Mạng → Trạng thái mạng → Cài đặt IP**.
2. Nhập số đó vào ô **Địa chỉ IP của TV** trong công cụ, bấm **Kiểm tra kết nối TV**. Đây là IP của TV, khác với số ở bước 1; nhập nhầm IP máy tính thì công cụ báo ngay.

PC, điện thoại và TV cần cùng mạng Wi-Fi/LAN.

### Bước 3. Cài Console Translator

1. Dán đầy đủ **mã cài bắt đầu bằng `SSTV-`** được cấp riêng qua Telegram, bấm **Cài đặt lên TV**.
2. Trình duyệt mở trang Samsung: **đăng nhập tài khoản Samsung của bạn** (miễn phí, email đã xác minh). Samsung chỉ cho cài app ngoài cửa hàng khi gói cài được cấp chứng chỉ cho đúng TV của bạn; công cụ dùng lần đăng nhập này để xin chứng chỉ đó và không thấy mật khẩu.
3. Chờ thông báo đã cài xong. Cần Internet để kiểm tra mã và tải gói.

## Cài từ Mac, không cần Terminal

1. [Tải bộ cài Mac 1.0.8](https://github.com/kyozxx/PS-Translator/releases/download/samsung-mac-1.0.8/ConsoleTranslator-Samsung-Mac-1.0.8.zip), giải nén rồi kéo **Console Translator Samsung** vào **Applications**. Hỗ trợ macOS 12 trở lên, Intel và Apple Silicon.
2. Mở app. Nếu macOS chặn vì app chưa notarize: bấm **Xong**, vào **Cài đặt hệ thống → Quyền riêng tư & Bảo mật → Vẫn mở**, rồi bấm **Mở**. Chỉ làm với ZIP từ GitHub chính thức.
3. Nếu thiếu Node.js, bấm **Cài Node.js** trong app. Mac Apple Silicon cần thêm **Rosetta** của Apple; app sẽ hỏi và cài giúp.
4. Làm theo ba bước đánh số trong app, giống bản Windows ở trên: **Bước 1** nhập **IP máy Mac** (số lớn trong app) vào ô **Host PC IP** trên TV rồi rút điện TV; **Bước 2** nhập IP của TV, bấm **Kiểm tra kết nối TV**; **Bước 3** nhập mã **SSTV** cấp riêng cho Mac, bấm **Cài đặt lên TV**, đăng nhập tài khoản Samsung khi trình duyệt mở.

Mã đã kích hoạt trên Windows không chuyển sang Mac, hãy nhận mã riêng.

## Mỗi lần chơi

1. Bật PX5/máy chơi game.
2. Trên TV mở **Ứng dụng → Console Translator TV**. Lần đầu chọn cổng HDMI của máy chơi game.
3. Chạy dịch trên điện thoại như bình thường. Không cần máy tính.

Phím remote trong app: **Lên** đổi cổng HDMI, **Đỏ** xóa điện thoại đã ghép, **Back** thoát.

## Ghép phụ đề từ điện thoại

1. Mở **Phụ đề trên TV** trong app trên iPhone hoặc Android, chọn TV hoặc nhập địa chỉ hiện trên TV.
2. Nhập mã phụ đề **8 số** đang hiện trên TV, bấm **Kết nối**. Chỉ cần làm lần đầu. Đây không phải mã cài PC.

## Cập nhật app trên TV

Không cần tải lại bộ cài (từ bản 1.0.2). Mở công cụ trên máy tính, bấm **Kiểm tra kết nối TV**: nếu có bản mới, nhật ký hiện dòng **CÓ BẢN MỚI**. Bấm **Cài đặt lên TV** để cập nhật. Máy tính đã kích hoạt có thể để trống ô mã cài; vẫn đăng nhập tài khoản Samsung như lần đầu. TV không tự cập nhật được app cài qua Developer Mode.

Đang dùng bộ cài 1.0.0 hoặc 1.0.1: tải bản 1.0.2 một lần, giải nén vào thư mục mới.

Bộ cài cũng tự báo khi chính nó có bản mới: mỗi lần bấm một nút, nhật ký hiện dòng **BỘ CÀI CÓ BẢN MỚI** kèm link tải nếu bạn đang dùng bản cũ.

## Phân biệt các mã

| Mã | Dùng ở đâu? |
| --- | --- |
| `SSTV-YYYYMMDD-HHMMSS-…` | Nhập trong bộ cài Windows hoặc Mac cho TV Samsung. |
| `LGTV-YYYYMMDD-HHMMSS-…` | Chỉ dùng cho bộ cài TV LG, không dùng cho Samsung. |
| `ANDROID-YYYYMMDD-HHMMSS-…` | Kích hoạt app beta trên điện thoại Android. |
| Mã phụ đề 8 số trên Console Translator TV | Nhập trên điện thoại để ghép phụ đề; mã đổi mỗi phút. |

Mã cài gắn với tài khoản Windows đã kích hoạt; đổi máy cần mã mới. Mã hết hạn hoặc bị thu hồi chỉ chặn cài tiếp, app đã cài trên TV vẫn mở được.

## Dữ liệu

- ZIP không chứa app TV. Công cụ tải app sau khi kiểm tra mã, ký riêng cho TV của bạn rồi cài.
- PC chỉ lưu IP của TV và khóa máy. Mã cài, đăng nhập Samsung và chứng chỉ không được lưu.

## Nếu chưa được

- **Bấm 1 2 3 4 5 không hiện gì:** phải đang ở đúng màn hình Ứng dụng; bấm nhanh liền mạch hoặc dùng bàn phím số trên màn hình.
- **Không kết nối TV:** từ bản Windows 1.0.6 / Mac 1.0.5 công cụ nói rõ nguyên nhân:
  - `SS-NO_DEVICE`: không có thiết bị nào ở IP đã nhập. Sai IP của TV, TV đang tắt, hoặc TV và máy tính khác mạng.
  - `SS-PORT_CLOSED`: đúng TV nhưng Developer Mode chưa có hiệu lực. Rút dây nguồn TV 30 giây rồi cắm lại, mở Ứng dụng phải thấy chữ Developer Mode.
  - `SS-REFUSED`: TV từ chối máy tính này. Ô Host PC IP trên TV chưa đúng số ở Bước 1 (IP máy tính có thể đổi sau khi khởi động lại modem), hoặc máy tính đang bật VPN.
  - `SS-SDB_RUN`: máy tính không chạy được công cụ kết nối của Samsung, thường do phần mềm diệt virus chặn.
- **Cài báo `SS-TV_CERTIFICATE` hoặc `install failed[118012]`:** TV không nhận chứng chỉ vừa cấp, thường do đồng hồ TV chạy chậm hoặc sai ngày. Từ bản Windows 1.0.8 / Mac 1.0.7 bộ cài gửi kèm giấy phép cài đặt của Samsung lên TV (TV đời 2018 trở về trước cần file này), và tự chờ rồi thử lại vài lần. Nếu vẫn lỗi: trên TV vào **Cài đặt → Chung → Quản lý hệ thống → Thời gian → Đồng hồ**, chọn **Tự động**, rồi cài lại.
- **`SS-OLD_TV`:** TV sản xuất trước năm 2017 (Tizen 2.x). App cần Tizen 3.0 trở lên nên TV này chưa cài được. Xem đời TV: bấm **Kiểm tra kết nối TV**, dòng "TV: Tizen ..." trong nhật ký.
- **Không cài được:** kiểm tra mã SSTV còn hạn và kết nối Internet. Nếu vẫn lỗi, sao chép toàn bộ log (có mã lỗi `SS-…`) gửi hỗ trợ.
- **Không thấy cổng HDMI trong app:** cắm và bật máy chơi game trước khi mở app.
- **App biến mất sau một thời gian:** TV có thể tự tắt Developer Mode và gỡ app cài ngoài. Bật lại Developer Mode rồi cài lại.

[Telegram hỗ trợ](https://t.me/pstranslator)
