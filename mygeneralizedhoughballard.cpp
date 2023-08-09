#include "mygeneralizedhoughballard.h"

MyGeneralizedHoughBallard::MyGeneralizedHoughBallard() : MyHough() {}

//Konstruktor využívaný při načítání z resource.
MyGeneralizedHoughBallard::MyGeneralizedHoughBallard(Mat Picture,
                                                     Mat Template) : MyHough(Picture) {
    this->Template = new Mat(Template);
}

//Konstruktor využívaný při načítání externích fotografií.
MyGeneralizedHoughBallard::MyGeneralizedHoughBallard(const string& pathPicture,
                                                     const string& pathTemplate) : MyHough(pathPicture) {
    Template = new Mat(imread(pathTemplate, IMREAD_COLOR));
}

//Destruktor
MyGeneralizedHoughBallard::~MyGeneralizedHoughBallard() {
    if(Template != nullptr) delete Template;
    if(FoundEdgesTemplate != nullptr) delete FoundEdgesTemplate;
    if(GradPictureX != nullptr) delete GradPictureX;
    if(GradPictureY != nullptr) delete GradPictureY;
    if(GradTemplateX != nullptr) delete GradTemplateX;
    if(GradTemplateY != nullptr) delete GradTemplateY;
}

//Práce s obrazem a binárním obrazem hledaného objektu.
Mat& MyGeneralizedHoughBallard::getTemplate() { return *Template; }
Mat& MyGeneralizedHoughBallard::getEdgesTemplate() { return *FoundEdgesTemplate; }

//Detekce hran obrazu a hledaného objektu.
void MyGeneralizedHoughBallard::FindEdges() {

    //Lambda funkce pro převod fotografie na stupně šedi, aplikaci rozmazání a použití Cannyho.
    auto DetectEdges = [&](Mat& input) {
        Mat mat;
        cvtColor(input, mat, COLOR_BGR2GRAY);
        GaussianBlur(mat, mat, Size(5,5), 0);
        Canny(mat, mat, CannyTreshold, 3 * CannyTreshold, 3);
        return new Mat(mat);
    };

    if(!Picture->empty()) FoundEdges = DetectEdges(*Picture);
    if(!Template->empty()) FoundEdgesTemplate = DetectEdges(*Template);
}

//Vypočítání gradientu hran obrazu a hledaného objektu.
void MyGeneralizedHoughBallard::SetupGradients() {

    //Lambda funkce pro převod fotografie na stupně šedi, aplikaci rozmazání a použití Sobelova operátoru.
    auto calculateGradient = [](Mat& input, int dx, int dy){
        Mat mat;
        Mat* output = new Mat();

        cvtColor(input, mat, COLOR_BGR2GRAY);
        GaussianBlur(mat, mat, Size(3,3), 0);
        Sobel(mat, *output, CV_16S, dx, dy, 3);
        return output;
    };

    if(!Picture->empty()) {
        GradPictureX = calculateGradient(*Picture, 1, 0);
        GradPictureY = calculateGradient(*Picture, 0, 1);
    }
    if(!Template->empty()) {
        GradTemplateX = calculateGradient(*Template, 1, 0);
        GradTemplateY = calculateGradient(*Template, 0, 1);
    }
}

//Jelikož je metoda virtuální, tak ji musim deklarovat, i když ji nepoužiji.
void MyGeneralizedHoughBallard::CalculateCosAndSinValues() {}

//Přetížení metody pro kontrolu, jestli se načetl vstupní obraz a hledaný objekt.
bool MyGeneralizedHoughBallard::Exist() {
    if(Picture == nullptr) return false;
    if(Template == nullptr) return false;
    return (!Picture->empty() && !Template->empty());
}

//Přetížení metody pro kontrolu, zdali již byla provedena detekce nebo ne.
bool MyGeneralizedHoughBallard::Generated() {
    if(Picture == nullptr || Template == nullptr || Result == nullptr || Accumulator == nullptr
            || FoundEdges == nullptr || FoundEdgesTemplate == nullptr) return false;
    return true;
}

//Algoritmus detekce obbecných těles v obraze.
void MyGeneralizedHoughBallard::Detect() {
    Mat* Accumulator = new Mat(FoundEdges->rows, FoundEdges->cols, CV_32S, int(0));

    //Lambda funkce pro kontrolu jestli se program nesnaží sáhnout mimo akumulátor.
    auto notOutOfBounds = [&](int x, int y) {
        return x >= 0 && x < Accumulator->cols && y >= 0 && y < Accumulator->rows;
    };

    Point center(FoundEdgesTemplate->cols/2, FoundEdgesTemplate->rows/2);
    vector<vector<Point>>LUT(180,vector<Point>(0));
    /*
     * Naplnění LuT.
     * Pro všechny hrany v binárním obraze objektu se vypočítá vzdálenost hrany od středu center
     * a uloží se do LuT pod hodnotu sklonu hrany (phi).
     */
    for(int i = 0; i < FoundEdgesTemplate->rows; i++) {
        for(int j = 0; j < FoundEdgesTemplate->cols; j++) {
            if(FoundEdgesTemplate->at<uchar>(i,j) == 255 && GradTemplateX->at<short>(i,j) != 0) {
                int phi = 90 + CONVDEG * atan(GradTemplateY->at<short>(i,j) /
                                              (float)GradTemplateX->at<short>(i,j));
                LUT[phi].push_back(Point(center.x - j, center.y - i));
            }
        }
    }

    int max = 0;
    /*
     * Naplnění akumulátoru.
     * Pro všechny hrany v binárním obraze se vypočítá sklon hrany (phi) a akumulátor se
     * inkrementuje všemi body v LuT, které jsou uloženy pod daným sklonem.
     */
    for(int i = 0; i < FoundEdges->rows; i++) {
        for(int j = 0; j < FoundEdges->cols; j++) {
            if(FoundEdges->at<uchar>(i,j) == 255 && GradPictureX->at<short>(i,j) != 0) {
                int phi = 90 + CONVDEG * atan(GradPictureY->at<short>(i,j) /
                                              (float)GradPictureX->at<short>(i,j));
                for(Point Edge: LUT[phi]) {
                    if(notOutOfBounds(Edge.x + j, Edge.y + i)) {

                        Accumulator->at<int>(Edge.y + i, Edge.x + j)++;

                        //Nalezení nejvyšší hodnoty akumulátoru pro logaritmickou jasovou transformaci.
                        max = Accumulator->at<int>(Edge.y + i, Edge.x + j) > max ?
                                    Accumulator->at<int>(Edge.y + i, Edge.x + j) : max;
                    }
                }
            }
        }
    }

    //Konstanta pro logaritmickou jasovou transformaci.
    const float c = 255/(log2f(1 + max));

    Scalar red = Scalar(0,0,255);
    /*
     * Prohledání akumulátoru.
     * Nalezení všech maxim, které překročily práh a vykreslení obdelníku kolem daného středu.
     */
    for(int i = 0; i < Accumulator->rows; i++) {
        for(int j = 0; j < Accumulator->cols; j++) {
            if(Accumulator->at<int>(i,j) > Treshold) {
                Point p1(j + Template->cols/2, i + Template->rows/2),
                      p2(j - Template->cols/2, i - Template->rows/2);
                rectangle(*Result, p1, p2, red, 2, LINE_AA);
            }

            //Logaritmická jasová transformace.
            Accumulator->at<int>(i,j) = c * log2f(1 + Accumulator->at<int>(i,j));
        }
    }

    //Uložení dynamické kopie akumulátoru.
    Mat* mat = new Mat();
    Accumulator->convertTo(*mat, CV_8U);

    delete Accumulator;
    this->Accumulator = mat;
}

