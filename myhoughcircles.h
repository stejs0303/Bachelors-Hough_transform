#pragma once
#include "myhough.h"

class MyHoughCircles : public MyHough {
private:
    int pointsOnCircle = 120;
    int MinRadius = 20;
    int MaxRadius = 100;

    float* CosValues = nullptr;
    float* SinValues = nullptr;
    vector<Mat*>* AccumulatorVector = nullptr;

public:
    MyHoughCircles();
    MyHoughCircles(const string& path);
    MyHoughCircles(int MinRadius, int MaxRadius, Mat mat, int pointsOnCircle);

    ~MyHoughCircles();

    void setNumOfPoints(int pointsOnCircle);
    void setMinRadius(int MinRadius);
    void setMaxRadius(int MaxRadius);
    void setAccumulator(int position);

    int getNumOfPoints();
    int getMinRadius();
    int getMaxRadius();
    int getAccumulatorSize();
    vector<Mat*>* getAccumulatorVector();

    virtual void Reset();
    virtual void CalculateCosAndSinValues();
    virtual void Detect();
};

