#include "range.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

Range :: Range(){
    Sides;
    Fried;
    StirFry;
};

void Range:: decrement_time(Wok &wok){
        if(wok.time != 0){
            wok.dec();
        }
};

bool Range:: containes(int new_item){
    if
      (Sides.item == new_item   || 
       Fried.item == new_item   ||
       StirFry.item == new_item )
    {
        return true;
    }
    else{
        return false;
    }
}