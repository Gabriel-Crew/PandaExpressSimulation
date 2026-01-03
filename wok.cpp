#include "wok.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

Wok :: Wok(){
    item = 99;
    time = 0;
    batch = 0;
};

void Wok::assign_value(int new_item, int new_time, int new_batch){
    item = new_item;
    time = new_time;
    batch = new_batch;
}

void Wok::dec(){
    if(time != 0){
        time--;
    }
}

bool Wok::is_empty(){
    if(item == 99){
        return true;
    }
    else{
        return false;
    }    
}