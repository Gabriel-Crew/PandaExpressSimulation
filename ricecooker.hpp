#ifndef RICECOOKER_H
#define RICECOOKER_H

#include <string>
using namespace std;


class RiceCooker{
    public:
        RiceCooker();
        void assign_value(string item,int time,int batch);
        void clean_ricecooker();
        void decrement_time();
        bool is_empty();
        bool is_done();
        bool is_dirty();
        string item;
        int time;
        int batch;
        
        
};
#endif