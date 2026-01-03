#ifndef RICEWARMER_H
#define RICEWARMER_H

#include <string>
using namespace std;

class RiceWarmer{
    public:
        RiceWarmer();
        void increase_servings();
        void decrement_servings();
        bool is_low();
        bool has_room();
        int get_servings();
        int servings_of_white;
        int pans_of_basic;
};

#endif