#ifndef DIRECTION_H
#define DIRECTION_H


typedef int DirectionType;

class Direction {
    public:
        static const DirectionType None = 0;
        static const DirectionType UP = 1;
        static const DirectionType DOWN = 2;
        static const DirectionType LEFT = 3;
        static const DirectionType RIGHT = 4;
        int type;
        Direction(DirectionType type=Direction::None): type{type} {}
        void setDirection(DirectionType type=Direction::None) {
            this->type = type;
        }
        bool isNone() {
            return this->type == Direction::None;
        }
        bool isUp() {
            return this->type == Direction::UP;
        }
        bool isDown() {
            return this->type == Direction::DOWN;
        }
        bool isLeft() {
            return this->type == Direction::LEFT;
        }
        bool isRight() {
            return this->type == Direction::RIGHT;
        }
        Direction getTurnLeft() {
            if (this->isUp())
                return Direction{Direction::LEFT};
            if (this->isDown())
                return Direction{Direction::RIGHT};
            if (this->isLeft())
                return Direction{Direction::DOWN};
            if (this->isRight())
                return Direction{Direction::UP};
            return Direction{Direction::None};
        }
        Direction getTurnRight() {
            if (this->isUp())
                return Direction{Direction::RIGHT};
            if (this->isDown())
                return Direction{Direction::LEFT};
            if (this->isLeft())
                return Direction{Direction::UP};
            if (this->isRight())
                return Direction{Direction::DOWN};
            return Direction{Direction::None};
        }
        DirectionType getType() {
            return this->type;
        }
        bool operator==(Direction& direction) {
            return this->type == direction.type;
        }
        bool operator!=(Direction& direction) {
            return !(*this == direction);
        }
};

#endif