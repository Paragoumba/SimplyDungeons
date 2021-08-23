#include "Item.hpp"

Item::Item(int x, int y) : pos({x, y}){}

Item::Item(Item* item) : pos(item->pos){}

SDL_Point& Item::getPos(){

    return pos;

}

void Item::setPos(const SDL_Point& _pos){

    pos = _pos;

}
