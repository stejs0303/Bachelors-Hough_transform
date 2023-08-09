#pragma once
#include <QMainWindow>
#include <QPixmap>
#include <QImage>
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>

#include "myhoughlines.h"
#include "myhoughcircles.h"
#include "mygeneralizedhoughballard.h"

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class HoughTransform; }
QT_END_NAMESPACE

class HoughTransform : public QMainWindow {
    Q_OBJECT

public:
    HoughTransform(QWidget *parent = nullptr);
    ~HoughTransform();

private slots:
    void on_LinesButton1_clicked();
    void on_LinesButton2_clicked();
    void on_LinesButton3_clicked();
    void on_LinesButtonDraw_clicked();
    void on_LinesCannyTresholdSlider_sliderReleased();
    void on_LinesCannyTresholdSlider_sliderMoved(int position);
    void on_LinesButtonLoadPath_clicked();
    void on_LinesButtonSave_clicked();

    void on_CirclesButton1_clicked();
    void on_CirclesButton2_clicked();
    void on_CirclesButton3_clicked();
    void on_CirclesButtonDraw_clicked();
    void on_CirclesAccumulatorSlider_sliderMoved(int position);
    void on_CirclesCannyTresholdSlider_sliderReleased();
    void on_CirclesCannyTresholdSlider_sliderMoved(int position);
    void on_CirclesButtonLoadPath_clicked();
    void on_CirclesButtonSave_clicked();

    void on_GeneralizedButton1_clicked();
    void on_GeneralizedButton2_clicked();
    void on_GeneralizedButton3_clicked();
    void on_GeneralizedButtonDraw_clicked();
    void on_GeneralizedCannyTresholdSlider_sliderMoved(int position);
    void on_GeneralizedCannyTresholdSlider_sliderReleased();
    void on_GeneralizedButtonLoadTemplatePath_clicked();
    void on_GeneralizedButtonLoadPath_clicked();
    void on_GeneralizedButtonSave_clicked();  

private:
    Ui::HoughTransform *ui;

    MyHoughLines* HoughLines = new MyHoughLines();
    MyHoughCircles* HoughCircles = new MyHoughCircles();
    MyGeneralizedHoughBallard* GeneralizedHough = new MyGeneralizedHoughBallard();
    QString filter = "Obrazky (*.jpg *.jpeg *.png *.bmp) ;; Všechny soubory (*.*) ";

    QPixmap convertToQPix(Mat& input, QImage::Format format);

    bool DoesntExist(MyHough* Hough);

    Mat loadFromQrc(QString qrc);

    void setupLinesSlider();
    void setupCirclesSlider();
    void setupGeneralizedSlider();

    void DisplayPicture(const int transformation);
};
