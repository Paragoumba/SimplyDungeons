#include "Entity.hpp"

Entity::Entity(int x, int y, Direction _dir) : pos({x, y}), dir(_dir){}

SDL_Point& Entity::getPos(){

    return pos;

}

Direction Entity::getDir() const {

    return dir;

}

void Entity::setPos(const SDL_Point& _pos){

    pos = _pos;

}

void Entity::setDir(Direction _dir){

    dir = _dir;

}
