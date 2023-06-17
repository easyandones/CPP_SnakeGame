#ifndef ITEMS_H
#define ITEMS_H

#include <chrono>
#include <vector>
#include <algorithm>

#include "Direction.hpp"
#include "Snake.hpp"
#include "Colors.hpp"


class Item {
    protected:
        Position position;
        const std::string text;
        const Color color;
        std::chrono::steady_clock::time_point createdAt;
    public:
        Item(Position position, std::string text, Color color=Colors::Default): position{position}, text{text}, color{color}, createdAt{std::chrono::steady_clock::now()} {
        }
        virtual ~Item() {
        }
        std::chrono::steady_clock::time_point getCreatedAt() {
            return this->createdAt;
        }
        int getLifeTime() {
            return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - this->getCreatedAt()).count();
        }
        Pixel getPixel() {
            return Pixel{this->text, this->color};
        }
        Position getPosition() {
            return this->position;
        }
        virtual void execute(Snake& snake) = 0;
};

class GrowthItem: public Item {
    private:
        static int count;
    public:
        GrowthItem(Position position): Item{position, "+", Colors::Growth} {
            ++this->count;
        }
        virtual ~GrowthItem() {
            --this->count;
        }
        static int getItemCount() {
            return GrowthItem::count;
        }
        void execute(Snake& snake) {
            snake.appendFirst(position);
            snake.stats.setGrowthCount(snake.stats.getGrowthCount() + 1);
        }
};
int GrowthItem::count = 0;

class PoisonItem: public Item {
    private:
        static int count;
    public:
        PoisonItem(Position position): Item{position, "-", Colors::Poison} {
            ++this->count;
        }
        virtual ~PoisonItem() {
            --this->count;
        }
        static int getItemCount() {
            return PoisonItem::count;
        }
        void execute(Snake& snake) {
            snake.moveTo(position);
            snake.removeLast();
            snake.stats.setPoisonCount(snake.stats.getPoisonCount() + 1);
        }
};
int PoisonItem::count = 0;

class GateItem: public Item {
    private:
        static int count;
        GateItem* target;
        std::vector<DirectionType> availableDirections;
    public:
        GateItem(Position position, std::vector<DirectionType> availableDirections): Item{position, "?", Colors::Gate}, target{nullptr}, availableDirections{availableDirections} {
            ++this->count;
        }
        virtual ~GateItem() {
            --this->count;
        }
        static int getItemCount() {
            return GateItem::count;
        }
        void setTarget(GateItem* target) {
            this->target = target;
        }
        bool isDirectionAvailable(Direction direction) {
            return std::find(this->availableDirections.begin(), this->availableDirections.end(), direction.getType()) != this->availableDirections.end();
        }
        void execute(Snake& snake) {
            if (this->target != nullptr) {
                Direction direction = snake.getDirection();
                if (!this->target->isDirectionAvailable(direction)) {
                    if (this->target->availableDirections.size() == 1)
                        direction = this->target->availableDirections[0];
                    else if (this->target->isDirectionAvailable(snake.getDirection().getTurnRight())) {
                        direction = snake.getDirection().getTurnRight();
                    }
                    else {
                        direction = snake.getDirection().getTurnLeft();
                    }
                }
                snake.moveTo(this->target->position + direction);
                snake.setDirection(direction);
                snake.getHead()->setIsInGate(true);
                snake.stats.setGateUseCount(snake.stats.getGateUseCount() + 1);
            }
        }
};
int GateItem::count = 0;

#endif