#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>

#include "fetch.h"
#include "base64.h"

#define RESPONSE_BODY_SIZE 128
#define BASE_URL "https://netease-cloud-music-api-theta-steel.vercel.app"
static char *qr_res = NULL;
const char *fetch_err = NULL;
size_t create_qr(void *ptr, size_t size, size_t nmemb, void *stream)
{
    // printf(">>%s", ptr);
    char *response_body = (char *)ptr;
    char *s;
    if (qr_res == NULL){
        s = malloc(strlen(response_body) + 1);
        strcpy(s, response_body);
    } else {
        s = malloc(strlen(qr_res) + strlen(response_body) + 1);
        strcpy(s, qr_res);
        strcat(s, response_body);
    } 
    qr_res = s;
    // free(s);
    struct json_object *parsed_json;
    parsed_json = json_tokener_parse(qr_res);
    struct json_object *data;
    json_object_object_get_ex(parsed_json, "data", &data);
    struct json_object *qrimg;
    json_object_object_get_ex(data, "qrimg", &qrimg);
    const char *str_qrimg = json_object_get_string(qrimg);
    // printf("qrimg: %s\n", str_qrimg);
    
    if (str_qrimg != NULL) {
        char *result = NULL;
        result = replaceWord(str_qrimg, "data:image/png;base64,", "");
        printf("decode qrimg: %s\n", result);
        char *name = "/song/qr.png";
        FILE *file = fopen(name, "wb");
        size_t output_length;
        unsigned char *png_data = base64_decode(result, strlen(result), &output_length);
        fwrite(png_data, 1, output_length, file);
        fclose(file);
    }
    free(parsed_json);
    free(data);
    free(qrimg);
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
    // printf(">>%s", ptr);
    char *response_body = (char *)ptr;
    struct json_object *parsed_json;
    parsed_json = json_tokener_parse(response_body);
    struct json_object *data;
    json_object_object_get_ex(parsed_json, "data", &data);
    struct json_object *key;
    json_object_object_get_ex(data, "unikey", &key);
    const char *str_key = json_object_get_string(key);
    printf("key: %s\n", str_key);

    char *s1 = BASE_URL;
    char *s2 = "/login/qr/create?key=";
    const char *s3 = str_key;
    char *s4 = "&qrimg=true";
    char *s = malloc(strlen(s1) + strlen(s2) + strlen(s3) + strlen(s4) + 1); // +1 for the null-terminator
    strcpy(s, s1);
    strcat(s, s2);
    strcat(s, s3);
    strcat(s, s4);
    request(s, create_qr);
    free(parsed_json);
    free(s);
    free(data);
    free(key);

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

    // char *jsonObj = "{ \"name\" : \"Pedro\" , \"age\" : \"22\" }";

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
        fetch_err = err;
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
    // get key
    char *s1 = BASE_URL;
    char *s2 = "/login/qr/key";
    char *s = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    strcpy(s, s1);
    strcat(s, s2);

    request(s, get_key);
    free(s);
    // create qr
    // get cookie, call check api
    // save cookie
}

void fetch_songs_by_playlist(const char *name)
{
}
