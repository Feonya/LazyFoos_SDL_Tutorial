#include <cstdio>
#include <string>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

// Texture wrapper class.
class Texture
{
public:
    // Initializes variables.
    Texture();
    // Deallocates memory.
    ~Texture();

    // Loads image at specified path.
    bool LoadFromFile(std::string path);
    // Deallocates texture.
    void Free();
    //Renders texture at given point.
    void Render(int x, int y);
    // Gets image dimensions.
    int GetWidth();
    int GetHeight();

private:
    // The actual hardware texture.
    SDL_Texture* mTexture;

    // Image dimensions.
    int mWidth;
    int mHeight;
};


// Screen dimension constants.
const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;
// The window we'll be rendering to.
SDL_Window* gWindow = NULL;
// The window renderer.
SDL_Renderer* gRenderer = NULL;
// Scene textures.
Texture gFooTexture;
Texture gBackgroundTexture;

// Starts up SDL and creates window.
bool init();
// Load media.
bool loadMedia();
// Frees media and shuts down SDL.
void close();
// Loads individual image as texture.
SDL_Texture* loadTexture(std::string path);

Texture::Texture()
{
    // Initialize.
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

Texture::~Texture()
{
    // Deallocate.
    Free();
}

bool Texture::LoadFromFile(std::string path)
{
    // Get rid of preexisting texture.
    Free();

    // The final texture.
    SDL_Texture* newTexture = NULL;

    // Load image at specified path.
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
        printf("Unable to load image %s! SDL_image_Error: %s\n", path.c_str(), IMG_GetError());
    else
    {
        // Color key image.
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));


        // Get texture from surface pixels.
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL)
            printf("Unable to create texture from %s! SDL_Error: %s\n", path.c_str(), SDL_GetError());
        else
        {
            // Get image dimensions.
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        // Get rid of old loaded surface.
        SDL_FreeSurface(loadedSurface);
    }

    // Return success.
    mTexture = newTexture;
    return mTexture != NULL;
}

void Texture::Free()
{
    // Free texture if it exists.
    if (mTexture != NULL)
    {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void Texture::Render(int x, int y)
{
    // Set rendering space and render to screen.
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};
    SDL_RenderCopy(gRenderer, mTexture, NULL, &renderQuad);
}

int Texture::GetWidth()
{
    return mWidth;
}

int Texture::GetHeight()
{
    return mHeight;
}

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

                // Clear screen.
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                // Render background texture to screen.
                gBackgroundTexture.Render(0, 0);
                // Render Foo to the screen.
                gFooTexture.Render(240, 190);

                // Update screen.
                SDL_RenderPresent(gRenderer);
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
            // Create renderer to window.
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == NULL)
            {
                printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                // Initialize renderer color.
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                // Initialize PNG loading.
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("SDL_image could not initialize! SDL_image_Error: %s\n", IMG_GetError());
                    success = false;
                }
            }
        }
    }

    return success;
}

//SDL_Texture* loadTexture(std::string path)
//{
//    // The final texture.
//    SDL_Texture* newTexture = NULL;
//
//    // Load image at specified path.
//    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
//    if (loadedSurface == NULL)
//        printf("Unable to load image %s! SDL_image_Error: %s\n", path.c_str(), IMG_GetError());
//    else
//    {
//        // Create texture from surface pixels.
//        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
//        if (newTexture == NULL)
//            printf("Unable to create texture from %s! SDL_Error: %s\n", path.c_str(), SDL_GetError());
//
//        // Get rid of old loaded surface.
//        SDL_FreeSurface(loadedSurface);
//    }
//
//    return newTexture;
//}

bool loadMedia()
{
    // Loading success flag.
    bool success = true;

    // Load Foo texture.
    if (!gFooTexture.LoadFromFile("foo.png"))
    {
        printf("Failed to load Foo texture image!\n");
        success = false;
    }

    // Load background texture.
    if (!gBackgroundTexture.LoadFromFile("background.png"))
    {
        printf("Failed to load background texture image!\n");
        success = false;
    }

    // Nothing to load.
    return success;
}

void close()
{
    // Free loaded image.
    gFooTexture.Free();
    gBackgroundTexture.Free();

    // Destory window.
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    // Quit SDL subsystem.
    IMG_Quit();
    SDL_Quit();
}

