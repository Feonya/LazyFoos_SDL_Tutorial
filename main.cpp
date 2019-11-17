#include <cstdio>
#include <string>
#include "SDL2/SDL.h"

// key press surfaces constants.
enum KeyPressSurfaces
{
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

// Screen dimension constants.
const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;
// The window we'll be rendering to.
SDL_Window* gWindow = NULL;
// The surface contained by the window.
SDL_Surface* gScreenSurface = NULL;
// The images that correspond to a keypress.
SDL_Surface* gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];
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

            // Set default current surface.
            gStretchedSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];

            // While application is running.
            while (!quit)
            {
                // Handle events on queue.
                while (SDL_PollEvent(&e) != 0)
                {
                    // User requests quit.
                    if (e.type == SDL_QUIT)
                        quit = true;
                    // User press a key.
                    else if (e.type == SDL_KEYDOWN)
                        switch (e.key.keysym.sym)
                        {
                            case SDLK_UP:
                                gStretchedSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
                                break;
                            case SDLK_DOWN:
                                gStretchedSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
                                break;
                            case SDLK_LEFT:
                                gStretchedSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
                                break;
                            case SDLK_RIGHT:
                                gStretchedSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
                                break;
                            default:
                                gStretchedSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
                                break;
                        }
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
            // Get window surface.
            gScreenSurface = SDL_GetWindowSurface(gWindow);
    }

    return success;
}

bool loadMedia()
{
    // Loading success flag.
    bool success = true;

    // Load default surface.
    gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("press.bmp");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL)
    {
        printf("Failed to load default image!\n");
        success = false;
    }
    // Load up surface.
    gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = loadSurface("up.bmp");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL)
    {
        printf("Failed to load up image!\n");
        success = false;
    }
    // Load down surface.
    gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = loadSurface("down.bmp");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL)
    {
        printf("Failed to load down image!\n");
        success = false;
    }
    // Load left surface.
    gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = loadSurface("left.bmp");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL)
    {
        printf("Failed to load left image!\n");
        success = false;
    }
    // Load right surface.
    gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadSurface("right.bmp");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL)
    {
        printf("Failed to load right image!\n");
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
    SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
    if (loadedSurface == NULL)
        printf("Unable to load image %s! SDL_Error: %s\n", path.c_str(), SDL_GetError());
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
