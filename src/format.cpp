#include <string>
#include <iostream>

#include "format.h"

using std::string;


// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
    string HH, MM, SS;
    SS = std::to_string(seconds % 60);
    if(SS.size() <2) {SS = "0" + SS;}
    MM = std::to_string((seconds / 60) % 60);
    if(MM.size() <2) {MM = "0" + MM;}
    HH = std::to_string(seconds / 3600);
    if(HH.size() <2) {HH = "0" + HH;}
    return HH + ":" + MM + ":" + SS; 
}