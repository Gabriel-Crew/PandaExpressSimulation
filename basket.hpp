#ifndef BASKET_H
#define BASKET_H

#include <string>
using namespace std;


class Basket{
    public:
        Basket();
        void assign_value(int item, int time, int batch);
        void dec();
        bool is_empty();
        int item;
        int time;
        int batch;
        
};
#endif