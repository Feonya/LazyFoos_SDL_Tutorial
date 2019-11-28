#include <iostream>
#include <sstream>
#include <string>

#include "timer.h"
#include "texture.h"

int g_screenWidth  = 600;
int g_screenHeight = 480;

SDL_Window*   g_window        = NULL;
SDL_Renderer* g_renderer      = NULL;
TTF_Font*     g_font          = NULL;

bool init();
bool loadMedia();
void close();

int main(int argc, char* argv[])
{
    // The main loop flag.
    bool quit = false;

    if (init() == false)
    {
        quit = true;
        std::cout << "Initialize SDL Error: " << SDL_GetError() << "\n";
    }

    if (loadMedia() == false)
    {
        quit = true;
        std::cout << "Load media Error: " << SDL_GetError() << "\n";
    }

    // SDL event handler.
    SDL_Event e;

    // The fps text color;
    SDL_Color fpsColor = {0, 0, 0, 255};
    // The text stream in memory.
    std::stringstream fpsText;
    // The fps timer.
    Timer fpsTimer;
    // The fps timer texture.
    Texture fpsTexture;
    // Start counting frames per second.
    int countFrames = 0;
    fpsTimer.Start();

    // The main loop.
    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT) quit = true;
        }

        // Calculate and correct fps.
        float avgFps = countFrames / (fpsTimer.GetTicks() / 1000.f);
        if (avgFps > 2000000) avgFps = 0;

        fpsText.str("");
        fpsText << "平均FPS为：" << avgFps;
        if (!fpsTexture.LoadFromRenderedText(g_renderer, g_font, fpsText.str(), fpsColor))
                std::cout << "Unable to render fps texture!\n";

        SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(g_renderer);

        fpsTexture.Render(g_renderer, 10, 10);

        SDL_RenderPresent(g_renderer);
        ++countFrames;
    }

    close();
    return 0;
}

bool init()
{
    // Initialize SDL subsystem.
    if (SDL_Init(SDL_INIT_VIDEO) != 0) return false;

    // Create SDL window.
    g_window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              g_screenWidth, g_screenHeight, SDL_WINDOW_SHOWN);
    if (g_window == NULL) return false;

    // Create SDL renderer.
    g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
    if (g_renderer == NULL) return false;

    // Initialize SDL ttf.
    if (TTF_Init() == -1) return false;

    // Everthing is OK.
    return true;
}

bool loadMedia()
{
    // Load font.
    g_font = TTF_OpenFont("msyh.ttc", 28);
    if (g_font == NULL) return false;

    // Everthing is OK.
    return true;
}

void close()
{
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
    TTF_CloseFont(g_font);

    g_renderer       = NULL;
    g_window         = NULL;
    g_font           = NULL;

    TTF_Quit();
    SDL_Quit();
}
