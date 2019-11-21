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
// Walking animation.
const int kWalkingAnimationFrames = 4;
SDL_Rect sprite_clips[kWalkingAnimationFrames];
Texture sprite_sheet_texture;

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

      // Current animation frame.
      int frame = 0;

      // While application is running.
      while (!quit) {
        // Handle events on queue.
        while (SDL_PollEvent(&e) != 0) {
          // User requests quit.
          if (e.type == SDL_QUIT) {
            quit = true;
          }
        }

        // Clear screen.
        SDL_RenderClear(renderer);

        // Render current frame.
        SDL_Rect* current_clip = &sprite_clips[frame / 10];
        sprite_sheet_texture.Render((kScreenWidth  - current_clip->w) / 2,
                                    (kScreenHeight - current_clip->h) / 2,
                                    current_clip);

        // Update screen.
        SDL_RenderPresent(renderer);

        // Go to next frame.
        ++frame;

        // Cycle animation.
        if (frame / 10 >= kWalkingAnimationFrames) frame = 0;
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
      // Create vsynced renderer for window.
      renderer = SDL_CreateRenderer(window, -1,
                                    SDL_RENDERER_ACCELERATED |
                                    SDL_RENDERER_PRESENTVSYNC);
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

  // Load sprite sheet texture.
  if (!sprite_sheet_texture.LoadFromFile("foo.png")) {
    printf("Failed to load sprite sheet texture!\n");
    success = false;
  } else {
    // Set sprite clips.
    sprite_clips[0].x = 0;
    sprite_clips[0].y = 0;
    sprite_clips[0].w = 64;
    sprite_clips[0].h = 205;

    sprite_clips[1].x = 64;
    sprite_clips[1].y = 0;
    sprite_clips[1].w = 64;
    sprite_clips[1].h = 205;

    sprite_clips[2].x = 128;
    sprite_clips[2].y = 0;
    sprite_clips[2].w = 64;
    sprite_clips[2].h = 205;

    sprite_clips[3].x = 196;
    sprite_clips[3].y = 0;
    sprite_clips[3].w = 64;
    sprite_clips[3].h = 205;
  }

  // Nothing to load.
  return success;
}

void close() {
  // Free loaded image.
  sprite_sheet_texture.Free();

  // Destory window.
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  window = NULL;
  renderer = NULL;

  // Quit SDL subsystem.
  IMG_Quit();
  SDL_Quit();
}

