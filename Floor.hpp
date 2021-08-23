#ifndef SIMPLYDUNGEONS_FLOOR_HPP
#define SIMPLYDUNGEONS_FLOOR_HPP

#include <memory>
#include <cmath>
#include <iostream>
#include <vector>
#include <SDL2/SDL_rect.h>

enum GroundType {
    NO,
    ROOM1,
    ROOM2,
    ROOM3,
    ROOM4
};

template <ushort W = 20, ushort H = 20>
class Floor {
private:
    std::array<std::array<GroundType, H>, W> ground = {NO};

    SDL_Point playerPos;

public:
    Floor();

    [[nodiscard]] ushort getWidth() const;
    [[nodiscard]] ushort getHeight() const;
    [[nodiscard]] SDL_Point& getPlayerPos();

    GroundType& operator()(ushort i, ushort j){

        return ground[i][j];

    }
};

template <ushort W, ushort H>
Floor<W, H>::Floor(){

    playerPos.x = rand() % (W - 1) + 1;
    playerPos.y = rand() % (H - 1) + 1;

    ushort  minSize = 4;

    std::vector<SDL_Rect> rooms(4);

    GroundType types[4] = {
            ROOM1,
            ROOM2,
            ROOM3,
            ROOM4
    };

    for (int i = 0; i < 4; ++i){

        SDL_Rect& room = rooms[i];

        room.x = rand() % (W - 2 - minSize - 1) + 1;
        room.y = rand() % (H - 2 - minSize - 1) + 1;
        room.w = rand() % (W - room.x - minSize) + minSize;
        room.h = rand() % (H - room.y - minSize) + minSize;

    }

    for (int i = 0; i < 4; ++i){

        SDL_Rect room = rooms[i];

        for (int x = room.x; x < room.x + room.w; ++x){
            for (int y = room.y; y < room.y + room.h; ++y){

                if (x < W && y < H){

                    ground[x][y] = types[i];

                }
            }
        }
    }
}

template <ushort W, ushort H>
ushort Floor<W, H>::getWidth() const {

    return W;

}

template <ushort W, ushort H>
ushort Floor<W, H>::getHeight() const {

    return H;

}

template <ushort W, ushort H>
SDL_Point& Floor<W, H>::getPlayerPos(){

    return playerPos;

}

#endif //SIMPLYDUNGEONS_FLOOR_HPP
