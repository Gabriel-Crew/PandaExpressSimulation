#include "ricecooker.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

RiceCooker :: RiceCooker(){
    item = "EMPTY";
    time = 0;
    batch = 0;

};

void RiceCooker::assign_value(string new_item, int new_time, int new_batch){
    item = new_item;
    time = new_time;
    batch = new_batch;
}

void RiceCooker::decrement_time(){
    if(item != "EMPTY" && time > 0){
        time--;
    }
}

bool RiceCooker::is_done(){
    if(item != "EMPTY" && time == 0){
        return true;
    }else{
        return false;
    }
}

bool RiceCooker::is_empty(){
    if(item == "EMPTY"){
        return true;
    }
    else{
        return false;
    } 
}