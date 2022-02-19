#include <MainApplication.hpp>

/*
// If you would like to initialize and finalize stuff before or after Plutonium, you can use libnx's userAppInit/userAppExit

extern "C" void userAppInit() {
    // Initialize stuff
}

extern "C" void userAppExit() {
    // Cleanup/finalize stuff
}
*/
#include <curl/curl.h>

// This example shows how to use libcurl. For more examples, see the official examples: https://curl.haxx.se/libcurl/c/example.html

void network_request(void)
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

// Main entrypoint
int main() {
    // First create our renderer, where one can customize SDL or other stuff's initialization.
    auto renderer_opts = pu::ui::render::RendererInitOptions(SDL_INIT_EVERYTHING, pu::ui::render::RendererHardwareFlags);
    renderer_opts.UseImage(pu::ui::render::IMGAllFlags);
    renderer_opts.UseAudio(pu::ui::render::MixerAllFlags);
    renderer_opts.UseTTF();
    auto renderer = pu::ui::render::Renderer::New(renderer_opts);

    // Create our main application from the renderer
    auto main = MainApplication::New(renderer);

    // Prepare out application. This MUST be called or Show() will exit and nothing will be rendered.
    main->Prepare();

    // Show -> start rendering in an "infinite" loop
    // If wou would like to show with a "fade in" from black-screen to the UI, use instead ->ShowWithFadeIn();
    main->Show();

    // Exit homebrew (Plutonium will handle all disposing of UI and renderer/application, don't worry!

    return 0;
}