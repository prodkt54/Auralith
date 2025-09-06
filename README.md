# 🎹 Auralith

**Auralith** là một phần mềm **synthesizer** tối giản, viết bằng **C++** và sử dụng **Raylib** cho audio/GUI.  
Dự án nhằm mục đích tạo một nhạc cụ ảo nhỏ gọn để học và thử nghiệm, sau này có thể mở rộng thành VSTi.

---

## ✨ Tính năng hiện tại
- Oscillator cơ bản: **sine / square / saw**  
- **3xOSC**: trộn tối đa **3 oscillator** để tạo âm thanh phong phú hơn  
- Hệ thống **real-time audio callback**  
- Code ngắn gọn, dễ đọc và dễ mở rộng  

> ⚠️ Hiện tại **chưa có ADSR envelope** và **chưa hỗ trợ noise oscillator**.

---
## 📺Demo video 

https://github.com/user-attachments/assets/bb859657-0d73-41cb-953e-27fa2de73e7a





## 🛠️ Build & Run
### Yêu cầu
- Windows  
- MSVC (Visual Studio 2019/2022)  
- [Raylib](https://www.raylib.com/) (cài bằng vcpkg hoặc build thủ công)

### Build
1. Clone repo:
   ```bash
   git clone https://github.com/prodkt54/Auralith.git
   cd Auralith
