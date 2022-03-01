#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <json-c/json.h>

#include "fetch.h"
#include "base64.h"
#include "ui.h"

#define STR_SIZE 10000
#define RESPONSE_BODY_SIZE 128
char *BASE_URL = "https://netease-cloud-music-api-theta-steel.vercel.app";
static char qr_res[STR_SIZE] = {0};
static char cookie_res[STR_SIZE] = {0};
const char *qr_msg = NULL;
const char *check_msg = NULL;
const char *g_key = NULL;
const char *g_cookie = NULL;
size_t save_cookie(void *ptr, size_t size, size_t nmemb, void *stream)
{
    char *response_body = (char *)ptr;
    
    // check_msg = response_body;

    // char *s;
    // if (cookie_res == NULL)
    // {
    //     s = malloc(strlen(response_body) + 1);
    //     strcpy(s, response_body);
    //     // free(response_body);
    // }
    // else
    // {
    //     s = malloc(strlen(qr_res) + strlen(response_body) + 1);
    //     strcpy(s, cookie_res);
    //     strcat(s, response_body);
    //     // free(cookie_res);
    //     // free(response_body);
    // }
    // cookie_res = s;
    snprintf(cookie_res, sizeof(cookie_res), "%s%s", cookie_res, response_body);
    // fetch_err = cookie_res;
    struct json_object *parsed_json;
    parsed_json = json_tokener_parse(cookie_res);
    if(parsed_json != NULL) {
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
                struct json_object *cookie;
                json_object_object_get_ex(parsed_json, "cookie", &cookie);
                char *str_cookie = json_object_get_string(cookie);
                g_cookie = str_cookie;
                char *name = "/song/cookie.txt";
                FILE *file = fopen(name, "w");
                if (file != NULL)
                {
                    fprintf(file, str_cookie);
                    fclose(file);
                }
                // save cookie
            }
        }
    }
    uint32_t response_body_len = strlen(response_body);
    uint32_t len = size * nmemb;
    if (len > RESPONSE_BODY_SIZE - response_body_len - 1)
    {
        len = RESPONSE_BODY_SIZE - response_body_len - 1;
    }
    memcpy(response_body + response_body_len, ptr, len);
    return size * nmemb;
}
size_t create_qr(void *ptr, size_t size, size_t nmemb, void *stream)
{
    char *response_body = (char *)ptr;
    // char *tmp = malloc(strlen(qr_res)); 
    // strcpy(qr_res);
    snprintf(qr_res, sizeof(qr_res), "%s%s", qr_res, response_body);
    struct json_object *parsed_json;
    parsed_json = json_tokener_parse(qr_res);
    struct json_object *data;
    json_object_object_get_ex(parsed_json, "data", &data);
    struct json_object *qrimg;
    json_object_object_get_ex(data, "qrimg", &qrimg);
    const char *str_qrimg = json_object_get_string(qrimg);
    
    if (str_qrimg != NULL) {
        char *result = NULL;
        result = replaceWord(str_qrimg, "data:image/png;base64,", "");
        printf("decode qrimg: %s\n", result);
        char *name = "/song/qr.png";
        FILE *file = fopen(name, "wb");
        if(file != NULL) {
            size_t output_length;
            unsigned char *png_data = base64_decode(result, strlen(result), &output_length);
            fwrite(png_data, 1, output_length, file);
            fclose(file);
            show_login_qr();
            qr_msg = "Please scan qrcode..continue enter Y check";
        }
        
    }
    uint32_t response_body_len = strlen(response_body);
    uint32_t len = size * nmemb;
    if (len > RESPONSE_BODY_SIZE - response_body_len - 1)
    {
        len = RESPONSE_BODY_SIZE - response_body_len - 1;
    }
    memcpy(response_body + response_body_len, ptr, len);
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

    g_key = str_key;

    char s[STR_SIZE] = {0};
    snprintf(s, sizeof(s), "%s%s%s%s", BASE_URL, "/login/qr/create?key=", str_key, "&qrimg=true");
    request(s, create_qr);

    uint32_t response_body_len = strlen(response_body);
    uint32_t len = size * nmemb;
    if (len > RESPONSE_BODY_SIZE - response_body_len - 1)
    {
        len = RESPONSE_BODY_SIZE - response_body_len - 1;
    }
    memcpy(response_body + response_body_len, ptr, len);
    return size * nmemb;
}
void request(char *url, size_t (*next)(void *ptr, size_t size, size_t nmemb, void *stream))
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
    curl_easy_setopt(curl, CURLOPT_BUFFERSIZE, 120000L);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        const char *err = curl_easy_strerror(res);
        printf("curl_easy_perform() failed: %s\n", err);
    }
        
    curl_easy_cleanup(curl);
}
char *get_url(char *id)
{
    return id;
}

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
    // stream is user defined file
    size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
    return written;
}

// https://raw.githubusercontent.com/Binaryify/NeteaseCloudMusicApi/master/module_example/test.mp3
void fetch_song(char *url)
{
    // download song
    char *name = "/song/test.mp3";
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libnx curl example/1.0");
        /* send all data to this function  */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        // Add any other options here.
        FILE *file = fopen(name, "wb");
        if (file)
        {

            /* write the page body to this file handle */
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
            printf("curl_easy_perform\n");

            res = curl_easy_perform(curl);

            /* close the header file */
            fclose(file);
            if (res != CURLE_OK)
                printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        printf("cleanup\n");
        curl_easy_cleanup(curl);
    }
}

void login() {
    request("https://netease-cloud-music-api-theta-steel.vercel.app/login/qr/key", get_key);
}

void check() {
    char s[STR_SIZE] = {0};
    snprintf(s, sizeof(s), "%s%s%s", BASE_URL, "/login/qr/check?key=", g_key);
    request(s, save_cookie);
}

void fetch_songs_by_playlist(const char *name)
{
}
