- Đầu tiên là folder dataset: đây là nơi chứa các thư mục con bao gồm hình chụp khuôn mặt của mỗi nhân viên trong hệ thống. 
- Captureface.py là chương trình dùng để chụp lại khuôn mặt của nhân viên khi ta cần thêm vào hệ thống.
- Encode_faces.py là chương trình dùng để trích vector đặc trưng cho từng khuôn mặt có trong dataset.
- Encodings.pickle là file lưu lại đặc trưng tương ứng cho mỗi khuôn mặt của mỗi nhân viên trong hệ thống.
- Pi_face_recognition.py là chương trình dùng để nhận diện khuôn mặt nhân viên và ghi thời gian chấm công vào excel file.
- Haarcascade_frontalface_default.xml: là file chứa đặc trưng dùng để phát hiện khuôn mặt của OpenCV. 
- Main.py là chương trình giao diện của hệ thống.
- Voice.mp3 là file âm thanh dùng để báo hiệu khi nhân diện đúng khuôn mặt có trong hệ thống.
- 12 file excel đại diện cho 12 tháng trong năm. Hệ thống sẽ ghi thời gian chấm công tương ứng với từng tháng trong năm vào các file excel tương ứng. 
- Create_excel.py là file dùng để tạo ra 12 file excel khi folder hiện tại không có file excel
-------------------------------------------------------------------------------------------------
Để chạy hệ thống đầu tiên cần chạy chương trình Main.py. Sau đó click vào các nút Add,Encode hoặc Recognition để thực hiện
các chức năng cần thiết. 
+ Add dùng để thêm khuôn mặt cho nhân viên mới ( Lưu ý phải Add theo định dạng 001,002...tương tự cho cho số 4 chữ số là 0001,0002)
+ Encode dùng để trích đặc trưng và gán nhãn cho từng bức hình trong thư mục dataset
+ Recognition dùng để nhận diện khuôn mặt nhân viên khi ra vào cổng và ghi thời gian vào các file excel tương ứng với từng tháng
Chú ý : Khi nhấn Add thì mỗi lần nhấn phím K hệ thống sẽ chụp 1 bức hình, để nhận diện nhanh chóng thì cần add mỗi nhân viên khoảng
10 hình. Sau khi Add xong nhấn phím Q để thoát chương trình và quay lại giao diện.
-------------------------------------------------------------------------------------------------
Tuy nhiên để có thể chạy được tất cả chương trình raspberry cần đảm bảo là phải cài đặt đủ các thư viện cần thiết
+ OpenCV 3.3.0 , python 3.5.3 , pip3 
+ Sau đó sử dụng pip3 để cài các package cần thiết dlib, face_recognition, imutils, openpyxl, tkinter, pygame

