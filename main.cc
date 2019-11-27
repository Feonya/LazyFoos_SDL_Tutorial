/// Copyright 2019 Tuxyin.

#include <string>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"

SDL_Window*   window   = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture*  texture  = NULL;

// The music that will be played.
Mix_Music* music   = NULL;
// The sound effects that will be used.
Mix_Chunk* kiss    = NULL;
Mix_Chunk* dog     = NULL;
Mix_Chunk* coin    = NULL;
Mix_Chunk* oh_yeah = NULL;

bool init();
bool loadMedia();
void close();

int main(int argc, char* argv[]) {
  bool quit = false;

  if (init() == false) {
    printf("Initialize SDL unsuccessful!\n");
    quit = true;
  }

  if (loadMedia() == false) {
    printf("Load media unsuccessfull!\n");
    quit = true;
  }

  SDL_Event e;

  while (!quit) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
      // Handle key press.
      } else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
          // Play sound effects.
          case SDLK_1:
            Mix_PlayChannel(-1, kiss, 0);
            break;
          case SDLK_2:
            Mix_PlayChannel(-1, dog, 0);
            break;
          case SDLK_3:
            Mix_PlayChannel(-1, coin, 0);
            break;
          case SDLK_4:
            Mix_PlayChannel(-1, oh_yeah, 0);
            break;

          // Start, pause or resume play music.
          case SDLK_9:
            // If there is no music playing.
            if (Mix_PlayingMusic() == 0) {
              // Play the music.
              Mix_PlayMusic(music, 0);
            // If music is being paused.
            } else {
              // If the music is paused.
              if (Mix_PausedMusic() == 1) {
                // Resume the music.
                Mix_ResumeMusic();
              // If the music is playing.
              } else {
                // Pause the music.
                Mix_PauseMusic();
              }
            }
            break;

          // Stop music.
          case SDLK_0:
            Mix_HaltMusic();
            break;
        }
      }
    }

    // Render texture.
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, 0, 0);
    SDL_RenderPresent(renderer);
  }

  close();
  return 0;
}

bool init() {
  // Initialize SDL subsystems.
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
    printf("Failed to initialize SDL: %s\n", SDL_GetError());
    return false;
  }

  // Create SDL window.
  window = SDL_CreateWindow("Lazy Foo's SDL Tutorial",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            800, 640,
                            SDL_WINDOW_SHOWN);
  if (window == NULL) {
    printf("Faile to create SDL window: %s\n", SDL_GetError());
    return false;
  }

  // Create SDL renderer.
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL) {
    printf("Failed to create SDL renderer: %s\n", SDL_GetError());
    return false;
  }

  // Initialize PNG loading.
  if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
    printf("Failed to initialize SDL image: %s\n", IMG_GetError());
    return false;
  }

  // Initialize Audio loading.
  if ((Mix_Init(MIX_INIT_MP3) & MIX_INIT_MP3) != MIX_INIT_MP3) {
    printf("Failed to initialize SDL mix: %s\n", Mix_GetError());
    return false;
  }

  // Initialize the mixer API.
  if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 1024) == -1) {
    printf("Failed to open audio from SDL mix: %s\n", Mix_GetError());
    return false;
  }

  // Everything is OK.
  return  true;
}

bool loadMedia() {
  // Load texture.
  SDL_Surface* surf = IMG_Load("assets/bg.png");
  texture = SDL_CreateTextureFromSurface(renderer, surf);
  if (texture == NULL) {
    printf("Failed to load texture: %s\n", SDL_GetError());
    return false;
  }
  SDL_FreeSurface(surf);

  // Load music.
  music = Mix_LoadMUS("assets/music.mp3");
  if (music == NULL) {
    printf("Failed to load music: %s\n", Mix_GetError());
    return false;
  }

  // Load sound effects.
  kiss = Mix_LoadWAV("assets/kiss.wav");
  if (kiss == NULL) {
    printf("Failed to load sound kiss: %s\n", Mix_GetError());
    return false;
  }

  dog = Mix_LoadWAV("assets/dog.wav");
  if (dog == NULL) {
    printf("Failed to load sound dog: %s\n", Mix_GetError());
    return false;
  }

  coin = Mix_LoadWAV("assets/coin.wav");
  if (coin == NULL) {
    printf("Failed to load sound coin: %s\n", Mix_GetError());
    return false;
  }

  oh_yeah = Mix_LoadWAV("assets/oh_yeah.wav");
  if (oh_yeah == NULL) {
    printf("Failed to load sound oh_yeah: %s\n", Mix_GetError());
    return false;
  }

  // Everything is OK.
  return true;
}

void close() {
  SDL_DestroyTexture(texture);
  texture = NULL;

  Mix_FreeMusic(music);
  Mix_FreeChunk(kiss);
  Mix_FreeChunk(dog);
  Mix_FreeChunk(coin);
  Mix_FreeChunk(oh_yeah);
  music   = NULL;
  kiss    = NULL;
  dog     = NULL;
  coin    = NULL;
  oh_yeah = NULL;

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  renderer = NULL;
  window   = NULL;

  Mix_Quit();
  IMG_Quit();
  SDL_Quit();
}
