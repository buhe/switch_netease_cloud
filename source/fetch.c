#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <json-c/json.h>

#include "fetch.h"
#include "base64.h"
#include "ui.h"  
#include "account.h" 

char *BASE_URL = "https://netease-cloud-music-api-theta-steel.vercel.app";
const char *qr_msg = NULL;
const char *check_msg = NULL;

char *g_key = NULL;

char *songs_res = NULL;
char *qr_res = NULL;
char *cookie_res = NULL;
char *url_res = NULL;

int song_len;
Song *g_songs = NULL;

size_t save_cookie(void *ptr, size_t size, size_t nmemb, void *stream)
{
    char *response_body = (char *)ptr;
    if (cookie_res)
    {
        char *result = malloc(strlen(cookie_res) + strlen(response_body) + 1);
        strcpy(result, cookie_res);
        strcat(result, response_body);
        free(cookie_res);
        cookie_res = result;
    }
    else
    {
        cookie_res = malloc(strlen(response_body) + 1);
        strcpy(cookie_res, response_body);
    }
    struct json_object *parsed_json;
    parsed_json = json_tokener_parse(cookie_res);
    if(parsed_json != NULL) {
        if(cookie_res){
            free(cookie_res);
            cookie_res = NULL;
        }
        
        struct json_object *code;
        json_object_object_get_ex(parsed_json, "code", &code);
        int int_code = json_object_get_int(code);
        if (int_code != 0)
        {

            if (int_code == 800)
            {
                check_msg = "qrcode not exist or timeout";
            }
            if (int_code == 801)
            {
                check_msg = "wait scan..";
            }
            if (int_code == 803)
            {
                check_msg = "login sucessful";
                dispose_login_qr();
                qr_msg = "enter X fetch songs, enter - logout";
                login2();
                free(g_key);
            }
        }
        json_object_put(parsed_json);
    }
    return size * nmemb;
}

size_t create_qr(void *ptr, size_t size, size_t nmemb, void *stream)
{
    char *response_body = (char *)ptr;
    if (qr_res)
    {
        char *r = malloc(strlen(qr_res) + strlen(response_body) + 1);
        strcpy(r, qr_res);
        strcat(r, response_body);
        free(qr_res);
        qr_res = r;
    }
    else
    {
        qr_res = malloc(strlen(response_body) + 1);
        strcpy(qr_res, response_body);
    }
    struct json_object *parsed_json;
    parsed_json = json_tokener_parse(qr_res);
    if(parsed_json){
        if(qr_res){
            free(qr_res);
            qr_res = NULL;
        }
        struct json_object *data;
        json_object_object_get_ex(parsed_json, "data", &data);
        struct json_object *qrimg;
        json_object_object_get_ex(data, "qrimg", &qrimg);
        const char *str_qrimg = json_object_get_string(qrimg);
        char *result = NULL;
        result = replaceWord(str_qrimg, "data:image/png;base64,", "");
        printf("decode qrimg: %s\n", result);
        FILE *file = fopen(QR, "wb");
        if (file != NULL)
        {
            printf("create qr file\n");
            size_t output_length;
            unsigned char *png_data = base64_decode(result, strlen(result), &output_length);
            fwrite(png_data, 1, output_length, file);
            fclose(file);
            show_login_qr();
            qr_msg = "Please scan qrcode..continue enter Y check";
        }
        json_object_put(parsed_json);
    }
    return size * nmemb;
}

size_t get_key(void *ptr, size_t size, size_t nmemb, void *stream)
{
    char *response_body = (char *)ptr;
    struct json_object *parsed_json;
    parsed_json = json_tokener_parse(response_body);
    struct json_object *data;
    json_object_object_get_ex(parsed_json, "data", &data);
    struct json_object *key;
    json_object_object_get_ex(data, "unikey", &key);
    const char *str_key = json_object_get_string(key);

    g_key = malloc(strlen(str_key) + 1);
    strcpy(g_key, str_key);
    printf("key:%s\n", g_key);
    json_object_put(parsed_json);

    char s[S_STR_SIZE] = {0};
    snprintf(s, sizeof(s), "%s%s%s%s", BASE_URL, "/login/qr/create?key=", str_key, "&qrimg=true");
    request(s, create_qr, 1);
    return size * nmemb;
}
void request(char *url, size_t (*next)(void *ptr, size_t size, size_t nmemb, void *stream),int write_cookie)
{
    printf("url: %s\n", url);
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl == NULL)
    {
        return ;
    }

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "charset: utf-8");

    curl_easy_setopt(curl, CURLOPT_URL, url);

    // curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    // curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_object_to_json_string(payload));
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libnx curl example/1.0");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, next);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1l);

    // curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    // curl_easy_setopt(curl, CURLOPT_BUFFERSIZE, 120000L);
    curl_easy_setopt(curl, CURLOPT_COOKIEFILE, R_COOKIE);
    curl_easy_setopt(curl, CURLOPT_COOKIEJAR, W_COOKIE);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        const char *err = curl_easy_strerror(res);
        printf("curl_easy_perform() failed: %s\n", err);
    }
        
    curl_easy_cleanup(curl);
    if (write_cookie) {
        printf("write cookie\n");
        remove(R_COOKIE);
        if (rename(W_COOKIE, R_COOKIE) != 0)
        {
            printf("rename file not successful,cause:%d\n", errno);
        }
    } else {
        printf("drop cookie\n");
        remove(W_COOKIE);
    }
}


static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
    // stream is user defined file
    size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
    return written;
}

// https://raw.githubusercontent.com/Binaryify/NeteaseCloudMusicApi/master/module_example/test.mp3
void fetch_song(int id, const char *url)
{
    // download song
    char file_name[S_STR_SIZE] = {0};
    snprintf(file_name, sizeof(file_name), "%d%s", id, ".mp3");
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1l);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libnx curl example/1.0");
        /* send all data to this function  */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        // Add any other options here.
        FILE *file = fopen(file_name, "wb");
        if (file)
        {

            /* write the page body to this file handle */
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

            res = curl_easy_perform(curl);

            /* close the header file */
            fclose(file);
            if (res != CURLE_OK)
                printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }
}

void show_qr() {
    request("https://netease-cloud-music-api-theta-steel.vercel.app/login/qr/key", get_key, 1);
}

void check() {
    char s[S_STR_SIZE] = {0};
    snprintf(s, sizeof(s), "%s%s%s", BASE_URL, "/login/qr/check?key=", g_key);
    request(s, save_cookie, 1);
}

static size_t get_songs(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    char *response_body = (char *)contents;
    if(songs_res){
        char *result = malloc(strlen(songs_res) + strlen(response_body) + 1);
        strcpy(result, songs_res);
        strcat(result, response_body);
        free(songs_res);
        songs_res = result;
    }else{
        songs_res = malloc(strlen(response_body) + 1);
        strcpy(songs_res, response_body);
    }
    struct json_object *parsed_json;
    parsed_json = json_tokener_parse(songs_res);

    if (parsed_json)
    {
        if(songs_res){
            free(songs_res);
            songs_res = NULL;
        }
        int arraylen, i;
        struct json_object *playlist;
        json_object_object_get_ex(parsed_json, "playlist", &playlist);
        struct json_object *tracks;
        json_object_object_get_ex(playlist, "tracks", &tracks);
        struct json_object *song;
        struct json_object *id;
        struct json_object *name;
        arraylen = json_object_array_length(tracks);
        Song *songs = malloc(sizeof(Song) * arraylen);
        printf("songs len:%d\n", arraylen);
        for (i = 0; i < arraylen; i++)
        {
            song = json_object_array_get_idx(tracks, i);
            Song s = {0};
            json_object_object_get_ex(song, "name", &name);
            const char *str_name = json_object_get_string(name);

            json_object_object_get_ex(song, "id", &id);
            const int int_id = json_object_get_int(id);

            s.id = int_id;
            strcpy(s.name, str_name);
            songs[i] = s;
            printf("id: %d\n", songs[i].id);
            printf("name: %s\n", songs[i].name);
        }
        song_len = arraylen;
        g_songs = songs;
        json_object_put(parsed_json);
    }
    return realsize;
}

void fetch_songs_by_playlist(const char *id)
{
    // https://netease-cloud-music-api-theta-steel.vercel.app/playlist/detail?id=72614739
    char s[S_STR_SIZE] = {0};
    snprintf(s, sizeof(s), "%s%s%s", BASE_URL, "/playlist/detail?id=", id);
    request(s, get_songs, 0);
}
static size_t get_url(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    char *response_body = (char *)contents;
    if (url_res)
    {
        char *result = malloc(strlen(url_res) + strlen(response_body) + 1);
        strcpy(result, url_res);
        strcat(result, response_body);
        free(url_res);
        url_res = result;
    }
    else
    {
        url_res = malloc(strlen(response_body) + 1);
        strcpy(url_res, response_body);
    }
    struct json_object *parsed_json;
    parsed_json = json_tokener_parse(url_res);
    if (parsed_json)
    {
        if(url_res){
            free(url_res);
            url_res = NULL;
        }
        struct json_object *data;
        json_object_object_get_ex(parsed_json, "data", &data);
        struct json_object *first;
        first = json_object_array_get_idx(data, 0);
        struct json_object *url;
        json_object_object_get_ex(first, "url", &url);
        const char *str_url = json_object_get_string(url);
        printf("get url:%s\n", str_url);
        struct json_object *id;
        json_object_object_get_ex(first, "id", &id);
        int int_id = json_object_get_int(id);
        fetch_song(int_id, str_url);
        printf("download!!!!!!\n");
        json_object_put(parsed_json);
    }
    return realsize;
}

void request_song(const Song *song) {
    char url[S_STR_SIZE] = {0};
    snprintf(url, sizeof(url), "%s%s%d", BASE_URL, "/song/url?id=", song->id);
    request(url, get_url, 0);
}