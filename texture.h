#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <string>

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

class Texture
{
public:
    Texture();
    ~Texture();

    bool LoadFromRenderedText(SDL_Renderer* renderer, TTF_Font* font,
                              std::string text, SDL_Color color);
    void Render(SDL_Renderer* renderer, int x, int y, SDL_Rect* srcRect = NULL);
    void Free();

private:
    SDL_Texture* texture_;
    int          width_;
    int          height_;
};

#endif  // TEXTURE_H_
