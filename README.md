![title](https://user-images.githubusercontent.com/106372669/236672162-ec0173b3-d7e6-4a49-a81c-0baca967a784.gif)

## Giới thiệu
* Họ và tên: Nguyễn Đức Hùng
* MSSV: 22021109
* Trường Đại học Công nghệ, Đại học Quốc gia Hà Nội (UET-VNU)

Dark Knight là một game 2D RPG được viết bằng ngôn ngữ lập trình C++ và sử dụng SDL 2 để đồ họa. Trong game, người chơi sẽ nhập vai vào một hiệp sĩ tối thượng và chinh phục các thử thách để cứu giúp vương quốc khỏi sự tàn phá của các thế lực hắc ám.

Trong game Dark Knight, người chơi sẽ được trải nghiệm các tính năng như:

* Hệ thống chiến đấu: Với nhiều chiêu thức và kỹ năng khác nhau để đánh bại các kẻ thù.
* Hệ thống nâng cấp: Cho phép người chơi nâng cấp sức mạnh của nhân vật để có thể đối phó với những tình huống khó khăn hơn trong game.
* Hệ thống câu chuyện: Với các nhiệm vụ và những câu chuyện kích thích để giúp người chơi hiểu rõ hơn về thế giới và nhân vật của game.

Với đồ họa 2D tuyệt đẹp và âm thanh sống động, Dark Knight hứa hẹn sẽ mang đến cho người chơi những trải nghiệm thú vị và đầy cảm xúc.

## Cách cài đặt game
* Bước 1: Clone game về máy bằng lệnh git clone hoặc tải về [tại đây](https://github.com/hudzg/knight/archive/refs/heads/main.zip)
* Bước 2: Giải nén và click vào `main.exe` để bắt đầu chơi game thôi nàoooo!!!!!!!

Nếu muốn biên dịch game bằng Makefile thì thực hiện các bước sau:
* Bước 1: Mở cmd và cd đến thư mục của game
* Bước 2: Dùng lệnh sau để bắt đầu Makefile: 
```bash
mingw32-make
```
* Bước 3: Dùng lệnh sau để khởi động game:
```bash
main
```
## Chi tiết về game
### Gameplay
Xem gameplay [tại đây](https://youtu.be/C2Q8dkf2o7w)
### Nhân vật
Là một hiệp sĩ tối thượng với bộ kĩ năng cực kì mạnh mẽ

![knight](https://user-images.githubusercontent.com/106372669/236669112-f0188ad7-90e8-44da-bc9e-c9622da7d66e.gif)

Tấn công

![attack3](https://user-images.githubusercontent.com/106372669/236669287-3c047eb0-2119-4071-ac6a-c4b648fd2f84.gif)

Kĩ năng

![Sprite-0001-export](https://user-images.githubusercontent.com/106372669/236670667-3622f35a-42e4-45c2-bd36-07685a4ad31b.gif)
### Quái vật
Skeleton

![Skeleton Attack-export](https://user-images.githubusercontent.com/106372669/236670771-f50bc349-1c49-4822-8298-066f80e7fd99.gif)

Boss

![boss](https://user-images.githubusercontent.com/106372669/236670883-685ff1ee-752e-4e1d-a483-fcde5eaac788.gif)

### Bẫy

![trap](https://user-images.githubusercontent.com/106372669/236671042-99685a0e-8787-40be-8ef2-bf2f98aad0f0.png)

### Cửa
Mỗi khi mở cửa nhân vật sẽ được tăng sức mạnh

![door-export](https://user-images.githubusercontent.com/106372669/236671232-82d2a270-9efd-46ec-9241-6359a4a9bb90.png)

### Battle
* Tìm đường thoát và tiêu diệt quái vật.
* Giết boss và cứu lấy thế giới.

### Một số hình ảnh trong game
![Screenshot 2023-05-07 173340](https://user-images.githubusercontent.com/106372669/236672645-0fcd1303-c462-4d18-a670-79bc4abf9048.png)
![Screenshot 2023-05-07 173402](https://user-images.githubusercontent.com/106372669/236672652-7d16a364-2277-44b6-8db7-81751a0ec491.png)
![Screenshot 2023-05-07 173417](https://user-images.githubusercontent.com/106372669/236672654-7cfa0f1d-e402-47ec-bc7d-cc1bfae65cad.png)
![Screenshot 2023-05-07 173525](https://user-images.githubusercontent.com/106372669/236672656-7f4c07aa-2027-47da-9d06-cd6da026d879.png)
![Screenshot 2023-05-07 173555](https://user-images.githubusercontent.com/106372669/236672659-b4854c5e-7b10-41d5-8b50-a39c8d45e44a.png)
![Screenshot 2023-05-07 173841](https://user-images.githubusercontent.com/106372669/236672661-e3f8e67b-d2dc-4ded-be4f-8d22a9e0b9f0.png)


## Các kĩ thuật lập trình đã sử dụng
* Struct và Class
* Con trỏ
* Vector, set, map,...
* Xâu
* Nhập/xuất file
* Mảng tĩnh
* Sắp xếp
* Lập trình hướng đối tượng
* Xử lí va chạm
* Kĩ thuật Tile Map

## Kết luận
* Học SDL tại: https://lazyfoo.net/tutorials/SDL/
* Hình ảnh và âm thanh được tham khảo từ nhiều nguồn khác nhau và được chỉnh sửa để phù hợp với game, nhưng đa số được tham khảo từ: https://itch.io/game-assets/free
* Biết cách làm nên một game
* Biết cách debug những bug "ẩn" rẩt lớn (thậm chí mất đến vài ngày)
* Sử dụng tốt ngôn ngữ c++ kết hợp với thư viện SDL để làm game
* Sử dụng phần mềm chỉnh sửa ảnh pixel: Aseprite
* Biết cách chia nhỏ một project lớn thành những file nhỏ, những file nhỏ thành những class nhỏ hơn để thuận tiện trong quá trình code hay debug

