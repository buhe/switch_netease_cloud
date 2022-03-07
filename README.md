## Nintendo Switch Netease cloud
1. crack ns
2. setup dev tools
    ```
    sudo dkp-pacman -S switch-dev
    
    sudo dkp-pacman -S switch-curl
    
    sudo dkp-pacman -S devkitA64 switch-tools switch-curl switch-bzip2 switch-freetype switch-libjpeg-turbo switch-sdl2 switch-sdl2_gfx switch-sdl2_image switch-sdl2_ttf switch-zlib switch-libpng
    
    sudo dkp-pacman -S switch-libjson-c
    ```

### Access full RAM
- Hold r button click any app access full RAM.
### NetLoad
1. open the hbmenu and press the Y button.
2. ```make load```
### How to
- [x] login via scan qrcode.
  - [x] fetch qr base64
  - [x] decode base64 and write to file
  - [x] render qrcode
  - [x] call check and save cookie
  - [x] is login
  - [x] logout

- [x] fetch playlist via id.
  - [x] with cookie
  - [x] song entry
  - [x] implement list with scroll ui
  - [ ] use cache when network lose
- [x] download song. 
  - [x] get song url
  - [x] download and save the song
  - [ ] if song file not download
  - [ ] progress ui
- [x] play song via mpg123.

### Features
- play or pause.
- use cache when lose network

### UI
- pure sdl2

### Design
https://www.figma.com/file/BMqb2WCmluOGYmek0blxfn/ns?node-id=0%3A1

### ScreenShot

![WechatIMG7611](https://tva1.sinaimg.cn/large/e6c9d24egy1h015i8n3zqj20u01hc0w1.jpg)



![WechatIMG7611](https://tva1.sinaimg.cn/large/e6c9d24egy1h015hrtjvtj20u01hc0wf.jpg)



