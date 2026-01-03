#include "customer.hpp"
#include <cstdlib>
#include <ctime>
using namespace std;

Customer :: Customer(){
    //'99' is used in the place of '0' to represent an empty item in an order.
    //This is so i can use '0' represent Orange Chicken for easier syntax when accessing arrays
    m_1 = 99;
    m_2 = 99;
    m_3 = 99;
    s = 99;
};

Customer :: Customer(int i, int j, int k, int l){
    m_1 = i;
    m_2 = j;
    m_3 = k;
    s = l;
};