## Nintendo Switch Netease cloud
1. crack ns
2. setup dev tools
```
sudo dkp-pacman -S switch-dev

sudo dkp-pacman -S switch-curl

sudo dkp-pacman -S devkitA64 switch-tools switch-curl switch-bzip2 switch-freetype switch-libjpeg-turbo switch-sdl2 switch-sdl2_gfx switch-sdl2_image switch-sdl2_ttf switch-zlib switch-libpng

sudo dkp-pacman -S switch-libjson-c
```
### NetLoad
1. open the hbmenu and press the Y button.
2. ```make load```
### How to
- [x] login via scan qrcode.
  - [x] fetch qr base64
  - [x] decode base64 and write to file
  - [x] render qrcode
  - [ ] call check and save cookie

- [ ] fetch playlist via id.
- [ ] if song file not download, download it.
- [ ] play song via mpg123.

### Features
- play or pause.
- use cache when lose network

### UI
- pure sdl2

### Debug

```bash
https://netease-cloud-music-api-theta-steel.vercel.app/user/playlist?uid=68277534
```


```bash
https://netease-cloud-music-api-theta-steel.vercel.app/playlist/detail?id=72614739
```

```bash
https://netease-cloud-music-api-theta-steel.vercel.app/song/url?id=1915875397
```