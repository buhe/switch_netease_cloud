#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>

#define RESPONSE_BODY_SIZE 128

static size_t get_key(void *ptr, size_t size, size_t nmemb, void *stream)
{
    // printf(">>%s", ptr);
    char *response_body = (char *)ptr;
    struct json_object *parsed_json;
    parsed_json = json_tokener_parse(response_body);
    struct json_object *data;
    json_object_object_get_ex(parsed_json, "data", &data);
    struct json_object *key;
    json_object_object_get_ex(data, "unikey", &key);
    printf("key: %s\n", json_object_get_string(key));
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
    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
        printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    curl_easy_cleanup(curl);
}
char *get_url(char *id)
{
    return id;
}

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
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
void fetch_songs_by_playlist(const char *name)
{

    printf("curl init\n");
    curl_global_init(CURL_GLOBAL_DEFAULT);

    request("https://netease-cloud-music-api-theta-steel.vercel.app/login/qr/key", get_key);

    // fetch_song("https://raw.githubusercontent.com/Binaryify/NeteaseCloudMusicApi/master/module_example/test.mp3");

    curl_global_cleanup();
}