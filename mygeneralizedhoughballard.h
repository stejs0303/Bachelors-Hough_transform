#pragma once
#include "myhough.h"

class MyGeneralizedHoughBallard : public MyHough {
private:
    Mat* Template = nullptr;
    Mat* FoundEdgesTemplate = nullptr;

    Mat* GradPictureX = nullptr;
    Mat* GradPictureY = nullptr;
    Mat* GradTemplateX = nullptr;
    Mat* GradTemplateY = nullptr;

public:
    MyGeneralizedHoughBallard();
    MyGeneralizedHoughBallard(Mat Picture, Mat Template);
    MyGeneralizedHoughBallard(const string& pathPicture, const string& pathTemplate);
    ~MyGeneralizedHoughBallard();

    Mat& getTemplate();
    Mat& getEdgesTemplate();

    void SetupGradients();

    virtual bool Generated();
    virtual bool Exist();
    virtual void CalculateCosAndSinValues();
    virtual void FindEdges();
    virtual void Detect();
};
