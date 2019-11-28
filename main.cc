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
Texture*      g_promptTexture1 = NULL;
Texture*      g_promptTexture2 = NULL;

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
    // The text color;
    SDL_Color textColor = {0, 0, 0, 255};
    // The text stream in memory.
    std::stringstream timeText;
    // The Application timer.
    Timer timer;
    // The time texture.
    Texture timeTexture;

    // The main loop.
    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT) quit = true;
            else if (e.type == SDL_KEYDOWN)
            {
                // Start/Stop.
                if (e.key.keysym.sym == SDLK_s)
                {
                    if (timer.IsStarted()) timer.Stop();
                    else                   timer.Start();
                }
                // Pause/Unpause.
                else if (e.key.keysym.sym == SDLK_p)
                {
                    if (timer.IsPaused()) timer.Unpause();
                    else                  timer.Pause();
                }
            }
        }

        // Set text to be rendered.
        timeText.str("");
        timeText << "Seconds since start time " << timer.GetTicks() / 1000.f;
        // Render and load text.
        timeTexture.Free();
        if (!timeTexture.LoadFromRenderedText(g_renderer, g_font, timeText.str(), textColor))
            std::cout << "Unable to render time texture!\n";
        // Clear screen.
        SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(g_renderer);
        // Render texture.
        g_promptTexture1->Render(g_renderer, 0, 0);
        g_promptTexture2->Render(g_renderer, 0, 28);
        timeTexture.Render(g_renderer, 0, g_screenHeight / 2);
        // Update screen.
        SDL_RenderPresent(g_renderer);
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
    g_font = TTF_OpenFont("Gabriola.ttf", 28);
    if (g_font == NULL) return false;

    // Load prompt texture.
    SDL_Color color = {0, 0, 0, 255};
    g_promptTexture1 = new Texture();
    g_promptTexture2 = new Texture();
    if (!g_promptTexture1->LoadFromRenderedText(g_renderer, g_font,
                                               "Press S to Start or Stop the Timer",
                                               color)) return false;
    if (!g_promptTexture2->LoadFromRenderedText(g_renderer, g_font,
                                               "Press P to Pause or Unpause the Timer",
                                               color)) return false;

    // Everthing is OK.
    return true;
}

void close()
{
    g_promptTexture1->Free();
    g_promptTexture2->Free();
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
    TTF_CloseFont(g_font);

    g_promptTexture1 = NULL;
    g_promptTexture2 = NULL;
    g_renderer       = NULL;
    g_window         = NULL;
    g_font           = NULL;

    TTF_Quit();
    SDL_Quit();
}
