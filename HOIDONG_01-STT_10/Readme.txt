Các file trong thư mục và chức năng của từng file:
1/ Thư mục "Noi dung" bao gồm 2 file của cuốn báo cáo đồ án dưới dạng .docx và .pdf
đã được chỉnh sửa theo yêu cầu của hội đồng sau khi bảo vệ.
2/ Thư mục "Ma nguon" bao gồm 2 thư mục con là: 15119081_RFID_MQTT_Thinkspeak_Web_CBAS_final
và GiaoDienWeb
     +/ Thư mục con "15119081_RFID_MQTT_Thinkspeak_Web_CBAS_final" chứa mã nguồn nạp 
      	cho bộ xử lý trung tâm NodeMCU ESP8266. Có tác dụng đọc giá trị cảm biến ánh sáng, 
	xử lý điều khiển thiết bị bằng thẻ RFID và có gửi giá trị lên ThinkSpeak qua giao
	thức HTTP, xử lý điều khiển thiết bị qua giao thức MQTT.
     +/ Thư mục con "Giao diện web" chứa mã nguồn chạy trên phần mềm NodeJS. Có tác dụng 
	xử lý điều khiển thiết bị và cập nhật trạng thái của thiết bị khi giao tiếp MQTT.
	Thư mục "Server" có chức năng giao tiếp với Client trong việc truyền nhận thông tin
	điều khiển, đồng thời giao tiếp với MQTT. Có thể mô tả như sau: Client-Server-MQTT.
	Thư mục "Client" có chức năng truyền nhận thông tin điều khiển và trạng thái thiết bị
	với Server.