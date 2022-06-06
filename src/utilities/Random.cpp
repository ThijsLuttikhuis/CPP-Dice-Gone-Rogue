//
// Created by thijs on 22-04-22.
//

#include "Random.h"

namespace DGR {

std::mt19937 Random::rng;
std::uniform_real_distribution<double> Random::unif;

void Random::initialize(int seed) {
    if (seed == 0) {
        std::random_device rd;
        rng = std::mt19937(rd());
    } else {
        rng = std::mt19937(seed);
    }
    unif = std::uniform_real_distribution<double>(0.0, 1.0);
}

std::mt19937 &Random::getRNG() {
    return rng;
}

double Random::random() {
    return unif(rng);
}

int Random::randInt(int min, int max) {
    return (int) (min + (1 + max - min) * random());
}

double Random::random(double min, double max) {
    return min + (max - min) * random();
}

}
