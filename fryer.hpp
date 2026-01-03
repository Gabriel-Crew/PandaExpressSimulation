#ifndef FRYER_H
#define FRYER_H

#include <string>
#include "basket.hpp"
using namespace std;


class Fryer{
    public:
        Fryer();
        void decrement_time(Basket &basket);
        bool containes(int);
        Basket Big_Basket_1;
        Basket Big_Basket_2;
        Basket Small_Basket_1;
        Basket Small_Basket_2;
        Basket Small_Basket_3;
        
};
#endif