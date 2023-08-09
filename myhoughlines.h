#pragma once
#include "myhough.h"

class MyHoughLines : public MyHough {

private:
    float CosValues[180];
    float SinValues[180];

public:
    MyHoughLines();
    MyHoughLines(Mat mat);
    MyHoughLines(const string& path);
    ~MyHoughLines();

    virtual void CalculateCosAndSinValues();
    virtual void Detect();
};
