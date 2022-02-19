#include <curl/curl.h>

void fetch_song(const char *name)
{
    CURL *curl;
        CURLcode res;

        printf("curl init\n");
        curl_global_init(CURL_GLOBAL_DEFAULT);

        curl = curl_easy_init();
        if (curl)
        {
            curl_easy_setopt(curl, CURLOPT_URL, "https://baidu.com/");
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "libnx curl example/1.0");
            // Add any other options here.

            printf("curl_easy_perform\n");
            consoleUpdate(NULL);

            res = curl_easy_perform(curl);
            if (res != CURLE_OK)
                printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

            // curl_easy_recv(res);
            // In an actual app you should return an error on failure, following cleanup.

            printf("cleanup\n");
            curl_easy_cleanup(curl);
        }

        curl_global_cleanup();
}