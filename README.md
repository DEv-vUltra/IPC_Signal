<!doctype html>
<html lang="vi">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>README — IPC-Signal</title>
  <style>
    body{font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, 'Helvetica Neue', Arial; line-height:1.6; padding:24px; max-width:980px; margin:0 auto; color:#111}
    h1,h2,h3{color:#0b66c3}
    pre{background:#f6f8fa; padding:12px; border-radius:6px; overflow:auto}
    code{background:#eef3fb; padding:2px 6px; border-radius:4px}
    img{max-width:100%; height:auto; border:1px solid #ddd; border-radius:6px}
    .note{background:#fff8c6; padding:10px; border-left:4px solid #f0c040}
    footer{margin-top:32px; font-size:0.9em; color:#555}
    ul{margin-top:0}
  </style>
</head>
<body>
  <h1>Chủ đề: IPC-Signal</h1>

  <p>Ta biết rằng các tiến trình là độc lập với nhau vì hệ điều hành cấp phát cho mỗi chương 1 địa chỉ riêng và với mỗi tiến trình thì chỉ có 1 PID nhưng có những chương trình lớn ví dụ như trình duyệt thì cần phải trao đổi thông tin vì phải hiển thị giao diện, xử lí tab, plugin nên cần phải trao đổi thông tin để biết ví dụ như nội dung web, trạng thái tab, vv… Điều này dẫn tới IPC</p>

  <p><strong>IPC</strong> viết tắt của <em>Inter-Process Communication</em>: là 1 phương thức giao tiếp giữa các tiến trình để một tiến trình thông báo cho các tiến trình khác về sự kiện không đồng bộ xảy ra như yêu cầu ngừng hoạt động hoặc hoàn tất nhiệm vụ.</p>

  <p>Linux cung cấp 1 số cơ chế giao tiếp:</p>
  <ul>
    <li>Signal Handling - Trao đổi bằng tín hiệu</li>
    <li>Pipe - Trao đổi bằng cơ chế đường ống</li>
    <li>Message Queues - Trao đổi thông qua hàng đợi tin nhắn</li>
    <li>Shared Memory - Trao đổi bằng phân đoạn nhớ chung</li>
    <li>Giao tiếp thông qua socket</li>
    <li>Giao tiếp đồng bộ dùng semaphore</li>
  </ul>

  <p>Ở trong bài viết này chúng ta sẽ tập trung vào cơ chế trao dổi bằng tín hiệu</p>

  <p>Mỗi tín hiệu có thể kết hợp hoặc có sẵn bộ xử lí tín hiệu (signal handler). Dùng lệnh <code>kill -l</code> để xem các tín hiệu của hệ thống</p>

  <p><img width="801" height="310" alt="Screenshot from 2025-10-22 13-12-09" src="https://github.com/user-attachments/assets/4b8db301-dd73-4a02-8183-0b8b7524da1d" /></p>

  <p>Signal thì có thể được gửi từ nhiều nguồn khác nhau nhưng phổ biến và quen thuộc nhất có lẽ là từ người dùng.</p>

  <p>Khi chạy 1 tiến trình thì người dùng muốn dừng tiến trình thì hay sử dụng tổ hợp phím Ctrl+C để ngắt tiến trình hoặc Ctrl + Z để tạm dừng tiến trình.</p>

  <p>Từ hệ điều hành Kernel, tín hiệu được gửi để báo sự kiện hoặc lỗi tiến trình ví dụ như segmentation fault do tín hiệu <code>SIGSEGV</code> tức là Signal Segmentation Violation.</p>

  <p>Từ tiến trình gửi đến tiến trình khác ví dụ để đồng bộ tiến trình cha - con.</p>

  <hr>

  <h2>Chủ đề 1.1: Tạo và chạy luồng cơ bản</h2>

  <p>Chương trình “<code>Problem1.c</code>” mô tả cách tín hiệu <code>SIGINT</code> kết hợp với một hàm xử lí tín hiệu tên là <code>sigint_handler</code> để xử lí tín hiệu. Tín hiệu <code>SIGINT</code> được viết tắt của từ Signal Interrupt tức là tín hiệu ngắt, thông thường được tạo bởi người dùng (tức là tổ hợp phím Ctrl + C). Dưới đây là kết quả của chương trình</p>

  <p><img width="696" height="224" alt="Screenshot from 2025-10-22 13-53-30" src="https://github.com/user-attachments/assets/b085f0a1-e359-404e-8219-bdbaca5bfe4a" /></p>

  <p>Thông qua câu <code>Caught signal 2</code> nếu bạn trả trong bảng được liệt kê bởi lệnh <code>kill -l</code> ở trên sẽ thấy rằng đó là signal interrupt và chương trình dừng sau 3 lần Ctrl+C.</p>

  <h3>Vấn đề 1: Nếu bỏ qua tín hiệu SIGINT, chuyện gì sẽ xảy ra sau khi nhấn Ctrl+C</h3>

  <p><strong>Trả lời:</strong> Nếu bỏ qua tín hiệu SIGINT, tức là trong hàm <code>int main()</code> bỏ dòng đăng kí signal cho SIGINT thì sau 1 lần Ctrl+ C sẽ dừng. Điều này nghĩa là hàm xử lí tín hiệu SIGINT không nhận được tín hiệu nên chương trình sẻ không trả về số lần lặp và hệ điều hành sẽ xử lí theo chế độ default là terminate.</p>

  <p><img width="694" height="92" alt="Screenshot from 2025-10-22 14-06-31" src="https://github.com/user-attachments/assets/315af02e-4bb4-41a0-9906-917daaa41686" /></p>

  <div class="note">
    <p><strong>Lưu ý:</strong> Chính việc terminate sẽ khiến nó không dọn dẹp tài nguyên nên trong chương trình ta nên đăng kí thêm 1 hàm là <code>SIGTSTP</code> nó sẽ dừng terminal đúng lúc.</p>
  </div>

  <hr>

  <h2>Chủ đề 1.2: Sử dụng tín hiệu SIGALRM để tạo timer</h2>

  <p>Đối với chương trình “<code>Problem2.c</code>” thì nó có công dụng là tạo 1 bộ đếm thời gian bằng cách sử dụng <code>SIGALRM</code>. Việc sử dụng tín hiệu này đi kèm với hàm <code>alarm()</code> để đặt bộ đếm.</p>

  <h3>Vấn đề 1: Điều gì sẽ xảy ra khi không gọi hàm <code>alarm(1)</code> trong hàm <code>void</code>. Dưới đây là kết quả</h3>

  <p><img width="854" height="136" alt="Screenshot from 2025-10-22 15-11-27" src="https://github.com/user-attachments/assets/52aed10a-3349-4d89-ba44-ab4c40cedcfc" /></p>

  <p>Nếu không đặt bộ đếm thì hàm <code>while</code> sẽ chỉ tăng biến đếm lên 1 đơn vị và đồng nghĩa là nó không lặp lại. Nếu đặt <code>alarm(1)</code> trong hàm <code>handler</code> thì vì <code>while(1)</code> nó sẽ quay trở lại để kích hoạt thêm 1s nữa và sẽ nhận được tín hiệu <code>SIGALRM</code> chứ không bị dừng ở giây thứ nhất nữa.</p>

  <h3>Vấn đề 2: Ngược lại với vấn đề 1 vậy nếu không đặt bộ đếm ở hàm <code>main</code> thì chuyện gì sẽ xảy ra ?</h3>

  <p><strong>Đáp án:</strong> Nếu không đặt bộ đếm thì hàm <code>while</code> sẽ chỉ tăng biến đếm lên 1 đơn vị và đồng nghĩa là nó không lặp lại. Nếu đặt <code>alarm(1)</code> trong hàm <code>handler</code> thì vì <code>while(1)</code> nó sẽ quay trở lại để kích hoạt thêm 1s nữa và sẽ nhận được tín hiệu <code>SIGALRM</code>
  chứ không bị dừng ở giây thứ nhất nữa.</p>

  <h3>Vấn đề 2: Ngược lại với vấn đề 1 vậy nếu không đặt bộ đếm ở hàm main thì chuyện gì sẽ xảy ra ?</h3>

  <p><strong>Đáp án:</strong></p>

  <p><img width="859" height="64" alt="Screenshot from 2025-10-22 15-18-20" src="https://github.com/user-attachments/assets/17d90b5d-a34d-4435-9c83-c668f2a466c3" /></p>

  <p>Vì không có bộ đếm nên sẽ không có tín hiệu nào cả điều này khiến việc chờ đợi tín hiệu sẽ kéo dài mãi vì vòng lặp <code>while(1) pause()</code>. hãy sử dụng html để làm thành 1 bản README hoàn thiện nhưng không được phép thay đổi nội dung</p>
Chủ đề 1.3: Tạo giao tiếp giữa các process bằng tín hiệu
Mô tả:

Chương trình Problem3.c tạo 2 tiến trình cha và con và sử dụng tín hiệu SIGUSR1 để giao tiếp giữa chúng.

Để có thể hiểu rõ về tiến trình thì bạn nên tìm hiểu về tiến trình và cách hoạt động của nó. Chương trình này viết để thỏa mãn các yêu cầu sau:

Dùng fork() để tạo process con từ process cha.

Process cha sẽ gửi tín hiệu SIGUSR1 cho process con mỗi 2 giây.

Khi nhận tín hiệu SIGUSR1, process con sẽ in ra thông báo "Received signal from parent".

Dừng chương trình sau khi đã gửi tín hiệu 5 lần.

Vấn đề:

Tại sao lại phải dùng kill() để gửi tín hiệu trong bài này?

Đáp án:
Chúng ta cần sử dụng kill() để gửi tín hiệu vì sau khi fork(), tiến trình cha và con có các PID riêng biệt và không chia sẻ cùng không gian địa chỉ.

Hàm kill() là một system call (vì để giao tiếp với tiến trình khác thì user phải thông qua kernel) dùng để gửi một tín hiệu (như SIGUSR1) đến một tiến trình khác trong hệ thống thông qua PID của tiến trình đó.

Do đó, để tiến trình cha có thể gửi tín hiệu SIGUSR1 đến tiến trình con, việc sử dụng kill() là hoàn toàn hợp lý và cần thiết.

Chủ đề 1.4: Chương trình chờ tín hiệu kết hợp chờ người dùng nhập liệu
Mô tả:

Chương trình có thể nhận tín hiệu đồng thời cho phép người dùng nhập liệu từ bàn phím.

Yêu cầu:

Viết một chương trình có thể nhận tín hiệu đồng thời cho phép người dùng nhập liệu từ bàn phím.

Dùng select() hoặc poll() để cho phép chương trình xử lý tín hiệu mà không làm gián đoạn khi nhận dữ liệu nhập từ bàn phím.

Khi nhận tín hiệu SIGINT, in ra thông báo "SIGINT received."

Nếu nhận tín hiệu SIGTERM, thoát chương trình.

Khi người dùng nhập liệu và nhấn Enter, in nội dung ra màn hình console.

Ghi chú:

Trong yêu cầu này, phần khó nhất có lẽ là hiểu select() là gì và sử dụng như thế nào.

Bạn có thể tham khảo bài viết sau:
 Giới thiệu về I/O Multiplexing và hàm select() – vimentor.com

Hoặc tìm đọc cuốn “The Linux Programming Interface”, sau đó tìm đến trang 1331 để hiểu hơn về system().


  <footer>
    <p>Generated README — nội dung do người dùng cung cấp. Không thay đổi nội dung gốc.</p>
  </footer>
</body>
</html>







