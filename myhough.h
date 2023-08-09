#pragma once
#include <opencv2/opencv.hpp>
#include <cmath>
#include <vector>
#include <iostream>

#define CONVRAD (M_PI/180)
#define CONVDEG (180/M_PI)

using namespace cv;
using namespace std;

class MyHough {
protected:
    Mat* Picture = nullptr;
    Mat* FoundEdges = nullptr;
    Mat* Accumulator = nullptr;
    Mat* Result = nullptr;

    int Treshold = 100;
    int CannyTreshold = 50;

public:
    MyHough();
    MyHough(Mat &mat);
    MyHough(const string& path);
    virtual ~MyHough();

    void setTreshold(int TRESHOLD);
    int getTreshold();

    void setCannyTreshold(int Treshold);
    int getCannyTreshold();

    Mat& getPicture();
    Mat& getEdges();
    Mat& getAccumulator();
    Mat& getResult();

    virtual void Reset();
    virtual bool Exist();
    virtual bool Generated();
    virtual void FindEdges();
    virtual void CalculateCosAndSinValues() = 0;
    virtual void Detect() = 0;
};

