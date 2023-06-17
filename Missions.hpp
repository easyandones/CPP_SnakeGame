#ifndef MISSIONS_H
#define MISSIONS_H

#include "Stats.hpp"
#include <ncurses.h>


class Missions {
    private:
        Stats& stats;
        bool dataChanged;
    public:
        const int maxScoreGoal;
        bool maxScoreGoalAccomplished;
        const int growthCountGoal;
        bool growthCountGoalAccomplished;
        const int poisonCountGoal;
        bool poisonCountGoalAccomplished;
        const int gateUseCountGoal;
        bool gateUseCountGoalAccomplished;
        Missions(Stats& stats, int maxScoreGoal, int growthCountGoal, int poisonCountGoal, int gateUseCountGoal): stats{stats}, maxScoreGoal{maxScoreGoal}, maxScoreGoalAccomplished{false}, growthCountGoal{growthCountGoal}, growthCountGoalAccomplished{false}, poisonCountGoal{poisonCountGoal}, poisonCountGoalAccomplished{false}, gateUseCountGoal{gateUseCountGoal}, gateUseCountGoalAccomplished{false}, dataChanged{true} {
        }
        void checkUpdate() {
            if (!stats.hasUpdate())
                return;
            if (!this->maxScoreGoalAccomplished && stats.getMaxScore() == this->maxScoreGoal) {
                this->maxScoreGoalAccomplished = true;
                this->dataChanged = true;
            }
            if (!this->growthCountGoalAccomplished && stats.getGrowthCount() == this->growthCountGoal) {
                this->growthCountGoalAccomplished = true;
                this->dataChanged = true;
            }
            if (!this->poisonCountGoalAccomplished && stats.getPoisonCount() == this->poisonCountGoal) {
                this->poisonCountGoalAccomplished = true;
                this->dataChanged = true;
            }
            if (!this->gateUseCountGoalAccomplished && stats.getGateUseCount() == this->gateUseCountGoal) {
                this->gateUseCountGoalAccomplished = true;
                this->dataChanged = true;
            }
        }
        bool isDone() {
            return this->maxScoreGoalAccomplished && this->growthCountGoalAccomplished && this->poisonCountGoalAccomplished && this->gateUseCountGoalAccomplished;
        }
        bool hasUpdate() {
            return this->dataChanged;
        }
        void setProcessed() {
            this->dataChanged = false;
        }
        
};

#endif