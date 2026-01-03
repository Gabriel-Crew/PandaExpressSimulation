#ifndef GRILL_H
#define GRILL_H

#include <string>
using namespace std;


class Grill{
    public:
        Grill();
        void assign_value(int item,int time,int batch);
        void decrement_time();
        bool time_to_flip();
        bool is_empty();
        bool containes(int new_item);
        int item;
        int time;
        int batch;
        
};
#endif