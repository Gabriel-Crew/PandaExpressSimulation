#include "grill.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

Grill :: Grill(){
    item = 99;
    time = 0;
    batch = 0;
};

void Grill::assign_value(int new_item, int new_time, int new_batch){
    item = new_item;
    time = new_time;
    batch = new_batch;
}

bool Grill::time_to_flip(){
    if(time == 3){
        return true;
    }
    return false;
};

void Grill::decrement_time(){
    if(item != 99){
        time--;
    }
}

bool Grill::is_empty(){
    if(item == 99){
        return true;
    }
    else{
        return false;
    } 
}

bool Grill:: containes(int new_item){
    if
      (item == new_item)
    {
        return true;
    }
    else{
        return false;
    }
}