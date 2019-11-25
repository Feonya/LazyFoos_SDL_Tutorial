/// Copyright 2019 Tuxyin.

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <string>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

class Texture {
 public:
  Texture();
  ~Texture();

  bool LoadFromFile(SDL_Renderer* renderer, std::string path);
  void Render(SDL_Renderer* renderer);
  void Free();

 private:
  SDL_Texture* texture_;
};

#endif  // TEXTURE_H_
