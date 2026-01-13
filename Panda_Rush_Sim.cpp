#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>
#include <vector>
#include "customer.hpp"
#include "fryer.hpp"
#include "range.hpp"
#include "dishrack.hpp"
#include "grill.hpp"
#include "ricecooker.hpp"
#include "ricewarmer.hpp"

void cook(int,int);

//This is so that I can use 0 to represent orange chicken for easier use with indexing arrays
int EMPTY = 99; 

//For printing usuage
float NUM_CUSTOMERS = 0;
float NUM_MAINS = 0;
float NUM_SIDES = 0;
int items_total[15] = {0};
int mains_total[11] = {0};
int sides_total[4] = {0};
//^^^^^^

enum Mains{
    OrangeChicken,
    Teriyaki,
    BeijingBeef,
    BroccoliBeef,
    KungPaoChicken,
    MushroomChicken,
    SesameChicken,
    WalnutShrimp,
    Steak,
    StringBean,
    Tofu
};

enum Sides{
    ChowMein = 11,
    FriedRice,
    SuperGreens,
    WhiteRice
};

enum Dish{
    Chance,
    ServingsLeft
};

enum Batch{
    Half,
    One,
    Two
};

enum Recipe{
    CookTime,
    Servings
};

Fryer fryer;
Range range;
Dishrack dishrack;
Grill grill;
RiceCooker ricecooker;
RiceWarmer ricewarmer;

int cook_times[15][3][2] = {{{7, 6},{8, 12},{8, 24}},  //Orange Chicken #0
                            {{5, 4},{5, 8},{5, 16}},   //Teriyaki       #1
                            {{6, 2},{6, 4},{6, 8}},    //Beijing Beef   #2
                            {{2, 3},{2, 5},{3, 10}},   //Broccoli Beef  #3
                            {{2, 3},{2, 6},{3, 12}},   //Kung-Pao       #4
                            {{2, 2},{2, 5},{3, 9}},    //Mushroom       #5
                            {{3, 2},{3, 4},{3, 8}},    //Sesame         #6
                            {{4, 2},{4, 4},{4, 9}},    //Walnut Shrimp  #7
                            {{3, 2},{3, 4},{4, 7}},    //Steak          #8
                            {{2, 2},{2, 4},{3, 8}},    //String bean    #9
                            {{5, 2},{5, 3},{5, 5}},    //Tofu}          #10
                            {{4,18},{5,27},{6,36}},    //Chow Mein      #11
                            {{3,10},{4,15},{4,21}},    //Fried Rice     #12
                            {{1,4},{2,8},{3,12}},      //Super Greens   #13
                            {{45,30},{45,45},{45,60}}  //White Rice     #14
                        };

//int menu_array[15][2] = {{40,0},{55,0},{62,0},{69,0},{76,0},{81,0},{86,0},{91,0},{94,0},{97,0},{100,0},{40,0},{80,0},{90,0},{100,0}};
int menu_array[15][2] = {{33,0},{49,0},{58,0},{66,0},{74,0},{80,0},{86,0},{91,0},{94,0},{97,0},{100,0},{40,0},{80,0},{90,0},{100,0}};

Customer test_array[10] = {{1,2,3,4},{},{},{},{},{},{},{},{},{}};

std::vector<std::tuple<int,int>> fryer_backlog;
std::vector<std::tuple<int,int>> wok_backlog;
std::vector<int> waiting_items;
std::vector<int> time_between_m1;
//maybe put this all in a .h? ^^^^^^^^

/*////////////////////////////////////////////////////////////
PRINT FUNCTIONS
*/////////////////////////////////////////////////////////////
void print_waiting_items(int line){
    std::cout<<"Waiting Items: ";
    for(int i = 0; i < waiting_items.size(); i++){
        std::cout<<waiting_items[i]<<", ";
    }
    std::cout<<endl;
    std::cout<<"Line: ";
    for(int i = 0; i < line; i++){
        std::cout<<" | ";
    }
    std::cout<<endl;
}

void fryer_range_backlog_check_print(int line){
    std::cout<<"Fryers: "<<"["<<fryer.Big_Basket_1.item<<":"<<fryer.Big_Basket_1.time<<"]"
                         <<"["<<fryer.Big_Basket_2.item<<":"<<fryer.Big_Basket_2.time<<"]  "
                         <<"["<<fryer.Small_Basket_1.item<<":"<<fryer.Small_Basket_1.time<<"]"
                         <<"["<<fryer.Small_Basket_2.item<<":"<<fryer.Small_Basket_2.time<<"]"
                         <<"["<<fryer.Small_Basket_3.item<<":"<<fryer.Small_Basket_3.time<<"]"<<endl;
    std::cout<<"Range : "<<"("<<range.Sides.item<<","<<range.Sides.time<<") "
                        <<" ("<<range.Fried.item<<","<<range.Fried.time<<") "
                        <<" ("<<range.StirFry.item<<","<<range.StirFry.time<<")"<<endl;
    std::cout<<"Grill : "<<"[  "<<grill.item<<" , "<<grill.time<<"  ]"<<endl;
    std::cout<<"Fryer Backlog: ";
    for(int i = 0; i<fryer_backlog.size();i++){
        std::cout<<std::get<0>(fryer_backlog[i])<<", ";
    }
    std::cout<<endl;
    std::cout<<"Wok Backlog: ";
    for(int i = 0; i<wok_backlog.size();i++){
        std::cout<<std::get<0>(wok_backlog[i])<<", ";
    }
    std::cout<<endl;
    std::cout<<"clean Dishes "<<dishrack.CleanSmall<<" "<<dishrack.CleanBig<<endl;
    print_waiting_items(line);
    std::cout<<endl;
    std::cout<<endl;
    for(int i = 0; i < time_between_m1.size() - 1;i++){
        cout<<"["<<time_between_m1[i] - time_between_m1[i+1]<<"], "; // print time between batches of chow mein are being made. I dont think we are going through enough to be realistic
    }
}

void print_steam_table(){
    for(int i = 0; i < 11; i++){
        std::cout<<"("<<menu_array[i][Dish::ServingsLeft]<<")";
    }
    std::cout<<endl;
     for(int i = 11; i<15; i++){
        std::cout<<"("<<menu_array[i][Dish::ServingsLeft]<<")";
    }
    std::cout<<endl;

    cout<<"Total customers served: "<< NUM_CUSTOMERS << endl;

    cout<<"Rice Warmer: ["<<ricewarmer.get_servings()<<"]"<< "("<< menu_array[14][Dish::ServingsLeft]<<")"<<endl;

    cout<<"Rice Cooker: [("<< ricecooker.time <<"|"<< ricecooker.item <<")]"<<endl;
}

void print_line(Customer *line){
    int i = 0;
    while (line[i].s != 0)
    {
        std::cout<<"Customer ";
        std::cout<<i+1<<endl;
        std::cout<<line[i].m_1<<endl;
        std::cout<<line[i].m_2<<endl;
        std::cout<<line[i].m_3<<endl;
        std::cout<<line[i].s<<endl;
        std::cout<<endl;
        i++;
    }
}

void print_time(int time){
    int hours = 10 + (time/60);
    int minutes = time%60;


    if(hours < 12){
        cout<<"Time is: "<<hours<<":"<<minutes<<" AM"<<endl;
    }
    else{
        cout<<"Time is: "<<hours - 12<<":"<<minutes<<" PM"<<endl;
    }
}
/*////////////////////////////////////////////////////////////
BOH KITCHEN HELP FUNCTIONS
*/////////////////////////////////////////////////////////////
void clear_basket(Basket &basket){
    basket.time = 0;
    basket.item = EMPTY; 
    basket.batch = 0; 
}

void sauce_item(Basket &basket){
    if(range.Fried.is_empty()){
        range.Fried.assign_value(basket.item, basket.time, basket.batch);
        clear_basket(basket);
    }
    else{
        basket.time++;  //hold in fryer until wok is ready
    }
}

bool is_basket_up(Basket basket){
    if(basket.time == 1){
        return true;
    }
    return false;
}

void check_fryer(){
    if(is_basket_up(fryer.Big_Basket_1)){
        sauce_item(fryer.Big_Basket_1);
        fryer.Big_Basket_2 = fryer.Big_Basket_1;
    }
    if(is_basket_up(fryer.Small_Basket_1)){
        sauce_item(fryer.Small_Basket_1);
    }
    if(is_basket_up(fryer.Small_Basket_2)){
        sauce_item(fryer.Small_Basket_2);
    }
    if(is_basket_up(fryer.Small_Basket_3)){
        sauce_item(fryer.Small_Basket_3);
    }
}

void wash_dishes(){
    //Dishes += 2 small 1 big 
    /**/
    if(ricecooker.is_dirty()){
        ricecooker.clean_ricecooker();
    }else{
        if(dishrack.CleanSmall < 40){
            dishrack.CleanSmall += 2;
            if(dishrack.CleanSmall > 40){
                dishrack.CleanSmall = 40;
            }
        }
        if(dishrack.CleanBig < 15){
            dishrack.CleanBig += 1;
            if(dishrack.CleanBig > 15){
                dishrack.CleanBig = 15;
            }
        }
    }
    /*
    if(dishrack.CleanSmall < 40){
            dishrack.CleanSmall += 2;
            if(dishrack.CleanSmall > 40){
                dishrack.CleanSmall = 40;
            }
        }
        if(dishrack.CleanBig < 15){
            dishrack.CleanBig += 1;
            if(dishrack.CleanBig > 15){
                dishrack.CleanBig = 15;
            }
        }*/
    
}

void flip_teri(){
    if(grill.time_to_flip()){
        if(!range.Sides.is_empty()){
            range.Sides.time++;
        }
    }
}

void drop_rice(){

    if(menu_array[Sides::WhiteRice][Dish::ServingsLeft] < 20){
        ricecooker.assign_value("White",cook_times[Sides::WhiteRice][2][Recipe::CookTime],2);
    }else if(ricewarmer.has_room()){
        ricecooker.assign_value("Basic",cook_times[Sides::WhiteRice][2][Recipe::CookTime],2);
    }
    
}

void pan_out_rice(){
    if(ricecooker.item == "Basic"){
        ricewarmer.increase_servings();
        ricecooker.assign_value("DIRTY",0,3);
    }else if(ricecooker.item == "White"){
        menu_array[14][Dish::ServingsLeft] = 60;
        ricecooker.assign_value("DIRTY",0,3);
    }
}

void BoH_duties(){
    check_fryer();
    flip_teri();

    //This decides what the BoH working on sides is doing for the current cycle(minute).
    if(range.Sides.is_empty()){
        if(ricecooker.is_done()){
            pan_out_rice();
        }else if(ricecooker.is_empty()){
            drop_rice();
        }else{
            wash_dishes();
        }
    }else if(dishrack.CleanBig == 0 || dishrack.CleanSmall == 0){
        wash_dishes();
        range.Sides.time++;
    }    
    
}

/*////////////////////////////////////////////////////////////
COOKING SECONDARY FUNCTIONS
*/////////////////////////////////////////////////////////////

//Helper function to determine number of empty baskets
int get_num_empty_baskets(){
    int num_empty_baskets = 0;
    if(fryer.Big_Basket_1.is_empty()){
        num_empty_baskets++;
    }
    if(fryer.Small_Basket_1.is_empty()){
        num_empty_baskets++;
    }
    if(fryer.Small_Basket_2.is_empty()){
        num_empty_baskets++;
    }
    if(fryer.Small_Basket_3.is_empty()){
        num_empty_baskets++;
    }
    return num_empty_baskets;
}

//returns true if item is an entree
bool is_mains(int item){
    if(item < 11){
        return true;
    }
    return false;
}

//returns true if item is stir-fry entree
bool is_stir_fry(int item){
    if(item == Mains::BroccoliBeef || item == Mains::KungPaoChicken || item == Mains::MushroomChicken || 
       item == Mains::Steak        || item == Mains::StringBean     || item == Sides::SuperGreens){
            return true;
    }
    return false;
    
}

//returns true if item is a fried entree
bool is_fried(int item){
    int fried_items[5] = {Mains::OrangeChicken, Mains::BeijingBeef, Mains::SesameChicken, Mains::WalnutShrimp, Mains::Tofu};
    for(int i = 0; i < 5; i++){
        if(item == fried_items[i]){
            return true;
        }
    }
    return false;
}

//decrements dish count if dish is available. If not, hold item in wok until a dish is ready
bool grab_clean_dish(Wok &wok){
    if(wok.item == Mains::OrangeChicken || wok.item == Sides::ChowMein || wok.item == Sides::FriedRice){
        if(dishrack.CleanBig > 0){
            dishrack.CleanBig--;
            return 1;
        }
        wok.time++;
        return 0;
    }
    else{
        if(dishrack.CleanSmall > 0){
            dishrack.CleanSmall --;
            return 1;
        }
        wok.time++;
        return 0;
    }
}

//decrements dish count if dish is available. If not, hold item on grill until a dish is ready
bool grab_clean_teri_dish(){
    if(dishrack.CleanSmall > 0){
        dishrack.CleanSmall --;
        return 1;
    }
    grill.time++;
    return 0;
}

//helper function to determine if an item is already in the backlog
bool in_backlog(int item){
    for(int i = 0; i < fryer_backlog.size(); i++){
        if (std::get<0>(fryer_backlog[i]) == item)
        {
            return true;
        }
    }
    for(int i = 0; i < wok_backlog.size(); i++){
        if (std::get<0>(wok_backlog[i]) == item)
        {
            return true;
        }
    }
    return false;
}

//Helper function to check if a given main item is working
bool is_working(int item){
    if(fryer.containes(item) || range.containes(item) || in_backlog(item) || grill.containes(item)){
        return true;
    }
    return false;
}

//Serves the waiting customers before serving the line
void serve_waiting_customers(int item){
    for(int i = 0; i < waiting_items.size(); i++){
        if (waiting_items[i] == item){
            if(menu_array[item][Dish::ServingsLeft] > 0){ 
                menu_array[item][Dish::ServingsLeft]--;
                waiting_items.erase(waiting_items.begin()+i);
                i--;
            }
            //else if(Dish::ServingsLeft == 0 && !is_working())
        }
    }    
}

//check if any wok timers are at 0
void is_wok_food_ready(Wok &wok){
    if(wok.time == 0 && !wok.is_empty() && grab_clean_dish(wok)){
        menu_array[wok.item][Dish::ServingsLeft] += cook_times[wok.item][wok.batch][Recipe::Servings];
        serve_waiting_customers(wok.item);
        wok.item = EMPTY; 
        wok.batch = 0; 
    }
} 

//check if grill timer is at 0
void is_grill_food_ready(){
    if(grill.time == 0 && !grill.is_empty() && grab_clean_teri_dish()){
        menu_array[grill.item][Dish::ServingsLeft] += cook_times[grill.item][grill.batch][Recipe::Servings];
        serve_waiting_customers(grill.item);
        grill.item = EMPTY; 
        grill.batch = 0; 
    }
}

/*////////////////////////////////////////////////////////////
BEGIN COOKING FUNCTIONS
*/////////////////////////////////////////////////////////////

void start_fried(int item,int batch_number){
    if(item == Mains::OrangeChicken){
        if(fryer.Big_Basket_1.is_empty() && fryer.Big_Basket_2.is_empty()){
            fryer.Big_Basket_1.assign_value(item,cook_times[item][batch_number][Recipe::CookTime],batch_number);
            fryer.Big_Basket_2.assign_value(item,cook_times[item][batch_number][Recipe::CookTime],batch_number);
        }
        else{
            fryer_backlog.push_back(std::make_tuple(item,batch_number));
        }

    }
    else{
        if(fryer.Small_Basket_1.is_empty()){
            fryer.Small_Basket_1.assign_value(item, cook_times[item][batch_number][Recipe::CookTime], batch_number);
        }
        else if(fryer.Small_Basket_2.is_empty()){
            fryer.Small_Basket_2.assign_value(item, cook_times[item][batch_number][Recipe::CookTime], batch_number);
        }
        else if(fryer.Small_Basket_3.is_empty()){
            fryer.Small_Basket_3.assign_value(item, cook_times[item][batch_number][Recipe::CookTime],batch_number);
        }
        else{
            fryer_backlog.push_back(std::make_tuple(item,batch_number));
        }
    }
}

void start_stir_fry(int item, int batch_number){
    if(range.StirFry.is_empty()){
        range.StirFry.assign_value(item,cook_times[item][batch_number][Recipe::CookTime],batch_number);
    }
    else{
        wok_backlog.push_back(std::make_tuple(item,batch_number));
    } 
}

void start_sides(int item, int batch_number){
    if(range.Sides.is_empty()){
        if(item == Sides::FriedRice){
            if(ricewarmer.pans_of_basic > 0){
                range.Sides.assign_value(item,cook_times[item][batch_number][Recipe::CookTime],batch_number);
                ricewarmer.decrement_servings();
            }else{
                wok_backlog.push_back(std::make_tuple(item,batch_number));
            }
        }else{
            range.Sides.assign_value(item,cook_times[item][batch_number][Recipe::CookTime],batch_number);
        }
    }
    else{
        wok_backlog.push_back(std::make_tuple(item,batch_number));
    }
}

void start_teri(int item, int batch_number){
    grill.assign_value(item,cook_times[item][batch_number][Recipe::CookTime],batch_number);
}
//Function that facilitates 'cooking' given the specific food item. It will add the given item to the fryer or to a queue for items waiting to be cooked
void cook(int item, int batch_number){
    //Have the different items use different min servings 
    if(is_mains(item)){
        if(is_fried(item)){
            start_fried(item, batch_number);
        }
        else if(is_stir_fry(item)){
            start_stir_fry(item, batch_number);
        }
        else{
            start_teri(item,batch_number);
        }
    }
    else{
        if(item == ChowMein || item == FriedRice){
            start_sides(item, batch_number);
        }
        else if(item == SuperGreens){
            start_stir_fry(item, batch_number);
        }
    }
   
}

/*////////////////////////////////////////////////////////////
BACKLOG FUNCTIONS
*/////////////////////////////////////////////////////////////

void check_backlog_wok(){
    if(wok_backlog.size()>0){
        for(int i = 0; i < wok_backlog.size(); i++){
            if(is_stir_fry(std::get<0>(wok_backlog[i]))){
                if(range.StirFry.is_empty()){
                    cook(std::get<0>(wok_backlog[i]),std::get<1>(wok_backlog[i]));
                    wok_backlog.erase(wok_backlog.begin()+i);
                    i--;
                }
            }
            else{
                if(range.Sides.is_empty()){
                    start_sides(std::get<0>(wok_backlog[i]),std::get<1>(wok_backlog[i]));
                    wok_backlog.erase(wok_backlog.begin()+i);
                    i--;
                }
            } 
        }
    }
}

void check_backlog_fryer(){
    int x = get_num_empty_baskets();
    for(int i = 0; i < x; i++){
        if(fryer_backlog.size() > 0 ){
            //Since Orange can only be fried in the large baskets, it must be confirmed that one of the empty baskets is the large one.
            if(std::get<0>(fryer_backlog[0]) == Mains::OrangeChicken){
                if(fryer.Big_Basket_1.is_empty()){
                    cook(std::get<0>(fryer_backlog[0]), std::get<1>(fryer_backlog[0]));
                    fryer_backlog.erase(fryer_backlog.begin()+0);
                }
                //if the large basket is still in use, then the next item in the backlog will be cooked
                else if(fryer_backlog.size() > 1){
                    cook(std::get<0>(fryer_backlog[1]), std::get<1>(fryer_backlog[1]));
                    fryer_backlog.erase(fryer_backlog.begin()+1);
                }
            }
            else{
                //This ensures that the only empty basket isn't a large basket(since they are reserved for Orange)
                if(get_num_empty_baskets() == 1 && fryer.Big_Basket_1.is_empty()){
                //do nothing
                }
                else if(fryer_backlog.size() > 0){
                    cook(std::get<0>(fryer_backlog[0]), std::get<1>(fryer_backlog[0]));
                    fryer_backlog.erase(fryer_backlog.begin()+0);
                }
            }
        }
    }
}

/*////////////////////////////////////////////////////////////
DECREMENT COOKING TIMER
*/////////////////////////////////////////////////////////////

//Reduces the time left for each item in the fryer and woks
void food_time_decrement(){
    fryer.decrement_time(fryer.Big_Basket_1);
    fryer.decrement_time(fryer.Big_Basket_2);
    fryer.decrement_time(fryer.Small_Basket_1);
    fryer.decrement_time(fryer.Small_Basket_2);
    fryer.decrement_time(fryer.Small_Basket_3);
    range.decrement_time(range.Sides);
    range.decrement_time(range.Fried);
    range.decrement_time(range.StirFry);
    grill.decrement_time();
    ricecooker.decrement_time();
}

/*////////////////////////////////////////////////////////////
STEAM TABLE FUCNTIONS
*/////////////////////////////////////////////////////////////

//Checks the given portion in a plate to see if it contains and item. 
//Then reduce the items servings and if that item is low on servings then call for it to be cooked
void scoop_n_check_servings(int min_servings, int item, int batch_number, int time){
    if(item != EMPTY && menu_array[item][Dish::ServingsLeft] != 0){
        menu_array[item][Dish::ServingsLeft]--;
        if(menu_array[item][Dish::ServingsLeft] <= min_servings && !is_working(item)){
            if(item == ChowMein){
                time_between_m1.push_back(time);
            }
            cook(item,batch_number);
        }
    }
    else if(item != EMPTY && menu_array[item][Dish::ServingsLeft] == 0){
        waiting_items.push_back(item);
        if(!is_working(item)){
            if(item == ChowMein){
                time_between_m1.push_back(time);
            }
            cook(item, batch_number);
        }
    }

}

void grab_food(){   
    is_wok_food_ready(range.Sides);
    is_wok_food_ready(range.Fried);
    is_wok_food_ready(range.StirFry);
    is_grill_food_ready();
    check_backlog_fryer();
    check_backlog_wok();
}

void fill_order(Customer order, int min_servings, int time){       //Have this interact with line length to determine batch size
    int batch_number = 2;
    scoop_n_check_servings(min_servings,order.m_1,batch_number,time);
    scoop_n_check_servings(min_servings,order.m_2,batch_number,time);
    scoop_n_check_servings(min_servings,order.m_3,batch_number,time);
    scoop_n_check_servings(min_servings,order.s,batch_number,time);
   
}

void fill_steam_table(){
    for(int i = 0; i < 15; i++){
        menu_array[i][Dish::ServingsLeft] = cook_times[i][Batch::One][Recipe::Servings];
    }
    ricewarmer.increase_servings();
}


/*////////////////////////////////////////////////////////////
GENERATE RANDOM NUMBER FUNCTION
*/////////////////////////////////////////////////////////////
int gen_rand_num(int x, int y){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(x,y);
    return(dis(gen));
}


/*////////////////////////////////////////////////////////////
GENERATING PLATE FUNCTIONS
*/////////////////////////////////////////////////////////////

int gen_main(){
    int x = gen_rand_num(0,100);
    int y;
    for(int i = 0; i <= 10; i++){
        if(x <= menu_array[Mains::OrangeChicken][Dish::Chance]){
            y = 0;
        }
        else if(x <= menu_array[i][Dish::Chance] && x > menu_array[i-1][Dish::Chance]){
            y = i;
        }
    }
    return(y);

}

int gen_side(){
    int x = gen_rand_num(0,100);
    int y;
    for(int i = 11; i <= 14; i++){
        if(x <= menu_array[Sides::ChowMein][Dish::Chance]){
            y = Sides::ChowMein;
        }
        else if(x <= menu_array[i][Dish::Chance] && x > menu_array[i-1][Dish::Chance]){
            y = i;
        }
    }
    return(y);
}


/*////////////////////////////////////////////////////////////
CUSTOMER OPERATION FUNCTIONS
*/////////////////////////////////////////////////////////////

Customer gen_rand_customer(){

    int x = gen_rand_num(0,10);
    
    //Plate -- 2 mains and 1 side
    if(x >= 1 && x <= 4){
        Customer customer(gen_main(),gen_main(),EMPTY,gen_side());
        return(customer);
    }

    //Bowl -- 1 main and 1 side
    else if(x >= 5 && x <= 8){
        Customer customer(gen_main(),EMPTY,EMPTY,gen_side());
        return(customer);
    }

    //Bigger Plate -- 3 mains and 1 side
    else{
        Customer customer(gen_main(),gen_main(),gen_main(),gen_side());
        return(customer);
    }
}


void remove_customer(Customer *line, int &back_of_line){
   
    for(int i = 0; i < back_of_line; i++){
        line[i] = line[i+1];
    }
    if(back_of_line != 0){
        back_of_line--;
    }
}


/*////////////////////////////////////////////////////////////
LINE OPERATION FUNCTIONS
*/////////////////////////////////////////////////////////////


void add_for_print(Customer customer){
 NUM_CUSTOMERS++;
 if(customer.m_1 != EMPTY){
    mains_total[customer.m_1]++;
    NUM_MAINS++;
 }
 if(customer.m_2 != EMPTY){
    mains_total[customer.m_2]++;
    NUM_MAINS++;
 }
 if(customer.m_3 != EMPTY){
    mains_total[customer.m_3]++;
    NUM_MAINS++;
 }
 if(customer.s != EMPTY){
    sides_total[customer.s - 11]++;
    NUM_SIDES++;
 }

 for(int i = 0; i < 11; i++){
    if(mains_total[i] != 0){
        int res = (mains_total[i]/NUM_MAINS)*100;
        std::cout<<"("<<res<<"),";
    }
    else{
        std::cout<<"(0|),";
    }
 }
 std::cout<<endl;
 for(int i = 0; i < 4; i++){
    if(sides_total[i] != 0){
        int res = (sides_total[i]/NUM_SIDES)*100;
        std::cout<<"("<<res<<"),";
    }
    else{
        std::cout<<"(0|),";
    }
 }
 std::cout<<endl;
}

/*
* This will generate the line that will be dealth with during each cycle. It will generate a random number 0-1
* and create that many Customer objects that will then added to the line.
*/
void gen_line(int rush_mult, Customer *line, int &back_of_line){
    //add print so that I can make sure that each item is being chosen with the expected frequency
    int num_of_customers = gen_rand_num(0,2) * rush_mult;
    std::cout<<"Customers Generated: "<<num_of_customers<<endl;
    for(int i = 0; i < num_of_customers; i++){
        NUM_CUSTOMERS++;
        line[back_of_line] = gen_rand_customer();
        //print for test
        //add_for_print(line[back_of_line]);
        back_of_line++;
    }
}

//This function adds newly cooked food to the steam table and serves the next customer in line.
void serve_line(Customer *line, int num_of_FOH, int &back_of_line, int min_servings,int time){
    grab_food();
    for(int i = 0; i < (num_of_FOH*1); i++){
        fill_order(line[0], min_servings,time);
        remove_customer(line,back_of_line); // Need to add a seperate queue for waiting customers
    }
}

/*////////////////////////////////////////////////////////////
RUSH MULT FUNCTION
*/////////////////////////////////////////////////////////////

//This function determines what the rush multiplier will be at any given moment. This is determining by a given range that is set in main
int determine_rush_mult(int time){
    
    int rush_mult = 1;
    if(time % 60 > 55 or time % 60 < 20){ //HOURLY RUSH
        rush_mult += 1;
        if(time > 60 && time < 360)//BIG RUSH
            rush_mult += 1;
            if(time > 120 && time < 300)//PEAK RUSH
                rush_mult += 1;
    }
    return rush_mult;
}

/*////////////////////////////////////////////////////////////
MAIN SIM LOGIC FUNCTION
*/////////////////////////////////////////////////////////////

void panda_sim(int print_period, int day, int l_rush_start, int l_rush_end, int d_rush_start, int d_rush_end, Customer *line){
    
    int back_of_line = 0;
    int num_of_FOH = 3;
    int min_servings = 6;

    //starts the day with a full steam table
    fill_steam_table();

    for (int time = 0; time < 600; time++){
        //Decrements the sim timer to begin the next minute of the sim
        food_time_decrement();
        print_time(time);
        int rush_mult = determine_rush_mult(time);
        //takes in the rush_mult and the current line to create a new line. Also created the customer object for later use.
        gen_line(rush_mult, line, back_of_line);
        //this function is where the customer is served, portions decremented, and food is called
        serve_line(line,num_of_FOH,back_of_line, min_servings,time);
        //this function makes sure that all back of house duties are being taken care of when there is time (dishes,rice)
        BoH_duties();

        //print functions for testing
        print_steam_table();
        fryer_range_backlog_check_print(back_of_line);
    }
}

int main(){
    std::cout << "Welcome to the Panda Express BOH Simulator"<< endl;

    int print_period = 10, day = 600; //Simulation will be running at 10 periods(minutes) per second for a day (600 minutes)
    int l_rush_start = 100, l_rush_end = 280; // begining and end times for the lunch rush. this will add a multiplier to business
    int d_rush_start = 430, d_rush_end = 550; // begining and end times for dinner rush. Functions the same as lunch rush
    Customer line[1024];

    srand((time(0)));
    
    panda_sim(print_period,day,l_rush_start,l_rush_end, d_rush_start,d_rush_end,line);
    return 0;
}