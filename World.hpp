#ifndef WORLD_H
#define WORLD_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

#include "Map.hpp"
#include "Random.hpp"
#include "Direction.hpp"
#include "Snake.hpp"
#include "Missions.hpp"


class World: public Map {
    public:
        int stage;
        std::chrono::steady_clock::time_point startedAt;
        Snake snake;
        Missions missions;
        World(int stage, int xSize, int ySize, Direction direction, std::vector<Position> snakePositions): Map{xSize, ySize}, stage{stage}, startedAt{std::chrono::steady_clock::now()}, snake{direction, snakePositions.back()}, missions{this->snake.stats, Random::getRandInt(7, 10), Random::getRandInt(3, 6), Random::getRandInt(1, 3), Random::getRandInt(3, 6)} {
            for (int i = 0; i < xSize; i++) {
                bool isWallX = i == 0 || i == xSize - 1;
                for (int j = 0; j < ySize; j++) {
                    bool isWallY = j == 0 || j == ySize - 1;
                    if (isWallX && isWallY)
                        this->setBlock(Position(i, j), new Wall{});
                    else if (isWallX || isWallY)
                        this->setBlock(Position(i, j), new ImmuneWall{});
                    else
                        this->setBlock(Position(i, j), new Air{});
                }
            }
            for (int i = snakePositions.size() - 2; i >= 0; i--)
                this->snake.appendFirst(snakePositions[i]);
            this->createRandomImmuneWalls(0);
            this->createRandomImmuneWalls(1);
        }
        void createRandomImmuneWalls(int wallType) {
            int randomLength = Random::getRandInt(4, 8);
            int randomPosA = Random::getRandInt(2, this->WORLD_X - 3 - randomLength);
            int randomPosB = Random::getRandInt(2, this->WORLD_Y - 3);
            for (int i = 0; i < randomLength; i++) {
                if (wallType == 0)
                    this->createRandomImmuneWall(Position{randomPosA + i, randomPosB});
                else
                    this->createRandomImmuneWall(Position{randomPosB, randomPosA + i});
            }
        }
        void createRandomImmuneWall(Position position) {
            if (!this->snake.isNearby(position)) {
                if (dynamic_cast<ImmuneWall*>(this->getBlock(position)) != nullptr)
                    this->setBlock(position, new Wall{});
                else
                    this->setBlock(position, new ImmuneWall{});
            }
        }
        void moveSnake(Direction direction) {
            Position position = this->snake.getHead()->getPosition() + direction;
            Block* block = this->getBlock(position);
            if (block->isPassable()) {
                Air* air = (Air*)block;
                if (air->hasItem()) {
                    air->getItem()->execute(snake);
                    if (dynamic_cast<GateItem*>(air->getItem()) == nullptr)
                        air->removeItem();
                }
                else {
                    this->snake.move(direction);
                }
            }
            else {
                throw SnakeCollisionDetected{position, direction};
            }
        }
        std::vector<Block*> getBlocks(std::function<bool(Block*)> filter) {
            std::vector<Block*> available;
            for (int i = 0; i < this->WORLD_X; i++) {
                for (int j = 0; j < this->WORLD_Y; j++) {
                    Block* block{this->getBlock(Position{i, j})};
                    if (filter(block)) {
                        available.push_back(block);
                    }
                }
            }
            return available;
        }
        std::vector<Position> getAvailablePositions(std::function<bool(Position, Block*, bool)> filter) {
            std::vector<Position> available;
            for (int i = 0; i < this->WORLD_X; i++) {
                for (int j = 0; j < this->WORLD_Y; j++) {
                    Position position{i, j};
                    if (filter(position, this->getBlock(position), this->snake.isOnPosition(position))) {
                        available.push_back(position);
                    }
                }
            }
            return available;
        }
        GateItem* createGate(Position position) {
            std::vector<DirectionType> availableDirections;
            if (dynamic_cast<Air*>(this->getBlock(position + Direction{Direction::UP})) != nullptr) {
                availableDirections.push_back((DirectionType)Direction::UP);
            }
            if (dynamic_cast<Air*>(this->getBlock(position + Direction{Direction::DOWN})) != nullptr) {
                availableDirections.push_back((DirectionType)Direction::DOWN);
            }
            if (dynamic_cast<Air*>(this->getBlock(position + Direction{Direction::RIGHT})) != nullptr) {
                availableDirections.push_back((DirectionType)Direction::RIGHT);
            }
            if (dynamic_cast<Air*>(this->getBlock(position + Direction{Direction::LEFT})) != nullptr) {
                availableDirections.push_back((DirectionType)Direction::LEFT);
            }
            return new GateItem{position, availableDirections};
        }
        void createGates() {
            std::vector<Position> available = getAvailablePositions([](Position position, Block* block, bool isSnakeOnPosition) {
                return dynamic_cast<ImmuneWall*>(block) != nullptr;
            });
            if (available.size() < 2)
                return;
            int index1 = Random::getRandInt(0, available.size() - 1);
            int index2 = Random::getRandInt(0, available.size() - 2);
            if (index2 >= index1)
                ++index2;
            Position pos1 = available[index1];
            Position pos2 = available[index2];
            Air* block1 = new Air{};
            Air* block2 = new Air{};
            GateItem* gate1 = createGate(pos1);
            GateItem* gate2 = createGate(pos2);
            gate1->setTarget(gate2);
            gate2->setTarget(gate1);
            block1->setItem(gate1);
            block2->setItem(gate2);
            this->setBlock(pos1, block1);
            this->setBlock(pos2, block2);
        }
        void createItem() {
            std::vector<Position> available = getAvailablePositions([](Position position, Block* block, bool isSnakeOnPosition) {
                Air* air = dynamic_cast<Air*>(block);
                return air != nullptr && !air->hasItem() && !isSnakeOnPosition;
            });
            if (available.size() == 0)
                return;
            Position position = available[Random::getRandInt(0, available.size() - 1)];
            Air* block = (Air*)this->getBlock(position);
            if (Random::getRandInt(0, 1) == 0)
                block->setItem(new GrowthItem{position});
            else
                block->setItem(new PoisonItem{position});
        }
};

#endif