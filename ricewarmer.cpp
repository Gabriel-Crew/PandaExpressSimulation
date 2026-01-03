#include "ricewarmer.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

RiceWarmer :: RiceWarmer(){
    pans_of_basic = 0;

};


void RiceWarmer::increase_servings(){
    
    //number of pans we nomrally put into the rice warmer after making a batch of basic is 4
    pans_of_basic += 4;
    
}

void RiceWarmer::decrement_servings(){
    pans_of_basic--;
}

bool RiceWarmer::is_low(){
    
    if(pans_of_basic <= 4){
        return true;
    }else{
        return false;
    }
}

bool RiceWarmer::has_room(){    
    
    if(pans_of_basic <= 6){
        return true;
    }else{
        return false;
    }  
}

int RiceWarmer::get_servings(){    
   return pans_of_basic;
}