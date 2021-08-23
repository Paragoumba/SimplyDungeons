#ifndef SIMPLYDUNGEONS_ENTITY_HPP
#define SIMPLYDUNGEONS_ENTITY_HPP

#include <SDL2/SDL_rect.h>

enum Direction {
    NORTH,
    EAST,
    SOUTH,
    WEST
};

class Entity {
private:
    SDL_Point pos;
    Direction dir;

public:
    explicit Entity(int x = 0, int y = 0, Direction dir = SOUTH);

    [[nodiscard]] SDL_Point& getPos();
    [[nodiscard]] Direction getDir() const;

    void setPos(const SDL_Point& pos);
    void setDir(Direction dir);
};

#endif //SIMPLYDUNGEONS_ENTITY_HPP
