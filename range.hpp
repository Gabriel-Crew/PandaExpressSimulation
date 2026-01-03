#ifndef RANGE_H
#define RANGE_H

#include <string>
#include "wok.hpp"
using namespace std;


class Range{
    public:
        Range();
        void decrement_time(Wok &wok);
        bool containes(int);
        Wok Sides;
        Wok Fried;
        Wok StirFry;
        
};
#endif
