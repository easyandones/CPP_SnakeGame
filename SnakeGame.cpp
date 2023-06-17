#include <chrono>
#include <vector>

#include "Position.hpp"
#include "Direction.hpp"
#include "Snake.hpp"
#include "World.hpp"
#include "Blocks.hpp"
#include "Items.hpp"
#include "RenderEngine.hpp"
#include "Random.hpp"


class SnakeGame {
    private:
        World* world;
        RenderEngine renderEngine;
    public:
        SnakeGame(int worldX, int worldY): world{this->generateWorld(1, worldX, worldY)}, renderEngine{world} {
        }
        virtual ~SnakeGame() {
            delete this->world;
        }
        World* generateWorld(int stage, int worldX, int worldY) {
            return new World{stage, worldX, worldY, Direction{Direction::UP}, std::vector<Position>{Position(worldX / 2, worldY / 2), Position(worldX / 2, worldY / 2 + 1), Position(worldX / 2, worldY / 2 + 2)}};
        }
        void startEventLoop(int eventTick) {
            try {
                this->renderEngine.render();
                const std::chrono::milliseconds tickInterval{eventTick};
                std::chrono::steady_clock::time_point lastEventTick = std::chrono::steady_clock::now();
                while (true) {
                    this->onEveryTick();
                    auto currentTime = std::chrono::steady_clock::now();
                    if (currentTime >= lastEventTick + tickInterval) {
                        lastEventTick = currentTime;
                        this->processEvents();
                    }
                    this->renderEngine.refresh();
                }
            }
            catch(GameOver gameOver) {
                this->renderEngine.showGameOver(gameOver.getReason());
            }
            while (getch() != ERR);
            while (getch() == ERR);
        }
        Direction readInput() {
            int lastInput;
            int temp = ERR;
            do {
                lastInput = temp;
                temp = getch();
            } while (temp != ERR);
            switch (lastInput) {
                case KEY_UP:
                    return Direction{Direction::UP};
                case KEY_DOWN:
                    return Direction{Direction::DOWN};
                case KEY_LEFT:
                    return Direction{Direction::LEFT};
                case KEY_RIGHT:
                    return Direction{Direction::RIGHT};
                default:
                    return Direction{Direction::None};
            }
        }
        void onEveryTick() {
            Direction direction = this->readInput();
            if (!direction.isNone() && this->world->snake.getDirection() != direction) {
                this->world->snake.setDirection(direction);
                this->renderEngine.renderWorld();
            }
            this->renderEngine.renderStats();
        }
        void processEvents() {
            this->world->moveSnake(this->world->snake.getDirection());
            this->world->missions.checkUpdate();
            auto blocks = this->world->getBlocks([](Block* block) {
                Air* air = dynamic_cast<Air*>(block);
                return air != nullptr && air->hasItem();
            });
            for (Block* block : blocks) {
                Air* air = (Air*)block;
                if (air->getItem()->getLifeTime() >= 10) {
                    if (dynamic_cast<GateItem*>(air->getItem()) != nullptr) {
                        if (!this->world->snake.isUsingGate()) {
                            this->world->setBlock(air->getItem()->getPosition(), new ImmuneWall{});
                        }
                    }
                    else {
                        air->removeItem();
                    }
                }
            }
            if (GrowthItem::getItemCount() + PoisonItem::getItemCount() < 3) {
                if (Random::getRandInt(0, 10) == 0)
                    this->world->createItem();
            }
            if (GateItem::getItemCount() == 0) {
                if (Random::getRandInt(0, 10) == 0)
                    this->world->createGates();
            }
            this->renderEngine.render();
            this->world->snake.stats.setProcessed();
            this->world->missions.setProcessed();
            if (this->world->missions.isDone()) {
                World* newWorld = this->generateWorld(this->world->stage + 1, this->world->WORLD_X, this->world->WORLD_Y);
                delete this->world;
                this->world = newWorld;
            }
        }
};

int main() {
    SnakeGame game{21, 21};
    game.startEventLoop(300);
    return 0;
}