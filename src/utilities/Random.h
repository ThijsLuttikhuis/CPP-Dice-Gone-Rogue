//
// Created by thijs on 22-04-22.
//

#ifndef GATSP_RANDOM_H
#define GATSP_RANDOM_H

#include <random>

namespace DGR {

class Random {
private:
    static std::mt19937 rng;
    static std::uniform_real_distribution<double> unif;

public:
    /**
    * @brief initialize the random engine with the specified seed
    */
    static void initialize(int seed);

    /**
    * @brief return the random engine
    */
    static std::mt19937 &getRNG();

    /**
    * @brief get a random number between 0.0 and 1.0, 0.0 included, 1.0 excluded
    */
    static double random();

    /**
    * @brief get a random integer between min and max, both min and max included
    */
    static int randInt(int min, int max);

    /**
    * @brief get a random number between min and max, min included, max excluded
    */
    static double random(double min, double max);
};

}


#endif //GATSP_RANDOM_H
