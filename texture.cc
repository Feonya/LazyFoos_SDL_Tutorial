/// Copyright 2019 Tuxyin.

#include "./texture.h"

Texture::Texture() : texture_(NULL) {}

Texture::~Texture() { Free(); }

bool Texture::LoadFromFile(SDL_Renderer* renderer, std::string path) {
  SDL_Surface* surf = IMG_Load(path.c_str());
  if (surf == NULL)
    printf("Failed to load image file! SDL_image Error: %s\n", IMG_GetError());
  else
    texture_ = SDL_CreateTextureFromSurface(renderer, surf);
  SDL_FreeSurface(surf);

  return texture_ != NULL;
}

void Texture::Render(SDL_Renderer* renderer) {
  SDL_RenderCopy(renderer, texture_, NULL, NULL);
}

void Texture::Free() {
  SDL_DestroyTexture(texture_);
}
