#include "texture.h"

Texture::Texture() : texture_(NULL), width_(0), height_(0) {}

Texture::~Texture() { Free(); }

bool Texture::LoadFromRenderedText(SDL_Renderer* renderer, TTF_Font* font,
                                   std::string text, SDL_Color color)
{
    SDL_Surface* surf = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    if (surf == NULL) return false;

    if (texture_ != NULL) SDL_DestroyTexture(texture_);
    texture_ = SDL_CreateTextureFromSurface(renderer, surf);
    if (texture_ == NULL) return false;
    width_  = surf->w;
    height_ = surf->h;

    SDL_FreeSurface(surf);
    return true;
}

void Texture::Render(SDL_Renderer* renderer, int x, int y, SDL_Rect* srcRect)
{
    SDL_Rect destRect = {x, y, width_, height_};

    SDL_RenderCopy(renderer, texture_, srcRect, &destRect);
}

void Texture::Free()
{
    SDL_DestroyTexture(texture_);
}
