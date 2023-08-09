#include "myhough.h"

MyHough::MyHough() {}

//Konstruktor využíváný při načítání z resource.
MyHough::MyHough(Mat &mat) {
    Picture = new Mat(mat);
    Result = new Mat(Picture->clone());
}

//Konstruktor využíváný při načítání externích fotografií.
MyHough::MyHough(const string& path) {
    Picture = new Mat(imread(path, IMREAD_COLOR));
    Result = new Mat(imread(path, IMREAD_COLOR));
}

//Destruktor
MyHough::~MyHough() {
    if(Picture != nullptr) delete Picture;
    if(FoundEdges != nullptr) delete FoundEdges;
    if(Accumulator != nullptr) delete Accumulator;
    if(Result != nullptr) delete Result;
}

//Práce s prahem detekce.
int MyHough::getTreshold() { return Treshold; }
void MyHough::setTreshold(int Treshold) {
    this->Treshold = Treshold;
}

//Getter a Setter pro práci s prahem Cannyho.
int MyHough::getCannyTreshold() { return CannyTreshold; }
void MyHough::setCannyTreshold(int Treshold) {
    this->CannyTreshold = Treshold;
}

Mat& MyHough::getPicture() { return *Picture; }
Mat& MyHough::getEdges() { return *FoundEdges; }
Mat& MyHough::getAccumulator() { return *Accumulator; }
Mat& MyHough::getResult() { return *Result; }

//Kontrola, jestli se načetl vstupní obraz.
bool MyHough::Exist() {
    if(Picture == nullptr) return false;
    return !Picture->empty();
}

//Kontrola jesli již proběhla detekce.
bool MyHough::Generated() {
    if(Picture == nullptr || Result == nullptr ||
       Accumulator == nullptr || FoundEdges == nullptr) return false;
    return true;
}

/*
 * Detekce hran v obraze.
 * Obraz se převede na stupně šedi, aplikuje se blur a detekují hrany pomocí Cannyho.
 */
void MyHough::FindEdges() {
    FoundEdges = new Mat();
    cvtColor(*Picture, *FoundEdges, COLOR_BGR2GRAY);
    GaussianBlur(*FoundEdges, *FoundEdges, Size(5, 5), 0);
    Canny(*FoundEdges, *FoundEdges, CannyTreshold, 3*CannyTreshold, 3);
}

/*
 * Mazání obrazu akumulátoru a výsledku.
 * Používaný při překreslování - změně vstupních parametrů.
 */
void MyHough::Reset() {
    if(Accumulator != nullptr) { delete Accumulator; Accumulator = nullptr; }
    if(Result != nullptr) delete Result;
    Result = new Mat(Picture->clone());
}
