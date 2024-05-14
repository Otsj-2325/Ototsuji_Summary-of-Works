#pragma once
#include"texPolygon.h"

class GOAL:public PTEX{
private:
    PTEX* target;
    bool goalf=false;

public:
    void getTarget(PTEX* t){ target=t;}
    void updateData();
    bool getGoalf(){return goalf;}
};