#ifndef SNAKE_H
#define SNAKE_H

#include <string>

#include "Stats.hpp"
#include "Position.hpp"
#include "Direction.hpp"
#include "Colors.hpp"
#include "Pixel.hpp"


class GameOver {
    private:
        std::string reasonText;
    public:
        GameOver(std::string reasonText): reasonText{reasonText} {
        }
        std::string getReason() {
            return this->reasonText;
        }
};

class SnakeCollisionDetected: public GameOver {
    private:
        Position position;
        Direction direction;
    public:
        SnakeCollisionDetected(Position position, Direction direction): GameOver{"Snake Collision Detected"}, position{position}, direction{direction} {
        }
};

class SnakeDied: public GameOver {
    public:
        SnakeDied(): GameOver{"Snake Died"} {
        }
};

class SnakePart {
    private:
        Position position;
        Direction direction;
        SnakePart* prev;
        SnakePart* next;
        bool inGate;
    public:
        SnakePart(Position position, Direction direction, SnakePart* prev=nullptr, SnakePart* next=nullptr): position{position}, direction{direction}, prev{prev}, next{next}, inGate{false} {
            if (prev != nullptr)
                prev->next = this;
            if (next != nullptr)
                next->prev = this;
        }
        ~SnakePart() {
            delete this->next;
        }
        Position getPosition() {
            return this->position;
        }
        void setDirection(Direction direction) {
            this->direction = direction;
        }
        Direction getDirection() {
            return this->direction;
        }
        SnakePart* getPrev() {
            return this->prev;
        }
        SnakePart* getNext() {
            return this->next;
        }
        bool hasPrev() {
            return this->prev != nullptr;
        }
        bool hasNext() {
            return this->next != nullptr;
        }
        void remove() {
            if (hasPrev())
                this->prev->next = this->next;
            if (hasNext())
                this->next->prev = this->prev;
            delete this;
        }
        bool isHead() {
            return !this->hasPrev();
        }
        void setIsInGate(bool inGate) {
            this->inGate = inGate;
        }
        bool isInGate() {
            return this->inGate;
        }
        Pixel getPixel() {
            std::string text;
            if (this->direction.isUp())
                text = " ^ ";
            else if (this->direction.isDown())
                text = " V ";
            else if (this->direction.isLeft())
                text = " < ";
            else if (this->direction.isRight())
                text = " > ";
            else
                text = " 0 ";
            return Pixel{text, this->isHead() ? Colors::SnakeHead : Colors::Snake};
        }
};

class Snake {
    private:
        SnakePart* head;
    public:
        Stats stats;
        Snake(Direction direction, Position position): head{new SnakePart{position, direction}} {
        }
        virtual ~Snake() {
            delete this->head;
        }
        int getLength() {
            int length = 0;
            for (SnakePart* target = this->head; target != nullptr; target = target->getNext(), length++);
            return length;
        }
        SnakePart* getHead() {
            return this->head;
        }
        SnakePart* getTail() {
            SnakePart* target = this->head;
            for (; target->getNext() != nullptr; target = target->getNext());
            return target;
        }
        void appendFirst(Position position) {
            this->head = new SnakePart{position, this->getDirection(), nullptr, this->head};
            this->stats.setScore(this->getLength());
        }
        void removeLast() {
            if (this->getLength() == 1)
                return;
            this->getTail()->remove();
            this->stats.setScore(this->getLength());
            if (this->getLength() < 3)
                throw SnakeDied{};
        }
        void setDirection(Direction direction) {
            this->getHead()->setDirection(direction);
        }
        Direction getDirection() {
            return this->getHead()->getDirection();
        }
        void move(Direction direction) {
            this->setDirection(direction);
            this->moveTo(this->head->getPosition() + direction);
        }
        void moveTo(Position position) {
            if (this->isOnPosition(position))
                throw SnakeCollisionDetected{position, this->getDirection()};
            this->head = new SnakePart{position, this->getDirection(), nullptr, this->head};
            this->getTail()->remove();
        }
        bool isOnPosition(Position position) {
            for (SnakePart* target = this->head; target != nullptr; target = target->getNext())
                if (target->getPosition() == position)
                    return true;
            return false;
        }
        bool isNearby(Position position) {
            std::vector<Position> positions{position, position + Direction{Direction::UP}, position + Direction{Direction::DOWN}, position + Direction{Direction::LEFT}, position + Direction{Direction::RIGHT}};
            for (Position position : positions) {
                if (this->isOnPosition(position))
                    return true;
            }
            return false;
        }
        bool isUsingGate() {
            for (SnakePart* target = this->head; target != nullptr; target = target->getNext())
                if (target->isInGate())
                    return true;
            return false;
        }
};

#endif