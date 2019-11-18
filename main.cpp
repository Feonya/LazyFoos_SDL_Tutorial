#include <cstdio>
#include <string>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

// Screen dimension constants.
const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;
// The window we'll be rendering to.
SDL_Window* gWindow = NULL;
// The surface contained by the window.
SDL_Surface* gScreenSurface = NULL;
// Current displayed image.
SDL_Surface* gStretchedSurface = NULL;

// Starts up SDL and creates window.
bool init();
// Load media.
bool loadMedia();
// Frees media and shuts down SDL.
void close();
// Loads individual image.
SDL_Surface* loadSurface(std::string path);

int main(int argc, char* argv[])
{
    // Start up SDL and create window.
    if (!init())
        printf("Failed to initialze!\n");
    else
    {
        // Load media.
        if (!loadMedia())
            printf("Failed to load media!\n");
        else
        {
            // Main loop flag.
            bool quit = false;

            // Event handler.
            SDL_Event e;

            // While application is running.
            while (!quit)
            {
                // Handle events on queue.
                while (SDL_PollEvent(&e) != 0)
                {
                    // User requests quit.
                    if (e.type == SDL_QUIT)
                        quit = true;
                }

                // Apply the image stretched.
                SDL_Rect stretchRect;
                stretchRect.x = 0;
                stretchRect.y = 0;
                stretchRect.w = SCREEN_WIDTH;
                stretchRect.h = SCREEN_HEIGHT;
                SDL_BlitScaled(gStretchedSurface, NULL, gScreenSurface, &stretchRect);
                // Update the surface.
                SDL_UpdateWindowSurface(gWindow);
            }
        }
    }

    // Free resources and close SDL.
    close();

    return 0;
}

bool init()
{
    // Initialize flag.
    bool success = true;

    // Initialize SDL.
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        // Create window.
        gWindow = SDL_CreateWindow("SDL_Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                  SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            // Initialize PNG loading.
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) & imgFlags))
            {
                printf("SDL_image could not initialize! SDL_image_Error: %s\n", IMG_GetError());
                success = false;
            }
            else
                // Get window surface.
                gScreenSurface = SDL_GetWindowSurface(gWindow);
        }
    }

    return success;
}

bool loadMedia()
{
    // Loading success flag.
    bool success = true;

    // Load surface.
    gStretchedSurface = loadSurface("png_image.png");
    if (gStretchedSurface == NULL)
    {
        printf("Failed to load png image file!\n");
        success = false;
    }

    // Load splash image
    return success;
}

void close()
{
    // Deallocate surface.
    SDL_FreeSurface(gStretchedSurface);
    gStretchedSurface = NULL;

    // Destory window.
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    // Quit SDL subsystem.
    SDL_Quit();
}

SDL_Surface* loadSurface(std::string path)
{
    // The final optimized image.
    SDL_Surface* optimizedSurface = NULL;

    // Load image at specified path.
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
        printf("Unable to load image %s! SDL_image_Error: %s\n", path.c_str(), IMG_GetError());
    else
    {
        // Convert surface to screen format.
        optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
        if (optimizedSurface == NULL)
            printf("Unable to optimize image %s! SDL_Error: %s\n", path.c_str(), SDL_GetError());

        // Get rid of old loaded surface.
        SDL_FreeSurface(loadedSurface);
    }

    return optimizedSurface;
}
