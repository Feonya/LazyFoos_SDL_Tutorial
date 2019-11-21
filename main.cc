// Copyright 2019 Tuxyin.

#include <cstdio>

#include <string>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

// Texture wrapper class.
class Texture {
 public:
  // Initializes variables.
  Texture();
  // Deallocates memory.
  ~Texture();

  // Loads image at specified path.
  bool LoadFromFile(std::string path);
  // Deallocates texture.
  void Free();
  // Renders texture at given point.
  void Render(int x, int y, SDL_Rect* clip = NULL);

  // Set color modulation.
  void set_color(Uint8 red, Uint8 green, Uint8 blue);
  // Set blending.
  void set_blend_mode(SDL_BlendMode blending);
  // Set alpha modulation.
  void set_alpha(Uint8 alpha);
  // Gets image dimensions.
  int width();
  int height();

 private:
  // The actual hardware texture.
  SDL_Texture* texture_;

  // Image dimensions.
  int width_;
  int height_;
};


// Screen dimension constants.
const int kScreenWidth = 640;
const int kScreenHeight = 480;
// The window we'll be rendering to.
SDL_Window* window = NULL;
// The window renderer.
SDL_Renderer* renderer = NULL;
// The texture for modulation.
Texture modulated_texture;
// The background texture.
Texture background_texture;

// Starts up SDL and creates window.
bool init();
// Load media.
bool loadMedia();
// Frees media and shuts down SDL.
void close();
// Loads individual image as texture.
SDL_Texture* loadTexture(std::string path);

Texture::Texture() {
  // Initialize.
  texture_ = NULL;
  width_ = 0;
  height_ = 0;
}

Texture::~Texture() {
  // Deallocate.
  Free();
}

bool Texture::LoadFromFile(std::string path) {
  // Get rid of preexisting texture.
  Free();

  // The final texture.
  SDL_Texture* new_texture = NULL;

  // Load image at specified path.
  SDL_Surface* loaded_surface = IMG_Load(path.c_str());
  if (loaded_surface == NULL) {
    printf("Unable to load image %s! SDL_image_Error: %s\n",
           path.c_str(), IMG_GetError());
  } else {
    // Color key image.
    SDL_SetColorKey(loaded_surface, SDL_TRUE,
                    SDL_MapRGB(loaded_surface->format, 0, 0xFF, 0xFF));

    // Get texture from surface pixels.
    new_texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
    if (new_texture == NULL) {
      printf("Unable to create texture from %s! SDL_Error: %s\n",
             path.c_str(), SDL_GetError());
    } else {
        // Get image dimensions.
      width_ = loaded_surface->w;
      height_ = loaded_surface->h;
    }

    // Get rid of old loaded surface.
    SDL_FreeSurface(loaded_surface);
  }

  // Return success.
  texture_ = new_texture;
  return texture_ != NULL;
}

void Texture::Free() {
  // Free texture if it exists.
  if (texture_ != NULL) {
    SDL_DestroyTexture(texture_);
    texture_ = NULL;
    width_ = 0;
    height_ = 0;
  }
}

void Texture::set_color(Uint8 red, Uint8 green, Uint8 blue) {
  // Modulate texture.
  SDL_SetTextureColorMod(texture_, red, green, blue);
}

void Texture::Render(int x, int y, SDL_Rect* clip) {
  // Set rendering space and render to screen.
  SDL_Rect render_quad = {x, y, width_, height_};

  // Set clip rendering dimensions.
  if (clip != NULL) {
    render_quad.w = clip->w;
    render_quad.h = clip->h;
  }

  SDL_RenderCopy(renderer, texture_, clip, &render_quad);
}

void Texture::set_blend_mode(SDL_BlendMode blending) {
  // Set blending function.
  SDL_SetTextureBlendMode(texture_, blending);
}

void Texture::set_alpha(Uint8 alpha) {
  // Modulate texture alpha.
  SDL_SetTextureAlphaMod(texture_, alpha);
}

int Texture::width() {
  return width_;
}

int Texture::height() {
  return height_;
}

int main(int argc, char* argv[]) {
  // Start up SDL and create window.
  if (!init()) {
    printf("Failed to initialze!\n");
  } else {
    // Load media.
    if (!loadMedia()) {
      printf("Failed to load media!\n");
    } else {
      // Main loop flag.
      bool quit = false;

      // Event handler.
      SDL_Event e;

      // Modulation component.
      Uint8 a = 255;

      // While application is running.
      while (!quit) {
        // Handle events on queue.
        while (SDL_PollEvent(&e) != 0) {
          // User requests quit.
          if (e.type == SDL_QUIT) {
            quit = true;
          // On keypress change rgb values.
          } else if (e.type == SDL_KEYDOWN) {
            // Increase alpha on w.
            if (e.key.keysym.sym == SDLK_w) {
              // Cap if over 255.
              if (a + 32 > 255) a  = 255;
              // Increment otherwise.
              else              a += 32;
            // Decrease alpha on s.
            } else if (e.key.keysym.sym == SDLK_s) {
              // Cap if below 0.
              if (a - 32 < 0) a  = 0;
              // Decrement otherwise.
              else            a -= 32;
            }
          }
        }

        // Clear screen.
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);

        // Render background texture.
        background_texture.Render(0, 0);

        // Render front texture.
        modulated_texture.set_alpha(a);
        modulated_texture.Render(0, 0);

        // Update screen.
        SDL_RenderPresent(renderer);
      }
    }
  }

  // Free resources and close SDL.
  close();

  return 0;
}

bool init() {
  // Initialize flag.
  bool success = true;

  // Initialize SDL.
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    success = false;
  } else {
    // Create window.
    window = SDL_CreateWindow("SDL_Tutorial",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              kScreenWidth, kScreenHeight,
                              SDL_WINDOW_SHOWN);
    if (window == NULL) {
      printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
      success = false;
    } else {
      // Create renderer to window.
      renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
      if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n",
               SDL_GetError());
        success = false;
      } else {
        // Initialize renderer color.
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

        // Initialize PNG loading.
        int img_flags = IMG_INIT_PNG;
        if (!(IMG_Init(img_flags) & img_flags)) {
          printf("SDL_image could not initialize! SDL_image_Error: %s\n",
                 IMG_GetError());
          success = false;
        }
      }
    }
  }

  return success;
}

// SDL_Texture* loadTexture(std::string path) {
//   // The final texture.
//   SDL_Texture* new_texture = NULL;
//
//   // Load image at specified path.
//   SDL_Surface* loaded_surface = IMG_Load(path.c_str());
//   if (loaded_surface == NULL) {
//     printf("Unable to load image %s! SDL_image_Error: %s\n",
//            path.c_str(), IMG_GetError());
//   } else {
//     // Create texture from surface pixels.
//     new_texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
//     if (new_texture == NULL)
//       printf("Unable to create texture from %s! SDL_Error: %s\n",
//              path.c_str(), SDL_GetError());
//
//     // Get rid of old loaded surface.
//     SDL_FreeSurface(loaded_surface);
//   }
//
//   return new_texture;
// }

bool loadMedia() {
  // Loading success flag.
  bool success = true;

  // Load front alpha texture.
  if (!modulated_texture.LoadFromFile("fadeout.png")) {
    printf("Failed to load front texture!\n");
    success = false;
  } else {
    // Set standard alpha blending.
    modulated_texture.set_blend_mode(SDL_BLENDMODE_BLEND);
  }

  // Load background texture.
  if (!background_texture.LoadFromFile("fadein.png")) {
    printf("Failed to load background texture!\n");
    success = false;
  }

  // Nothing to load.
  return success;
}

void close() {
  // Free loaded image.
  modulated_texture.Free();

  // Destory window.
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  window = NULL;
  renderer = NULL;

  // Quit SDL subsystem.
  IMG_Quit();
  SDL_Quit();
}

