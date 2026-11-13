# Chính sách riêng tư — Console Translator (iPhone, iPad)

Cập nhật: 14/11/2026

Console Translator không có tài khoản người dùng riêng, không có quảng cáo, không dùng công cụ theo dõi hay phân tích hành vi. Nhà phát triển chỉ dùng máy chủ (Cloudflare) cho các tính năng nêu bên dưới và không thu thập dữ liệu cá nhân của bạn.

## Dữ liệu được xử lý ngay trên máy

- **Hình ảnh game và video**: app đọc chữ phụ đề bằng công nghệ nhận dạng chữ của Apple, chạy ngay trên máy. Hình ảnh không được lưu lại và không được gửi đi đâu.
- **Giọng đọc**: giọng Apple và các giọng AI đều tạo âm thanh ngay trên máy.
- **Dịch bằng Apple Intelligence**: chạy ngay trên máy.
- **Cài đặt, hồ sơ game, bộ nhớ các câu đã dịch, nhật ký lỗi**: chỉ lưu trên máy của bạn và bị xoá khi bạn xoá app.
- **Khoá API (Gemini, OpenAI), thông tin ghép nối máy chơi game, PSN Account ID và mã đăng nhập Microsoft (Xbox)**: lưu trong Keychain, chỉ dùng trên máy này.

## Dữ liệu được gửi đi, chỉ khi bạn dùng tính năng tương ứng

- **Dịch AI (AI Tự động)**: câu phụ đề cần dịch, vài câu trước đó và tên game được gửi tới máy chủ dịch của nhà phát triển rồi tới nhà cung cấp AI để dịch. Để dịch nhanh và nhất quán, máy chủ giữ lại câu thoại của game kèm bản dịch theo từng game, không kèm thông tin nào về bạn hay thiết bị của bạn. Để tính lượt, app gửi mã giao dịch của gói Premium hoặc gói dùng thử miễn phí (máy chủ kiểm tra với Apple).
- **Dịch bằng Google Gemini hoặc OpenAI** (dùng khoá API của chính bạn): nội dung câu cần dịch được gửi thẳng tới nhà cung cấp đó và tuân theo chính sách riêng tư của họ.
- **Việt hóa (câu dịch sẵn của game)**: app tải gói câu từ máy chủ của nhà phát triển. Để mở gói, app gửi mã thiết bị một lần dùng của Apple DeviceCheck.
- **PSN Account ID**: app không đăng nhập Sony. Bạn tự tra Account ID trên một trang bên ngoài rồi dán vào app.
- **Chơi từ xa máy PlayStation**: app kết nối trực tiếp tới máy của bạn trong mạng nội bộ.
- **Chơi máy Xbox trong app**: bạn đăng nhập tài khoản Microsoft trên trang của chính Microsoft bằng một mã ngắn; app không bao giờ thấy mật khẩu. Mã đăng nhập Microsoft cấp chỉ dùng với dịch vụ Xbox của Microsoft để liệt kê máy và mở phiên chơi từ xa. Hình, tiếng và nút bấm đi giữa máy Xbox và thiết bị của bạn, được nối qua các dịch vụ đó. Trong mạng nội bộ, app còn tìm máy Xbox và có thể gửi tín hiệu bật máy. Không có thông tin nào của tài khoản Microsoft được gửi tới máy chủ của nhà phát triển. Bấm Đăng xuất ở màn Xbox để xoá mã đăng nhập.
- **Phụ đề trên TV**: bản dịch, câu gốc nếu bật, nội dung dịch vùng và giọng đọc nếu bật được gửi qua mạng nội bộ tới TV đã ghép bằng mã. Khi dùng TV qua trình duyệt, các nội dung này đi qua máy chủ chuyển tiếp của nhà phát triển và không được lưu lại. Khi cài app lên TV LG hoặc Samsung từ điện thoại, app gửi mã giao dịch PRO hoặc gói dùng thử để máy chủ kiểm tra với Apple trước khi cho tải.
- **Thông báo tin mới** (nếu bạn cho phép): mã nhận thông báo của thiết bị được gửi tới máy chủ của nhà phát triển để gửi tin. Tắt trong Cài đặt thì mã không còn được dùng.
- **Gửi nhật ký lỗi**: chỉ khi bạn bấm Gửi nhật ký, qua ứng dụng bạn chọn. Nhật ký không chứa khoá, mật khẩu, và địa chỉ IP đã được che.
- **Xem web và YouTube trong app**: các trang web bạn mở áp dụng chính sách riêng tư của họ.
- **Thông báo phiên bản và gói giọng tải thêm**: app đọc thông báo và tải gói giọng từ GitHub, kiểm tra phiên bản mới trên App Store. Các dịch vụ này có thể ghi nhận địa chỉ IP như với mọi lượt truy cập web.

## Mua trong app

Thanh toán do Apple xử lý hoàn toàn. App chỉ biết bạn đang có gói nào (Premium, PRO, dùng thử), không nhận thông tin thẻ hay thanh toán của bạn. Mã giao dịch chỉ được gửi tới máy chủ của nhà phát triển cho các tính năng nêu ở trên.

## Trẻ em

App không dành riêng cho trẻ em và không cố ý thu thập dữ liệu của trẻ em.

## Liên hệ

Kênh Telegram: https://t.me/pstranslator — hoặc tạo mục [Issues](https://github.com/kyozxx/PS-Translator/issues) trên GitHub.

---

# Privacy Policy — Console Translator (iPhone, iPad) (English)

Last updated: November 14, 2026

Console Translator has no user accounts of its own, no ads, and no tracking or analytics. The developer uses servers (Cloudflare) only for the features listed below and does not collect your personal data.

## Processed on your device

- **Game and video frames**: subtitles are read with Apple's on-device text recognition. Frames are not stored or sent anywhere.
- **Voices**: Apple voices and the AI voices are generated on the device.
- **Translation with Apple Intelligence**: runs on the device.
- **Settings, game profiles, the translation cache and the error log**: stored only on your device and removed when you delete the app.
- **API keys (Gemini, OpenAI), console pairing data, your PSN Account ID and your Microsoft (Xbox) sign-in token**: stored in the Keychain, used only on this device.

## Sent off the device, only when you use the feature

- **AI translation (AI Tự động)**: the subtitle line, a few previous lines and the game's name are sent to the developer's translation server and on to an AI provider to be translated. To translate quickly and consistently, the server keeps game lines with their translations per game, without anything about you or your device. To count lines, the app sends the transaction ID of your Premium subscription or free trial (checked with Apple).
- **Translation with Google Gemini or OpenAI** (with your own API key): the text to translate is sent directly to that provider and handled under its privacy policy.
- **Việt hóa (a game's ready-made translations)**: downloaded from the developer's server; to unlock it, the app sends a one-time Apple DeviceCheck token.
- **PSN Account ID**: the app does not sign in to Sony. You look up your Account ID on an outside page and paste it into the app.
- **PlayStation Remote Play**: the app connects directly to your console on your local network.
- **Playing your Xbox in the app**: you sign in to your Microsoft account on Microsoft's own page with a short code; the app never sees your password. The token Microsoft issues is used only with Microsoft's Xbox services to list your consoles and start a remote play session. Picture, sound and controller input go between your console and your device, connected through those services. On your local network the app also looks for your console and can send it a wake-up signal. Nothing from your Microsoft account is sent to the developer's servers. Sign out on the Xbox screen to delete the token.
- **Subtitles on your TV**: translated text, optional original text, area text and optional voice are sent over your local network to the TV you paired with its code. With the browser TV page, they pass through the developer's relay server and are not stored. When you install the TV app on an LG or Samsung TV from the phone, the app sends your PRO or trial transaction ID so the server can check it with Apple before the download.
- **News notifications** (if you allow them): your device's push token is sent to the developer's server to deliver news. Turning them off in Settings stops its use.
- **Sending the error log**: only when you tap Send, through the app you choose. The log contains no keys or passwords, and IP addresses are masked.
- **In-app web and YouTube**: the sites you open apply their own privacy policies.
- **Update notices and downloadable voices**: notices and voice packs come from GitHub, and new versions are checked on the App Store. These services may log IP addresses like any website.

## In-app purchases

Payments are handled entirely by Apple. The app only learns which plan you have (Premium, PRO, trial); it never receives your card or payment details. Transaction IDs are sent to the developer's server only for the features above.

## Children

The app is not directed at children and does not knowingly collect children's data.

## Contact

Telegram: https://t.me/pstranslator — or open an [issue](https://github.com/kyozxx/PS-Translator/issues) on GitHub.
