#include "myhoughlines.h"

MyHoughLines::MyHoughLines() : MyHough() {};

//Konstruktor používaný při načítání z resource.
MyHoughLines::MyHoughLines(Mat mat) : MyHough(mat) {
    CalculateCosAndSinValues();
}

//Konstruktor používaný při načítání externích fotografií.
MyHoughLines::MyHoughLines(const string& path) : MyHough(path) {
    CalculateCosAndSinValues();
}

//Destruktor (je volán destruktor ~MyHough())
MyHoughLines::~MyHoughLines() {}

//Předvypočítání sinu a cosinu úhlů <(-90) - 90) po +1°.
void MyHoughLines::CalculateCosAndSinValues() {
    float theta = 0, inc = M_PI / 180;
    for(int i = 0; i < 180; i++) {
        CosValues[i] = cos(theta - M_PI_2);
        SinValues[i] = sin(theta - M_PI_2);
        theta += inc;
    }
}

//Algoritmus detekce přímek v obraze.
void MyHoughLines::Detect() {
    int max = 0;

    float rho = 0, theta = 0;
    float MaxLength = hypot(FoundEdges->rows,FoundEdges->cols);
    Mat* Accumulator = new Mat(180, MaxLength * 2, CV_32S, int(0));
    /*
     * Naplnění akumulátoru.
     * Pro všechny hrany v binárním obraze se do akumulátoru vykreslí sinusoida.
     */
    for(int i = 0; i < FoundEdges->rows; i++) {
        for(int j = 0; j < FoundEdges->cols; j++) {
            if(FoundEdges->at<uchar>(i,j) == 255) {               
                for(int theta = 0; theta < 180; theta++) {

                    rho = cvRound(j * CosValues[theta] + i * SinValues[theta] + MaxLength);

                    Accumulator->at<int>(theta, rho)++;

                    //Nalezení nejvyšší hodnoty akumulátoru pro logaritmickou jasovou transformaci.
                    max = Accumulator->at<int>(theta, rho) > max ?
                                Accumulator->at<int>(theta, rho) : max;
                }
            }
        }
    }

    //Konstanta pro logaritmickou jasovou transformaci.
    const float c = 255/(log2f(1 + max));


    Scalar red(0, 0, 255);
    /*
     * Prohledání naplněného akumulátoru.
     * Pro všechny maxima v akumulátoru, která překročily práh se vypočítají dva body ležící na přímce
     * a ta je následně vykreslena do výsledku.
     */
    for(int i = 0; i < Accumulator->rows; i++) {
        for(int j = 0; j < Accumulator->cols; j++) {
            if(Accumulator->at<int>(i, j) >= Treshold) {

                rho = j - MaxLength;
                theta = (i - 90) * CONVRAD;

                float a = cos(theta), b = sin(theta);
                float x0 = a * rho, y0 = b * rho;

                Point pt1(cvRound(x0 + 10000 * (-b)), cvRound(y0 + 10000 * (a))),
                      pt2(cvRound(x0 - 10000 * (-b)), cvRound(y0 - 10000 * (a)));

                line(*Result, pt1, pt2, red, 2, LINE_AA);
            }

            //Jasová transformace.
            Accumulator->at<int>(i,j) = c * log2f(1 + Accumulator->at<int>(i,j));
        }
    }

    //Převedení akumulátoru na CV_8U a jeho uložení.
    Mat* mat = new Mat();
    Accumulator->convertTo(*mat, CV_8U);

    delete Accumulator;
    this->Accumulator = mat;
}
