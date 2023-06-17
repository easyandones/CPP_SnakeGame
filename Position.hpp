#ifndef POSITION_H
#define POSITION_H

#include "Direction.hpp"


class Position {
    public:
        int x;
        int y;
        Position(int x, int y): x{x}, y{y} {}
        bool operator==(Position& position) {
            return this->x == position.x && this->y == position.y;
        }
        bool operator!=(Position& position) {
            return !(*this == position);
        }
        Position operator+(Direction direction) {
            if (direction.isUp()) {
                return Position{this->x, this->y - 1};
            }
            else if (direction.isDown()) {
                return Position{this->x, this->y + 1};
            }
            else if (direction.isLeft()) {
                return Position{this->x - 1, this->y};
            }
            else if (direction.isRight()) {
                return Position{this->x + 1, this->y};
            }
            else {
                return *this;
            }
        }
};

#endif