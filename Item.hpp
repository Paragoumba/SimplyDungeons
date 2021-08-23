#ifndef SIMPLYDUNGEONS_ITEM_HPP
#define SIMPLYDUNGEONS_ITEM_HPP

#include <SDL2/SDL_rect.h>

class Item {
private:
    SDL_Point pos;

public:
    explicit Item(int x = 0, int y = 0);
    explicit Item(Item* item);

    [[nodiscard]] SDL_Point& getPos();

    void setPos(const SDL_Point& pos);
};

#endif //SIMPLYDUNGEONS_ITEM_HPP
