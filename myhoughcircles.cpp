#include "myhoughcircles.h"

MyHoughCircles::MyHoughCircles() : MyHough() { CalculateCosAndSinValues(); }

//Konstruktor využívaný při načítání externí fotografie.
MyHoughCircles::MyHoughCircles(const string& path) : MyHough(path) {
    CalculateCosAndSinValues();
}

//Konstruktoru používaný při načítání ukázek z resource.
MyHoughCircles::MyHoughCircles(int MinRadius,
                               int MaxRadius,
                               Mat mat,
                               int pointsOnCircle) : MyHough(mat) {

    this->pointsOnCircle = pointsOnCircle;
    this->MinRadius = MinRadius;
    this->MaxRadius = MaxRadius;
    CalculateCosAndSinValues();
}

//Destruktor
MyHoughCircles::~MyHoughCircles() {
    if(CosValues != nullptr) delete[] CosValues;
    if(SinValues != nullptr) delete[] SinValues;
    while(!AccumulatorVector->empty()) {
        delete AccumulatorVector->back();
        AccumulatorVector->pop_back();
    }
    Accumulator = nullptr;
}

//Getter a Setter pro počet dobů vykreslovaných pro každou kružnici.
int MyHoughCircles::getNumOfPoints() { return pointsOnCircle; }
void MyHoughCircles::setNumOfPoints(int pointsOnCircle) {
    this->pointsOnCircle = pointsOnCircle;
    CalculateCosAndSinValues();
}

//Getter a Setter pro nastavení minimálního poloměru.
int MyHoughCircles::getMinRadius() { return MinRadius; }
void MyHoughCircles::setMinRadius(int MinRadius) {
    this->MinRadius = MinRadius;
}

//Getter a Setter pro nastavení maximálního poloměru.
int MyHoughCircles::getMaxRadius() { return MaxRadius; }
void MyHoughCircles::setMaxRadius(int MaxRadius) {
    this->MaxRadius = MaxRadius;
}

//Nastavení ukazatele na jeden obraz akumulátoru.
void MyHoughCircles::setAccumulator(int position) {
    Accumulator = AccumulatorVector->at(position);
}

int MyHoughCircles::getAccumulatorSize() {
    return AccumulatorVector->size();
}

vector<Mat*>* MyHoughCircles::getAccumulatorVector() {
    return AccumulatorVector;
}

//Předvypočítání sinu a cosinu úhlů <0-pointsOnCircle) po +2pi/pointsOnCircle.
void MyHoughCircles::CalculateCosAndSinValues() {
    if(CosValues != nullptr) {
        delete[] CosValues;
        delete[] SinValues;
    }
    CosValues = new float[pointsOnCircle];
    SinValues = new float[pointsOnCircle];

    double theta = 0, inc = 2*M_PI/pointsOnCircle;
    for (int i = 0; i < pointsOnCircle; i++) {
        CosValues[i] = cos(theta);
        SinValues[i] = sin(theta);
        theta += inc;
    }
}

/*
 * Přetížení metody pro mazání obrazu akumulátoru a výsledku.
 * Používaný při překreslování - změně vstupních parametrů.
 */
void MyHoughCircles::Reset() {
    while(!AccumulatorVector->empty()) {
        delete AccumulatorVector->back();
        AccumulatorVector->pop_back();
    }
    AccumulatorVector = nullptr;
    Accumulator = nullptr;
    if(Result != nullptr) delete Result;
    Result = new Mat(Picture->clone());
}

//Algoritmus detekce kružnic v obraze.
void MyHoughCircles::Detect() {

    /*
     * Uložení souřadnic hran.
     * (Zrychlení detekce, není třeba procházet celý obraz pro každý poloměr.)
     */
    vector<Point> VectorOfFoundEdges;
    for(int i = 0; i < FoundEdges->rows; i++) {
        for(int j = 0; j < FoundEdges->cols; j++) {            
            if(FoundEdges->at<uchar>(i, j) == 255) {
                VectorOfFoundEdges.push_back(Point(j, i));
            }
        }
    }

    int x, y;

    // Rovnice pro jasovou transformaci obrazu v závislosti na množství vykreslovaných bodů.
    //y = 405,66x^(-0,901)
    float brighten = 405.66 * pow(pointsOnCircle, -0.901) + 1;

    vector<Mat*>* AccumulatorVector = new vector<Mat*>(0);

    //Algoritmus.
    for(int r = MinRadius; r < MaxRadius; r++) {
        Mat Accumulator = Mat::zeros(FoundEdges->rows, FoundEdges->cols, CV_8U);

        /*
         * Naplnění akumulátoru pro poloměr r.
         * Pro všechny nalezené hrany se do akumulátoru vykreslí kružnice o určitém počtu bodů.
         */
        for (Point edge: VectorOfFoundEdges) {
            for(int i = 0; i < pointsOnCircle; i++) {
                y = edge.y - cvRound(r * CosValues[i]);
                x = edge.x - cvRound(r * SinValues[i]);
                if(y >= 0 && y < Accumulator.rows && x >= 0 && x < Accumulator.cols)
                    Accumulator.at<uchar>(y, x)++;
            }
        }

        Scalar red(0, 0, 255), purple(255, 0, 255);

        //Detekce středů kružnic v naplněném akumulátoru o poloměru r.
        for(int i = 0; i < Accumulator.rows; i++) {
            for(int j = 0; j < Accumulator.cols; j++) {
                if(Accumulator.at<uchar>(i, j) >= Treshold){
                    circle(*Result, Point(j, i), 1, purple, 2, LINE_AA);
                    circle(*Result, Point(j, i), r, red, 2, LINE_AA);
                }

                //Zvýšení jasu obrazu akumulátoru.
                Accumulator.at<uchar>(i, j) *= brighten;
            }
        }

        //Vytvoření kopie akumulátoru a uložení této kopie do vektoru.
        AccumulatorVector->push_back(new Mat(Accumulator));
    }
    this->AccumulatorVector = AccumulatorVector;
}
