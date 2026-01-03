#include "basket.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

Basket :: Basket(){
    item = 99;
    time = 0;
    batch = 0;
};

void Basket::assign_value(int new_item, int new_time, int new_batch){
    item = new_item;
    time = new_time;
    batch = new_batch;
}

void Basket::dec(){
    if(time != 0){
        time--;
    }
}

bool Basket::is_empty(){
    if(item == 99){
        return true;
    }
    else{
        return false;
    }    
}
