#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include <chrono>
#include <string>
#include <sstream>
#include <vector>
#include <utility>

#include "Snake.hpp"
#include "Display.hpp"
#include "World.hpp"
#include "Stats.hpp"
#include "Missions.hpp"
#include "Position.hpp"


class RenderEngine {
    private:
        World*& world;
        Display display;
        bool worldUpdated;
        bool statsUpdated;
        bool missionUpdated;
    public:
        RenderEngine(World*& world): world{world}, display{this->world->WORLD_X, this->world->WORLD_Y}, worldUpdated{true}, statsUpdated{true} {
        }
        void render() {
            this->renderWorld();
            this->renderStats();
            this->renderMission();
            this->refresh();
        }
        void renderWorld() {
            for (int i = 0; i < this->world->WORLD_X; i++) {
                for (int j = 0; j < this->world->WORLD_Y; j++) {
                    this->display.world->printPixel(Position(i, j), this->world->getBlock(Position(i, j))->getPixel());
                }
            }
            for (SnakePart* target = this->world->snake.getHead(); target != nullptr; target = target->getNext()) {
                this->display.world->printPixel(target->getPosition(), target->getPixel());
            }
            this->worldUpdated = true;
        }
        void renderStats() {
            int prevSeconds = 0;
            Stats& stats = this->world->snake.stats;
            int seconds = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - this->world->startedAt).count();
            if (prevSeconds == seconds && !stats.hasUpdate())
                return;
            prevSeconds = seconds;
            this->display.stats->clear();
            this->display.stats->printCenter(1, "Score", Colors::Text);
            std::vector<std::ostringstream> outputStrings(6);
            outputStrings[0] << "Stage : " << this->world->stage;
            outputStrings[1] << "Time : " << seconds << "s";
            outputStrings[2] << "B : " << stats.getScore() << " / " << stats.getMaxScore();
            outputStrings[3] << "+ : " << stats.getGrowthCount();
            outputStrings[4] << "- : " << stats.getPoisonCount();
            outputStrings[5] << "G : " << stats.getGateUseCount();
            for (int i = 0; i < outputStrings.size(); i++) {
                this->display.stats->print(Position{1, 3 + i}, outputStrings[i].str(), Colors::Text);
            }
            this->statsUpdated = true;
        }
        void renderMission() {
            Missions& missions = this->world->missions;
            if (!missions.hasUpdate())
                return;
            this->display.mission->clear();
            this->display.mission->printCenter(1, "Mission", Colors::Text);
            std::vector<std::pair<std::ostringstream, bool>> outputStrings(4);
            outputStrings[0].first << "B : " << missions.maxScoreGoal;
            outputStrings[0].second = missions.maxScoreGoalAccomplished;
            outputStrings[1].first << "+ : " << missions.growthCountGoal;
            outputStrings[1].second = missions.growthCountGoalAccomplished;
            outputStrings[2].first << "- : " << missions.poisonCountGoal;
            outputStrings[2].second = missions.poisonCountGoalAccomplished;
            outputStrings[3].first << "G : " << missions.gateUseCountGoal;
            outputStrings[3].second = missions.gateUseCountGoalAccomplished;
            for (int i = 0; i < outputStrings.size(); i++) {
                outputStrings[i].first << "(" << (outputStrings[i].second ? "O" : "X") << ")";
                this->display.mission->print(Position{1, 3 + i}, outputStrings[i].first.str(), outputStrings[i].second ? Colors::Text_Green : Colors::Text_Red);
            }
            this->missionUpdated = true;
        }
        void showGameOver(std::string reason) {
            this->render();
            this->display.stats->printCenter(9, "Game Over!", Colors::Text_Red);
            this->display.stats->printCenter(10, reason, Colors::Text_Red);
            this->display.stats->printCenter(11, "Press any key to exit", Colors::Text_Red);
            this->display.stats->refresh();
        }
        void refresh() {
            if (!this->worldUpdated && !this->statsUpdated && !this->missionUpdated)
                return;
            if (this->worldUpdated)
                this->display.world->refresh();
            if (this->statsUpdated)
                this->display.stats->refresh();
            if (this->missionUpdated)
                this->display.mission->refresh();
            this->display.frame->refresh();
            this->worldUpdated = false;
            this->statsUpdated = false;
            this->missionUpdated = false;
        }
};

#endif