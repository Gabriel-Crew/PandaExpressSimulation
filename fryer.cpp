#include "fryer.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

Fryer :: Fryer(){
    Big_Basket_1;
    Big_Basket_2;
    Small_Basket_1;
    Small_Basket_2;
    Small_Basket_3;
};

void Fryer:: decrement_time(Basket &basket){
        if(basket.time != 0){
            basket.dec();
        }
};

bool Fryer:: containes(int new_item){
    if
      (Big_Basket_1.item == new_item   || 
       Big_Basket_2.item == new_item   ||
       Small_Basket_1.item == new_item ||
       Small_Basket_2.item == new_item ||
       Small_Basket_3.item == new_item)
    {
        return true;
    }
    else{
        return false;
    }
}