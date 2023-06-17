#ifndef MAP_H
#define MAP_H

#include "Position.hpp"
#include "Blocks.hpp"


class Map {
    private:
        Block*** data;
    public:
        const int WORLD_X, WORLD_Y;
        Map(int xSize, int ySize): WORLD_X{xSize}, WORLD_Y{ySize} {
            this->data = new Block**[WORLD_X];
            for (int i = 0; i < WORLD_X; i++) {
                this->data[i] = new Block*[WORLD_Y];
                for (int j = 0; j < WORLD_Y; j++)
                    this->data[i][j] = new Air{};
            }
        }
        virtual ~Map() {
            for (int i = 0; i < WORLD_X; i++) {
                for (int j = 0; j < WORLD_Y; j++)
                    delete this->data[i][j];
                delete[] this->data[i];
            }
            delete[] this->data;
        }
        void setBlock(Position position, Block* block) {
            delete this->data[position.x][position.y];
            this->data[position.x][position.y] = block;
        }
        Block* getBlock(Position position) {
            if (0 <= position.x && position.x <= this->WORLD_X - 1 && 0 <= position.y && position.y <= this->WORLD_Y - 1)
                return this->data[position.x][position.y];
            return nullptr;
        }
};

#endif