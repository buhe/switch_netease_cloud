#include <curl/curl.h>
#include <json-c/json.h>
char* get_url(char *id) {
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

    fetch_song("https://raw.githubusercontent.com/Binaryify/NeteaseCloudMusicApi/master/module_example/test.mp3");

    curl_global_cleanup();
}