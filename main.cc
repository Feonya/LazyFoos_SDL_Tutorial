// Copyright 2019 Tuxyin.

#include <cstdio>
#include <string>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

enum ButtonSprite {
  BUTTON_SPRITE_MOUSE_OUT         = 0,
  BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
  BUTTON_SPRITE_MOUSE_DOWN        = 2,
  BUTTON_SPRITE_MOUSE_UP          = 3,
  BUTTON_SPRITE_TOTAL             = 4
};

// Texture wrapper class.
class Texture {
 public:
  // Initializes variables.
  Texture();
  // Deallocates memory.
  ~Texture();

  // Loads image at specified path.
  bool LoadFromFile(std::string path);

#if defined(_SDL_TTF_H) || defined(SDL_TTF_H)
  // Creates image from font string.
  bool LoadFromRenderedText(std::string texture_text, SDL_Color text_color);
#endif

  // Deallocates texture.
  void Free();
  // Renders texture at given point.
  void Render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0,
              SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

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

// The mouse button.
class Button {
 public:
  // Initialize internal variables.
  Button();

  // Handles mouse event.
  void HandleEvent(SDL_Event* e);
  // Shows button sprite.
  void Render();

  // Sets top left position.
  void set_position(int x, int y);

 private:
  // Top left position.
  SDL_Point position_;

  // Currently used global sprite.
  ButtonSprite current_sprite_;
};

// Button constants.
const int kButtonWidth  = 320;
const int kButtonHeight = 240;
const int kTotalButtons = 4;
// Screen dimension constants.
const int kScreenWidth  = 640;
const int kScreenHeight = 480;
// The window we'll be rendering to.
SDL_Window* window = NULL;
// The window renderer.
SDL_Renderer* renderer = NULL;
// button sprite sheet texture.
Texture button_sprite_sheet_texture;
SDL_Rect sprite_clips[BUTTON_SPRITE_TOTAL];
// The button objects.
Button buttons[kTotalButtons];

// Starts up SDL and creates window.
bool init();
// Load media.
bool loadMedia();
// Frees media and shuts down SDL.
void close();
// Loads individual image as texture.
SDL_Texture* loadTexture(std::string path);

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

      // Set buttons position.
      buttons[0].set_position(0, 0);
      buttons[1].set_position(0, kScreenHeight / 2);
      buttons[2].set_position(kScreenWidth / 2, 0);
      buttons[3].set_position(kScreenWidth / 2, kScreenHeight / 2);

      // While application is running.
      while (!quit) {
        // Handle events on queue.
        while (SDL_PollEvent(&e) != 0) {
          // User requests quit.
          if (e.type == SDL_QUIT) {
            quit = true;
          }

          // Handle button events.
          for (int i = 0; i < kTotalButtons; ++i)
            buttons[i].HandleEvent(&e);
        }

        // Clear screen.
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);

        // Render buttons.
        for (int i = 0; i < kTotalButtons; ++i)
          buttons[i].Render();

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

        // Initialize SDL_ttf.
        if (TTF_Init() == -1) {
          printf("SDL_ttf could not initialize! SDL_ttf_Error: %s\n",
                 TTF_GetError());
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

  // Loads button sprites.
  button_sprite_sheet_texture.LoadFromFile("button_sprites.png");

  // Initialize sprite clips.
  sprite_clips[0] = {0, 0, kButtonWidth, kButtonHeight};
  sprite_clips[1] = {kButtonWidth, 0, kButtonWidth, kButtonHeight};
  sprite_clips[2] = {0, kButtonHeight, kButtonWidth, kButtonHeight};
  sprite_clips[3] = {kButtonWidth, kButtonHeight, kButtonWidth, kButtonHeight};

#if defined(_SDL_TTF_H) || defined(SDL_TTF_H)
  // Open the font.
  font = TTF_OpenFont("Gabriola.ttf", 28);
  if (font == NULL) {
    printf("Failed to load font! SDL_ttf_Error: %s\n", TTF_GetError());
    success = false;
  } else {
    // Render text.
    SDL_Color text_color = {0, 0, 0};
    if (!text_texture.LoadFromRenderedText("This is the test Text",
                                           text_color)) {
      printf("Failed to render text texture!\n");
      success = false;
    }
  }
#endif

  // Nothing to load.
  return success;
}

void close() {
  // Destory window.
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  window = NULL;
  renderer = NULL;

  // Quit SDL subsystem.
  IMG_Quit();
  SDL_Quit();
}

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

#if defined(_SDL_TTF_H) || defined(SDL_TTF_H)
bool Texture::LoadFromRenderedText(std::string texture_text,
                                   SDL_Color text_color) {
  // Get rid of preexisting texture.
  Free();

  // Render text surface.
  SDL_Surface* text_surface = TTF_RenderText_Solid(font, texture_text.c_str(),
                                                  text_color);
  if (text_surface == NULL) {
    printf("Unable to render text surface! SDL_ttf_Error: %s\n",
           TTF_GetError());
  } else {
    // Create texture from surface pixels.
    texture_ = SDL_CreateTextureFromSurface(renderer, text_surface);
    if (texture_ == NULL) {
      printf("Unable to create texture from rendered text! SDL_Error: %s\n",
             SDL_GetError());
    } else {
      // Get image dimensions.
      width_  = text_surface->w;
      height_ = text_surface->h;
    }

    // Get rid of old surface.
    SDL_FreeSurface(text_surface);
  }

  // return success.
  return texture_ != NULL;
}
#endif

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

void Texture::Render(int x, int y, SDL_Rect* clip, double angle,
                     SDL_Point* center, SDL_RendererFlip flip) {
  // Set rendering space and render to screen.
  SDL_Rect render_quad = {x, y, width_, height_};

  // Set clip rendering dimensions.
  if (clip != NULL) {
    render_quad.w = clip->w;
    render_quad.h = clip->h;
  }

  SDL_RenderCopyEx(renderer, texture_, clip, &render_quad,
                   angle, center, flip);
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

Button::Button() : position_{0, 0}, current_sprite_{BUTTON_SPRITE_MOUSE_OUT} {}

void Button::set_position(int x, int y) {
  position_.x = x;
  position_.y = y;
}

void Button::HandleEvent(SDL_Event* e) {
  // If mouse event happened.
  if (e->type == SDL_MOUSEMOTION ||
      e->type == SDL_MOUSEBUTTONDOWN ||
      e->type == SDL_MOUSEBUTTONUP) {
    // Get mouse position.
    int x, y;
    SDL_GetMouseState(&x, &y);

    // Check if mouse is in button.
    bool inside = true;

    // Mouse is left of the button.
    if (x < position_.x) {
      inside = false;
    // Mouse is right of the button.
    } else if (x > position_.x + kButtonWidth) {
      inside = false;
    // Mouse is above the button.
    } else if (y < position_.y) {
      inside = false;
    // Mouse is below the button.
    } else if (y > position_.y + kButtonHeight) {
      inside = false;
    }

    // Mouse is outside button.
    if (!inside) {
      current_sprite_ = BUTTON_SPRITE_MOUSE_OUT;
    // Mouse is over sprite.
    } else {
      // Set mouse over sprite.
      switch (e->type) {
        case SDL_MOUSEMOTION:
          current_sprite_ = BUTTON_SPRITE_MOUSE_OVER_MOTION;
          break;

        case SDL_MOUSEBUTTONDOWN:
          current_sprite_ = BUTTON_SPRITE_MOUSE_DOWN;
          break;

        case SDL_MOUSEBUTTONUP:
          current_sprite_ = BUTTON_SPRITE_MOUSE_UP;
          break;
      }
    }
  }
}

void Button::Render() {
  // Show current button sprite.
  button_sprite_sheet_texture.Render(position_.x, position_.y,
                                     &sprite_clips[current_sprite_]);
}
