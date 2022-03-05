// #define STR_SIZE 100000
#define S_STR_SIZE 10000

#define QR "qrcode.png"
#define W_COOKIE "w_cookies.txt"
#define R_COOKIE "r_cookies.txt"

#include "song.h"

void fetch_songs_by_playlist(const char *name);

void show_qr();

void check();

size_t save_cookie(void *ptr, size_t size, size_t nmemb, void *stream);

size_t create_qr(void *ptr, size_t size, size_t nmemb, void *stream);

size_t get_key(void *ptr, size_t size, size_t nmemb, void *stream);

void request(char *url, size_t (*next)(void *ptr, size_t size, size_t nmemb, void *stream), int write_cookie);

void request_song(const Song *song);

void fetch_song(int id, const char *url);
