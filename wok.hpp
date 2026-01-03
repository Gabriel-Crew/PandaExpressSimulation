#ifndef WOK_H
#define WOK_H

#include <string>
using namespace std;


class Wok{
    public:
        Wok();
        void assign_value(int item, int time, int batch);
        void dec();
        bool is_empty();
        int time;
        int item;
        int batch;
        
};
#endif