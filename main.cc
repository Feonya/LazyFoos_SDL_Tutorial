// Copyright 2019 Tuxyin.

#include "./texture.h"

bool init();
void close();

SDL_Window*   window          = NULL;
SDL_Renderer* renderer        = NULL;
Texture*      current_texture = NULL;
Texture       textures[5];

int main(int argc, char* argv[]) {
  bool quit = false;

  if (!init()) {
    printf("Failed to init SDL!\n");
    quit = true;
  }

  SDL_Event e;

  textures[0].LoadFromFile(renderer, "up.png");
  textures[1].LoadFromFile(renderer, "down.png");
  textures[2].LoadFromFile(renderer, "left.png");
  textures[3].LoadFromFile(renderer, "right.png");
  textures[4].LoadFromFile(renderer, "default.png");

  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT)
        quit = true;
    }

    // Set texture based on current keystate.
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    if (currentKeyStates[SDL_SCANCODE_UP]) {
      current_texture = &textures[0];
    } else if (currentKeyStates[SDL_SCANCODE_DOWN]) {
      current_texture = &textures[1];
    } else if (currentKeyStates[SDL_SCANCODE_LEFT]) {
      current_texture = &textures[2];
    } else if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
      current_texture = &textures[3];
    } else {
      current_texture = &textures[4];
    }

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);

    current_texture->Render(renderer);

    SDL_RenderPresent(renderer);
  }

  close();

  return 0;
}

bool init() {
  bool success = true;

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("Failed to Init SDL! SDL Error: %s\n", SDL_GetError());
    success = false;
  } else {
    window = SDL_CreateWindow("SDL Tutorial",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        640, 480, SDL_WINDOW_SHOWN);
    if (window == NULL) {
      printf("Failed to create window! SDL Error: %s\n", SDL_GetError());
      success = false;
    } else {
      renderer = SDL_CreateRenderer(window, -1,
          SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
      if (renderer == NULL) {
        printf("Failed to create renderer! SDL Error: %s\n", SDL_GetError());
        success = false;
      }
    }
  }

  if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
    printf("Failed to init SDL_image! SDL_image Error: %s\n", IMG_GetError());
    success = false;
  }

  return success;
}

void close() {
  SDL_DestroyRenderer(renderer);
  renderer = NULL;
  SDL_DestroyWindow(window);
  window = NULL;

  SDL_Quit();
}
