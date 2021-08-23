#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <unistd.h>
#include <ctime>
#include <memory>
#include <iostream>

#include "Floor.hpp"
#include "Entity.hpp"
#include "Item.hpp"

bool operator==(SDL_Point& p1, SDL_Point& p2){

    return p1.x == p2.x && p1.y == p2.y;

}

void setColor(SDL_Renderer* renderer, SDL_Color color){

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

}

template<ushort W, ushort H>
int updateRealTileSize(Floor<W, H>& floor, int windowWidth, int windowHeight, SDL_Rect& tile, int tileSize, int& floorX, int& floorY){

    int margin = 30;
    int floorWidth = floor.getWidth();
    int floorHeight = floor.getHeight();

/*    int realTileSize = std::min(windowWidth / (floorWidth + margin * 2),
                            windowHeight / (floorHeight + margin * 2));*/

    std::cout << "Horizontal factor: " << windowWidth / (tileSize * floorWidth + margin * 2) <<
    ", vertical factor: " << windowHeight / (tileSize * floorHeight + margin * 2) << std::endl;

    int realTileSize = std::max(1, std::min(
            windowWidth / (tileSize * floorWidth + margin * 2),
            windowHeight / (tileSize * floorHeight + margin * 2)))
                    * tileSize;

    floorX = (windowWidth - floorWidth * realTileSize) / 2;
    floorY = (windowHeight - floorHeight * realTileSize) / 2;

    tile.h = tile.w = realTileSize;

    return realTileSize;

}

int main(){

    if (SDL_Init(SDL_INIT_VIDEO) != 0){

        printf("Error while initializing SDL2:\n%s", SDL_GetError());
        return EXIT_FAILURE;

    }

    if (IMG_Init(IMG_INIT_PNG) == 0){

        printf("Error while initializing SDL_image png support:\n%s", IMG_GetError());
        return EXIT_FAILURE;

    }

    if (TTF_Init() == -1){

        printf("Error while initializing SDL_ttf:\n%s", TTF_GetError());
        return EXIT_FAILURE;

    }

    srand(time(nullptr));

    bool running = true;
    SDL_Event event;

    const int tileSize = 16;
    int realTileSize;
    const int floorWidth = 20, floorHeight = 10;
    int width = 640, height = 480;
    int floorX = width / 2, floorY = height / 2;

    Floor<floorWidth, floorHeight> floor;

    /*for (int j = 0; j < floorHeight; ++j){
        for (int i = 0; i < floorWidth; ++i){

            floorGround[i][j] = rand() % 2 ? GROUND : NO;

        }
    }*/

    SDL_Window* window = SDL_CreateWindow("SimplyDungeons", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (window == nullptr){

        printf("Could not create a window.");
        return 1;

    }

    SDL_MaximizeWindow(window);
    SDL_SetWindowBordered(window, SDL_FALSE);
    SDL_GetWindowSize(window, &width, &height);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Color clearColor = {0, 0, 0, 255};

    setColor(renderer, clearColor);

    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_Surface* playerSurface = IMG_Load("../res/royals.png");
    SDL_Surface* itemSurface = IMG_Load("../res/items.png");
    SDL_Surface* plainsSurface = IMG_Load("../res/plains.png");

    if (!(playerSurface && itemSurface && plainsSurface)){

        printf("IMG_Load: %s\n", IMG_GetError());

    }

    TTF_Font* font = TTF_OpenFont("../res/PressStart2P.ttf", 20);

    SDL_Texture* playerTex = SDL_CreateTextureFromSurface(renderer, playerSurface);
    SDL_Texture* itemTex = SDL_CreateTextureFromSurface(renderer, itemSurface);
    SDL_Texture* plainsTex = SDL_CreateTextureFromSurface(renderer, plainsSurface);

    int tiles[2][5] = {
            {0, 10, 11, 2, 0},
            {6,  2,  3, 9, 7}
    };

    //  0 6 Void
    // 10 2 Dirt
    // 11 3 Grass
    //  2 9 Water
    //  0 7 Scale

    SDL_FreeSurface(playerSurface);
    SDL_FreeSurface(itemSurface);
    SDL_FreeSurface(plainsSurface);

    SDL_Rect tile{floorX, floorY, realTileSize, realTileSize};
    SDL_Rect srcTile{0, 0, tileSize, tileSize};
    SDL_Rect textRect{10, 10, 0, 0};

    //Entity player(rand() % floorWidth, rand() % floorHeight);
    std::unique_ptr<Item> item = std::make_unique<Item>(new Item(rand() % floorWidth, rand() % floorHeight));

    realTileSize = updateRealTileSize<floorWidth, floorHeight>(floor, width, height, tile, tileSize, floorX, floorY);

    while (running){

        SDL_Point& playerPos = floor.getPlayerPos();

        if (item != nullptr && playerPos == item->getPos()){

            item.reset(nullptr);

        }

        setColor(renderer, clearColor);
        SDL_RenderClear(renderer);

        tile.w = realTileSize;
        tile.h = realTileSize;
        tile.x = 0;
        tile.y = 0;

        srcTile.w = tileSize;
        srcTile.h = tileSize;

        for (int j = 0; j < floorHeight; ++j){
            for (int i = 0; i < floorWidth; ++i){

                tile.x = floorX + i * realTileSize;
                tile.y = floorY + j * realTileSize;

                if (floor(i, j) == NO){

                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderFillRect(renderer, &tile);

                } else {

                    srcTile.x = tiles[0][floor(i, j)] * tileSize;
                    srcTile.y = tiles[1][floor(i, j)] * tileSize;

                    SDL_RenderCopy(renderer, plainsTex, &srcTile, &tile);

                }
            }
        }

        if (item != nullptr){

            SDL_Point& itemPos = item->getPos();

            tile.x = floorX + itemPos.x * realTileSize;
            tile.y = floorY + itemPos.y * realTileSize;
            tile.w = realTileSize;
            tile.h = realTileSize;

            srcTile.x = 0;
            srcTile.y = tileSize;
            srcTile.w = tileSize;
            srcTile.h = tileSize;

            SDL_RenderCopy(renderer, itemTex, &srcTile, &tile);

        }

        tile.x = floorX + playerPos.x * realTileSize;
        tile.y = floorY + (playerPos.y - 1) * realTileSize;
        tile.w = realTileSize;
        tile.h = realTileSize * 2;

        srcTile.x = 0;
        srcTile.y = 0;
        srcTile.w = tileSize;
        srcTile.h = tileSize * 2;

        SDL_RenderCopy(renderer, playerTex, &srcTile, &tile);

        std::string s = "Player: x" + std::to_string(playerPos.x) + " y" + std::to_string(playerPos.y);

        if (item != nullptr){

            SDL_Point itemPos = item->getPos();

            s += ", Item: x" + std::to_string(itemPos.x) + " y" + std::to_string(itemPos.y);

        }

        SDL_Surface* textSurface = TTF_RenderText_Solid(font, s.c_str(), {255, 255, 255, 255});
        SDL_Texture* textTex = SDL_CreateTextureFromSurface(renderer, textSurface);

        textRect.w = textSurface->w;
        textRect.h = textSurface->h;

        SDL_RenderCopy(renderer, textTex, nullptr, &textRect);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTex);

        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event)){
            switch (event.type){
                case SDL_QUIT:
                    running = false;
                    break;

                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym){
                        case SDLK_z:
                            if (playerPos.y > 0){

                                playerPos.y--;

                            }

                            break;

                        case SDLK_q:
                            if (playerPos.x > 0){

                                playerPos.x--;

                            }

                            break;

                        case SDLK_s:
                            if (playerPos.y < floorHeight - 1){

                                playerPos.y++;

                            }

                            break;

                        case SDLK_d:
                            if (playerPos.x < floorWidth - 1){

                                playerPos.x++;

                            }

                            break;

                        case SDLK_SPACE:
                            floor = Floor<floorWidth, floorHeight>();

                            break;

                    }

                case SDL_KEYUP:
                    if (event.key.state == SDL_RELEASED){
                        switch (event.key.keysym.sym){
                            case SDLK_ESCAPE:
                                running = false;

                                break;

                            case SDLK_F11:
                                if((SDL_GetWindowFlags(window) & SDL_WINDOW_MAXIMIZED) != 0){

                                    SDL_RestoreWindow(window);
                                    SDL_SetWindowBordered(window, SDL_TRUE);

                                } else {

                                    SDL_MaximizeWindow(window);
                                    SDL_SetWindowBordered(window, SDL_FALSE);

                                }

                                break;

                        }
                    }

                    break;

                case SDL_WINDOWEVENT:

                    if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED){

                        width = event.window.data1;
                        height = event.window.data2;

                        realTileSize = updateRealTileSize<floorWidth, floorHeight>(floor, width, height, tile, tileSize, floorX, floorY);

                    }

                    break;

                default:
                    break;

            }
        }

        usleep(16000);

    }

    TTF_CloseFont(font);

    SDL_DestroyTexture(playerTex);
    SDL_DestroyTexture(itemTex);
    SDL_DestroyTexture(plainsTex);

    SDL_DestroyWindow(window);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;

}
