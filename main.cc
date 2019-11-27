#include <string>
#include <sstream>

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

SDL_Window*   window         = NULL;
SDL_Renderer* renderer       = NULL;
SDL_Texture*  prompt_texture = NULL;
SDL_Texture*  text_texture   = NULL;
TTF_Font*     font           = NULL;

bool init();
bool load_media();
void close();
SDL_Texture* create_texture_from_text(std::string text, SDL_Color color);

int main(int argc, char* argv[])
{
  bool quit = false;

  if (init() == false)       quit = true;
  if (load_media() == false) quit = true;

  SDL_Event e;

  // Current time start time.
  Uint32 start_time = 0;
  // In memory text stream.
  std::stringstream time_text;
  // Text color.
  SDL_Color color = {0, 0, 0, 255};

  // Text rectangles.
  SDL_Rect prompt_rect;
  SDL_Rect text_rect;
  // Text texture width and height.
  int prompt_w = 0;
  int prompt_h = 0;
  int text_w   = 0;
  int text_h   = 0;

  // Get prompt texture width and height.
  SDL_QueryTexture(prompt_texture, NULL, NULL, &prompt_w, &prompt_h);
  // Set prompt rect.
  prompt_rect = {0, 0, prompt_w, prompt_h};

  while (!quit)
  {
    while (SDL_PollEvent(&e))
    {
      if (e.type == SDL_QUIT) quit = true;

      // Reset start time on return keypress.
      else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN)
        start_time = SDL_GetTicks();
    }

    // Set text to be rendered.
    time_text.str("");
    time_text << "Milliseconds since start time "
              << SDL_GetTicks() - start_time;

    // Start Rendering text.
    // Free text_texture's memory first.
    SDL_DestroyTexture(text_texture);
    // Then create texture from time text.
    text_texture = create_texture_from_text(time_text.str(), color);
    if (text_texture == NULL)
    {
      printf("Failed to create time text texture: %s\n", SDL_GetError());
      quit = true;
    }

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, prompt_texture, 0, &prompt_rect);

    // Get text texture width and height.
    SDL_QueryTexture(text_texture, NULL, NULL, &text_w, &text_h);
    // Set text rect.
    text_rect = {0, 240, text_w, text_h};
    SDL_RenderCopy(renderer, text_texture, 0, &text_rect);

    SDL_RenderPresent(renderer);
  }

  close();
  return 0;
}

bool init()
{
  // Initialize SDL subsystem.
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    printf("Failed to initialize SDL: %s\n", SDL_GetError());
    return false;
  }

  // Create SDL window.
  window = SDL_CreateWindow("Lazy Foo's SDL Tutorial",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            640, 480, SDL_WINDOW_SHOWN);
  if (window == NULL)
  {
    printf("Failed to create window: %s\n", SDL_GetError());
    return false;
  }
  
  // Create SDL renderer.
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL)
  {
    printf("Failed to create renderer: %s\n", SDL_GetError());
    return false;
  }

  // Initialize SDl ttf.
  if (TTF_Init() == -1)
  {
    printf("Failed to initalize SDL ttf: %s\n", TTF_GetError());
    return false;
  }

  return true;
}

bool load_media()
{
  // Load the font.
  font = TTF_OpenFont("comic.ttf", 28);
  if (font == NULL)
  {
    printf("Failed to load comic font: %s\n", TTF_GetError());
    return false;
  }

  // create texture from given text.
  SDL_Color color = {0, 0, 0, 255};
  prompt_texture = create_texture_from_text("Press Enter to Reset Start Time.",
                                            color);
  if (prompt_texture == NULL)
  {
    printf("Failed to create prompt text texture: %s\n", SDL_GetError());
    return false;
  }

  return true;
}

SDL_Texture* create_texture_from_text(std::string text, SDL_Color color)
{
  SDL_Surface* surf = TTF_RenderText_Solid(font, text.c_str(), color);
  SDL_Texture* texture;
  texture = SDL_CreateTextureFromSurface(renderer, surf);
  SDL_FreeSurface(surf);

  return texture;
}

void close()
{
  TTF_CloseFont(font);
  SDL_DestroyTexture(prompt_texture);
  SDL_DestroyTexture(text_texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  font           = NULL;
  prompt_texture = NULL;
  text_texture   = NULL;
  renderer       = NULL;
  window         = NULL;
  
  TTF_Quit();
  SDL_Quit();
}
