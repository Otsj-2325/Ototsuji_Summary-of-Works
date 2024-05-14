#include"goal.h"

void GOAL::updateData(){
    if(checkCollision(*target)){
        goalf=true;
    }
}