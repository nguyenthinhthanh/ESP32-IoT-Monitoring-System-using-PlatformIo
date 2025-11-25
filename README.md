# ESP32 IoT Monitoring System

Trong bối cảnh hiện nay, nhu cầu giám sát và quản lý các yếu tố môi trường ngày càng trở nên cấp
thiết, đặc biệt trong các lĩnh vực như nông nghiệp thông minh, đô thị hiện đại, và bảo vệ sức khỏe cộng
đồng. Tuy nhiên, phần lớn các hệ thống quan trắc môi trường đang được triển khai thường có chi phí
đầu tư cao, khó mở rộng, hoặc thiếu khả năng tùy chỉnh linh hoạt theo nhu cầu thực tế. Điều này đặt ra
yêu cầu về một giải pháp quan trắc mới: nhẹ, linh hoạt, dễ triển khai, nhưng vẫn đảm bảo đầy đủ các
chức năng cốt lõi của một hệ thống IoT hiện đại.

Xuất phát từ thực tiễn đó, dự án hướng tới xây dựng một mô hình:

- Giám sát môi trường theo thời gian thực với khả năng tích hợp nhiều loại cảm biến khác nhau (nhiệt độ, độ ẩm, chất lượng không khí, ánh sáng…).

- Tự cấu hình thông qua WebServer tích hợp khi thiết bị khởi động lần đầu, cho phép cấu hình WiFi, MQTT, cảm biến, ngưỡng cảnh báo và các tham số vận hành mà không cần lập trình lại.

- Truy xuất và hiển thị dữ liệu tập trung trên dashboard trực quan, hỗ trợ theo dõi và phân tích nhanh chóng.

- Hỗ trợ cảnh báo thông minh, dựa trên ngưỡng người dùng thiết lập hoặc phân tích bằng TinyML để phát hiện dữ liệu bất thường.

- Tích hợp OTA, lưu trữ cấu hình bền vững, đa nhiệm bằng FreeRTOS và cơ chế tự khôi phục kết nối WiFi/MQTT.

- Có website giám sát dữ liệu, lấy dữ liệu trực tiếp từ nền tảng CoreIoT, hiển thị dưới dạng biểu đồ, bảng và trạng thái thiết bị, giúp người dùng quan sát từ xa mà không cần truy cập trực tiếp vào thiết bị.

- Có khả năng mở rộng, phù hợp cho các hệ thống lớn hơn hoặc các ứng dụng thực tế như nông nghiệp, nhà thông minh, hoặc giám sát không khí.

## Bối cảnh ứng dụng thực tế
1. **Nhà kính/nông nghiệp**: đo độ ẩm đất, nhiệt độ, kích hoạt hệ thống tưới hoặc quạt để giữ môi trường tối ưu.
2. **Giám sát trong nhà**: theo dõi chất lượng không khí, cảnh báo khi vượt ngưỡng, bật purifer hoặc quạt.
3. **Phòng lab/kho lưu trữ**: ghi log môi trường, gửi dữ liệu lên dashboard để đội vận hành xem và xuất báo cáo.

## Yêu cầu phần cứng
- ESP32 (tích hợp Wi-Fi/Bluetooth, hỗ trợ FreeRTOS).
- Cảm biến nhiệt độ & độ ẩm (DHTxx hoặc tương đương).
- Cảm biến chất lượng không khí (PM/VOC).
- Dải LED NeoPixel hiển thị trạng thái.
- Relay kênh đơn hoặc nhiều kênh để điều khiển tải AC/DC.
- Cảm biến NPK cho đất, cảm biến không khí,...mô-đun TinyML để phân tích dữ liệu tại thiết bị.
- Nguồn cấp và khối vỏ bảo vệ phù hợp với môi trường triển khai.

## Chức năng chính
Dự án **Hệ thống IoT Quan Trắc cảm biến trên Node WiFi 32 – ESP32** được xây dựng với các chức năng chính sau:

### 1. Giám sát môi trường
- Thu thập dữ liệu từ nhiều loại cảm biến: nhiệt độ, độ ẩm, ánh sáng, chất lượng không khí,...
- Cập nhật dữ liệu theo thời gian thực.
- Đảm bảo độ ổn định và độ chính xác trong quá trình đo lường.

### 2. WebServer cấu hình thiết bị
- Tự động tạo Access Point khi mới khởi động lần đầu.
- Cho phép cấu hình các tham số:
  - Thông tin WiFi (SSID, password)
  - Thông tin MQTT broker
  - Loại cảm biến sử dụng
  - Ngưỡng cảnh báo
  - Các thông số vận hành khác
- Không cần lập trình lại thiết bị khi thay đổi cấu hình.

### 3. Truy xuất và hiển thị dữ liệu
- Hệ thống dashboard trực quan hiển thị trạng thái thiết bị và dữ liệu cảm biến.
- Dữ liệu được lấy trực tiếp từ nền tảng CoreIoT.
- Hỗ trợ biểu đồ, bảng, chỉ báo trạng thái,... phục vụ phân tích nhanh.

### 4. Cảnh báo thông minh
- Gửi cảnh báo khi giá trị vượt ngưỡng do người dùng thiết lập.
- Hỗ trợ phân tích dữ liệu bất thường bằng TinyML, giúp nâng cao khả năng phát hiện sai lệch.

### 5. Hỗ trợ OTA (Over-The-Air)
- Cho phép cập nhật firmware từ xa.
- Giảm thời gian bảo trì và tăng tính linh hoạt cho hệ thống.

### 6. Lưu trữ cấu hình và hoạt động ổn định
- Cấu hình được lưu trong bộ nhớ bền vững.
- FreeRTOS đảm bảo thiết bị hoạt động đa nhiệm.
- Tự động khôi phục khi mất kết nối WiFi hoặc MQTT.

### 7. Website giám sát từ xa
- Giao diện web trực quan, dễ sử dụng.
- Hiển thị dữ liệu từ hệ thống CoreIoT:
  - Biểu đồ theo thời gian
  - Bảng dữ liệu
  - Trạng thái thiết bị
- Người dùng có thể theo dõi từ bất kỳ đâu mà không cần truy cập vào thiết bị nội bộ.

### 8. Khả năng mở rộng
- Thiết kế phù hợp cho các hệ thống lớn.
- Có thể triển khai cho:
  - Nông nghiệp thông minh
  - Nhà thông minh
  - Giám sát môi trường, chất lượng không khí
  - Các ứng dụng IoT khác.

## Cách triển khai 
1. **Clone repository**
```bash
https://github.com/nguyenthinhthanh/ESP32-IoT-Monitoring-System-using-PlatformIo
```
2. **Mở dự án bằng PlatformIO**
   - Mở thư mục dự án trong VSCode + PlatformIO.
   - Kiểm tra file platformio.ini và cấu trúc thư mục src, data.

3. **Upload code và giao diện Web**
- Upload giao diện Web vào SPIFFS/LittleFS:
```bash
  Xóa bộ nhớ flash: Chọn Platform -> Erase Flash
  Build UI Web: Chọn Platform -> Build Filesystem Image
  Upload UI Web: Chọn Platform -> Upload Filesystem Image
```
- Upload firmware::
```bash
  Build: Chọn Gerneral -> Build
  Upload: Chọn Gerneral -> Upload
```

## Đóng góp
Bạn có ý tưởng cải thiện trò chơi? Hãy mở Pull Request hoặc Issue trên GitHub!

## Giấy phép
Dự án này được tạo ra **chỉ nhằm mục đích học tập**. Không được sử dụng cho mục đích thương mại.
