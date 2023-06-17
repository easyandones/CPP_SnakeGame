#ifndef RANDOM_H
#define RANDOM_H

#include <random>


class Random {
    public:
        static int getRandInt(int from, int to) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distrib(from, to);
            return distrib(gen);
        }
};

#endif