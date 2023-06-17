#ifndef STATS_H
#define STATS_H

#include <chrono>


class Stats {
    private:
        int score;
        int maxScore;
        int growthCount;
        int poisonCount;
        int gateUseCount;
        bool dataChanged;
    public:
        Stats(): score{0}, maxScore{this->score}, growthCount{0}, poisonCount{0}, gateUseCount{0}, dataChanged{true} {
        }
        void setScore(int score) {
            if (score != this->score) {
                this->score = score;
                if (this->score > this->maxScore)
                    this->maxScore = this->score;
                this->dataChanged = true;
            }
        }
        int getScore() {
            return this->score;
        }
        int getMaxScore() {
            return this->maxScore;
        }
        void setGrowthCount(int growthCount) {
            if (growthCount != this->growthCount) {
                this->growthCount = growthCount;
                this->dataChanged = true;
            }
        }
        int getGrowthCount() {
            return this->growthCount;
        }
        void setPoisonCount(int poisonCount) {
            if (poisonCount != this->poisonCount) {
                this->poisonCount = poisonCount;
                this->dataChanged = true;
            }
        }
        int getPoisonCount() {
            return this->poisonCount;
        }
        void setGateUseCount(int gateUseCount) {
            if (gateUseCount != this->gateUseCount) {
                this->gateUseCount = gateUseCount;
                this->dataChanged = true;
            }
        }
        int getGateUseCount() {
            return this->gateUseCount;
        }
        bool hasUpdate() {
            return this->dataChanged;
        }
        void setProcessed() {
            this->dataChanged = false;
        }
};

#endif