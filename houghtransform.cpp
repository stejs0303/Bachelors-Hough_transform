#include "houghtransform.h"
#include "ui_houghtransform.h"

HoughTransform::HoughTransform(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HoughTransform) {
    ui->setupUi(this);
}

HoughTransform::~HoughTransform() {
    if(HoughLines->Exist()) delete HoughLines;
    if(HoughCircles->Exist()) delete HoughCircles;
    if(GeneralizedHough->Exist()) delete GeneralizedHough;
    delete ui;
}

//Ukázka 1 - detekce přímek.
void HoughTransform::on_LinesButton1_clicked() {
    if(HoughLines->Exist()) delete HoughLines;

    HoughLines = new MyHoughLines(loadFromQrc(":/LineDetection/Obrazky/sudoku1024-2.jpg"));
    if(DoesntExist(HoughLines)) return;

    HoughLines->setTreshold(200);
    HoughLines->setCannyTreshold(100);
    HoughLines->FindEdges();
    setupLinesSlider();
    DisplayPicture(1);
}

//Ukázka 2 - detekce přímek.
void HoughTransform::on_LinesButton2_clicked() {
    if(HoughLines->Exist()) delete HoughLines;

    HoughLines = new MyHoughLines(loadFromQrc(":/LineDetection/Obrazky/line_det1.jpg"));
    if(DoesntExist(HoughLines)) return;

    HoughLines->setTreshold(180);
    HoughLines->setCannyTreshold(90);
    HoughLines->FindEdges();
    setupLinesSlider();
    DisplayPicture(1);
}

//Ukázka 3 - detekce přímek.
void HoughTransform::on_LinesButton3_clicked() {
    if(HoughLines->Exist()) delete HoughLines;

    HoughLines = new MyHoughLines(loadFromQrc(":/LineDetection/Obrazky/chess1.jpg"));
    if(DoesntExist(HoughLines)) return;

    HoughLines->setTreshold(160);
    HoughLines->setCannyTreshold(100);
    HoughLines->FindEdges();
    setupLinesSlider();
    DisplayPicture(1);
}

//Překreslení a připravení externí fotografie (pokud byla načtena cesta) - detekce přímek.
void HoughTransform::on_LinesButtonDraw_clicked() {
    auto setup = [&] () {
        if(!ui->LinesTreshold->text().isEmpty() &&
            ui->LinesTreshold->text().toInt() >= 20)
                HoughLines->setTreshold(ui->LinesTreshold->text().toInt());
    };

    std::string path = ui->LinesPath->text().toUtf8().constData();
    if(path.empty() && DoesntExist(HoughLines)) return;

    setup();
    if(path.empty()) { HoughLines->Reset(); DisplayPicture(1); return; }

    if(HoughLines->Exist()) delete HoughLines;
    HoughLines = new MyHoughLines(path);
    if(DoesntExist(HoughLines)) return;

    setup();

    ui->LinesPath->setText("");
    HoughLines->setCannyTreshold(100);
    HoughLines->FindEdges();
    setupLinesSlider();
    DisplayPicture(1);
}

//Potenciometr pro překreslení výsledku a akumulátoru přímek při změně hodnoty prahu Cannyho.
void HoughTransform::on_LinesCannyTresholdSlider_sliderReleased() {
    if(DoesntExist(HoughLines)) return;
    HoughLines->Reset();
    DisplayPicture(1);
}

//Potenciometr pro překreslení obrazu hran přímek při změně hodnoty prahu Cannyho.
void HoughTransform::on_LinesCannyTresholdSlider_sliderMoved(int position) {
    if(DoesntExist(HoughLines)) return;
    HoughLines->setCannyTreshold(position);
    HoughLines->FindEdges();
    QPixmap qpixmap = convertToQPix(HoughLines->getEdges(), QImage::Format_Grayscale8);
    ui->LinesEdges->setPixmap(qpixmap.scaled(550, 450, Qt::KeepAspectRatio));
}

//Načítání cesty k souboru - detekce přímek.
void HoughTransform::on_LinesButtonLoadPath_clicked() {
    const QString path = QFileDialog::getOpenFileName(this, "Načíst obrázek", QDir::homePath(), filter);
    ui->LinesPath->setText(path);
}

//Uložení obrázků detekce přímek.
void HoughTransform::on_LinesButtonSave_clicked() {
    QString folder = "C:/Houghova Transformace/Detekce primek";
    QDir mDir;

    if(!mDir.exists(folder)) mDir.mkpath(folder);
    if(HoughLines->Generated()) {
        imwrite(folder.toStdString() + "/obrazek.jpg", HoughLines->getPicture());
        imwrite(folder.toStdString() + "/hrany.png", HoughLines->getEdges());
        imwrite(folder.toStdString() + "/vysledek.jpg", HoughLines->getResult());
        imwrite(folder.toStdString() + "/akumulator.png", HoughLines->getAccumulator());

        QMessageBox::information(this, "Uloženo", "Obrázky byly uloženy do C:/Houghova Transformace/Detekce primek");
    } else {
        QMessageBox::information(this, "Nevygenerováno", "Pokus o uložení nevygenerovaných obrázků.");
    }
}

//Inicializace potenciometru pro Cannyho - detekce přímek.
void HoughTransform::setupLinesSlider() {
    ui->LinesCannyTresholdSlider->setMinimum(20);
    ui->LinesCannyTresholdSlider->setMaximum(170);
    ui->LinesCannyTresholdSlider->setSliderPosition(HoughLines->getCannyTreshold());
}

//Ukázka 1 - detekce kružnic.
void HoughTransform::on_CirclesButton1_clicked() {
    if(HoughCircles->Exist()) delete HoughCircles;

    HoughCircles = new MyHoughCircles(45, 65, loadFromQrc(":/CircleDetection/Obrazky/coins.jpg"), 120);
    if(DoesntExist(HoughCircles)) return;

    HoughCircles->setTreshold(72);
    HoughCircles->setCannyTreshold(100);
    HoughCircles->FindEdges();
    setupCirclesSlider();
    DisplayPicture(2);
}

//Ukázka 2 - detekce kružnic.
void HoughTransform::on_CirclesButton2_clicked() {
    if(HoughCircles->Exist()) delete HoughCircles;

    HoughCircles = new MyHoughCircles(80, 170, loadFromQrc(":/CircleDetection/Obrazky/planets_coliding1.jpg"), 120);
    if(DoesntExist(HoughCircles)) return;

    HoughCircles->setTreshold(40);
    HoughCircles->FindEdges();
    setupCirclesSlider();
    DisplayPicture(2);
}

//Ukázka 3 - detekce kružnic.
void HoughTransform::on_CirclesButton3_clicked() {
    if(HoughCircles->Exist()) delete HoughCircles;

    HoughCircles = new MyHoughCircles(28, 112, loadFromQrc(":/CircleDetection/Obrazky/space1.jpg"), 90);
    if(DoesntExist(HoughCircles)) return;

    HoughCircles->setTreshold(35);
    HoughCircles->FindEdges();
    setupCirclesSlider();
    DisplayPicture(2);
}

//Překreslení a připrava externí fotografie (pokud byla zadána cesta) - detekce kružnic.
void HoughTransform::on_CirclesButtonDraw_clicked() {
    auto setup = [&]() {
        if(!ui->CirclesTreshold->text().isEmpty() &&
            ui->CirclesTreshold->text().toInt() >= 10)
                HoughCircles->setTreshold(ui->CirclesTreshold->text().toInt());
        if(!ui->CirclesMinRadius->text().isEmpty() &&
            ui->CirclesMinRadius->text().toInt() > 0)
                HoughCircles->setMinRadius(ui->CirclesMinRadius->text().toInt());
        if(!ui->CirclesMaxRadius->text().isEmpty() &&
            ui->CirclesMaxRadius->text().toInt() <= 200)
                HoughCircles->setMaxRadius(ui->CirclesMaxRadius->text().toInt());
        if(!ui->CirclesPrecission->text().isEmpty() &&
            ui->CirclesPrecission->text().toInt() >= 20 &&
            ui->CirclesPrecission->text().toInt() <= 180)
                HoughCircles->setNumOfPoints(ui->CirclesPrecission->text().toInt());
    };

    std::string path = ui->CirclesPath->text().toUtf8().constData();
    if(path.empty() && DoesntExist(HoughCircles)) return;

    setup();
    if(path.empty()) { HoughCircles->Reset(); DisplayPicture(2); return; }

    if(HoughCircles->Exist()) delete HoughCircles;

    HoughCircles = new MyHoughCircles(path);
    if(DoesntExist(HoughCircles)) return;

    setup();

    ui->CirclesPath->setText("");
    HoughCircles->FindEdges();
    setupCirclesSlider();
    DisplayPicture(2);
}

//Potenciometr pro prohlížení akumulátoru - detekce kružnic.
void HoughTransform::on_CirclesAccumulatorSlider_sliderMoved(int position) {
    vector<Mat*>* Accumulator = HoughCircles->getAccumulatorVector();
    QPixmap qpixmap = convertToQPix(*Accumulator->at(position), QImage::Format_Grayscale8);
    ui->CirclesAccumulator->setPixmap(qpixmap.scaled(680, 380, Qt::KeepAspectRatio));
}

//Potenciometr pro překreslení výsledku a akumulátoru kružnic při změně prahu Cannyho.
void HoughTransform::on_CirclesCannyTresholdSlider_sliderReleased() {
    if(DoesntExist(HoughCircles)) return;
    HoughCircles->Reset();
    DisplayPicture(2);
}

//Potenciometr pro překreslení detekovaných hran kružnic při změně prahu Cannyho.
void HoughTransform::on_CirclesCannyTresholdSlider_sliderMoved(int position) {
    if(DoesntExist(HoughCircles)) return;
    HoughCircles->setCannyTreshold(position);
    HoughCircles->FindEdges();
    QPixmap qpixmap = convertToQPix(HoughCircles->getEdges(), QImage::Format_Grayscale8);
    ui->CirclesEdges->setPixmap(qpixmap.scaled(680, 380, Qt::KeepAspectRatio));
}

//Načtení cesty k externí fotografii - detekce kružnic.
void HoughTransform::on_CirclesButtonLoadPath_clicked() {
    const QString path = QFileDialog::getOpenFileName(this, "Načíst obrázek", QDir::homePath(), filter);
    ui->CirclesPath->setText(path);
}

//Uložení obrázků detekce kružnic.
void HoughTransform::on_CirclesButtonSave_clicked() {
    QString folder = "C:/Houghova Transformace/Detekce kruznic";
    QDir mDir;

    if(!mDir.exists(folder)) mDir.mkpath(folder);
    if(HoughCircles->Generated()) {
        imwrite(folder.toStdString() + "/obrazek.jpg", HoughCircles->getPicture());
        imwrite(folder.toStdString() + "/hrany.png", HoughCircles->getEdges());
        imwrite(folder.toStdString() + "/vysledek.jpg", HoughCircles->getResult());

        HoughCircles->setAccumulator(ui->CirclesAccumulatorSlider->value());
        imwrite(folder.toStdString() + "/akumulator.png", HoughCircles->getAccumulator());

        QMessageBox::information(this, "Uloženo", "Obrázky byly uloženy do C:/Houghova Transformace/Detekce kruznic");
    } else {
        QMessageBox::information(this, "Nevygenerováno", "Pokus o uložení nevygenerovaných obrázků.");
    }
}

//Inicializace potenciometru Cannyho - detekce kružnic.
void HoughTransform::setupCirclesSlider() {
    ui->CirclesCannyTresholdSlider->setMinimum(20);
    ui->CirclesCannyTresholdSlider->setMaximum(170);
    ui->CirclesCannyTresholdSlider->setSliderPosition(HoughCircles->getCannyTreshold());
}


//Ukázka 1 - detekce obecných těles.
void HoughTransform::on_GeneralizedButton1_clicked() {
    if(GeneralizedHough->Exist()) delete GeneralizedHough;

    GeneralizedHough = new MyGeneralizedHoughBallard(loadFromQrc(":/Generalized/Obrazky/Stop1.jpg"),
                                                     loadFromQrc(":/Generalized/Obrazky/StopTemplate1.jpg"));
    if(DoesntExist(GeneralizedHough)) return;

    GeneralizedHough->setTreshold(100);
    GeneralizedHough->FindEdges();
    GeneralizedHough->SetupGradients();
    setupGeneralizedSlider();
    DisplayPicture(3);
}

//Ukázka 2 - detekce obecných těles.
void HoughTransform::on_GeneralizedButton2_clicked() {
    if(GeneralizedHough->Exist()) delete GeneralizedHough;

    GeneralizedHough = new MyGeneralizedHoughBallard(loadFromQrc(":/Generalized/Obrazky/chessFigure1.jpeg"),
                                                     loadFromQrc(":/Generalized/Obrazky/chessFigureTemplate1-1.jpg"));
    if(DoesntExist(GeneralizedHough)) return;

    GeneralizedHough->setTreshold(40);
    GeneralizedHough->FindEdges();
    GeneralizedHough->SetupGradients();
    setupGeneralizedSlider();
    DisplayPicture(3);
}

//Ukázka 3 - detekce obecných těles.
void HoughTransform::on_GeneralizedButton3_clicked() {
    if(GeneralizedHough->Exist()) delete GeneralizedHough;

    GeneralizedHough = new MyGeneralizedHoughBallard(loadFromQrc(":/Generalized/Obrazky/sudoku400.jpg"),
                                                     loadFromQrc(":/Generalized/Obrazky/sudokuSeven-400.jpg"));
    if(DoesntExist(GeneralizedHough)) return;

    GeneralizedHough->setTreshold(50);
    GeneralizedHough->FindEdges();
    GeneralizedHough->SetupGradients();
    setupGeneralizedSlider();
    DisplayPicture(3);
}

//Překreslit a připravit externí fotografii (pokud byly zadány cesty) - detekce obecných těles.
void HoughTransform::on_GeneralizedButtonDraw_clicked() {
    auto setup = [&]() {
        if(!ui->GeneralizedTreshold->text().isEmpty() &&
            ui->GeneralizedTreshold->text().toInt() >= 5)
                GeneralizedHough->setTreshold(ui->GeneralizedTreshold->text().toInt());
    };

    std::string pathPicture = ui->GeneralizedPath->text().toUtf8().constData();
    std::string pathTemplate = ui->GeneralizedTemplatePath->text().toUtf8().constData();
    if((pathPicture.empty() || pathTemplate.empty()) && DoesntExist(GeneralizedHough)) return;

    setup();
    if((pathPicture.empty() || pathTemplate.empty())) { GeneralizedHough->Reset(); DisplayPicture(3); return; }

    if(GeneralizedHough->Exist()) delete GeneralizedHough;

    GeneralizedHough = new MyGeneralizedHoughBallard(pathPicture, pathTemplate);
    if(DoesntExist(GeneralizedHough)) return;

    setup();
    ui->GeneralizedPath->setText("");
    ui->GeneralizedTemplatePath->setText("");
    GeneralizedHough->FindEdges();
    GeneralizedHough->SetupGradients();
    setupGeneralizedSlider();
    DisplayPicture(3);
}

//Potenciometr pro překreslení výsledku a akumulátoru detekce obecných těles při změně prahu Cannyho.
void HoughTransform::on_GeneralizedCannyTresholdSlider_sliderReleased() {
    GeneralizedHough->Reset();
    DisplayPicture(3);
}

//Potenciometr pro překreslení obrazů hran detekce obecných těles při změně prahu Cannyho.
void HoughTransform::on_GeneralizedCannyTresholdSlider_sliderMoved(int position) {
    if(DoesntExist(GeneralizedHough)) return;
    GeneralizedHough->setCannyTreshold(position);
    GeneralizedHough->FindEdges();

    QPixmap qpixmap = convertToQPix(GeneralizedHough->getEdges(), QImage::Format_Grayscale8);
    ui->GeneralizedOriginalEdges->setPixmap(qpixmap.scaled(550, 380, Qt::KeepAspectRatio));
    qpixmap = convertToQPix(GeneralizedHough->getEdgesTemplate(), QImage::Format_Grayscale8);
    ui->GeneralizedTemplateEdges->setPixmap(qpixmap.scaled(qpixmap.width(), qpixmap.height(), Qt::KeepAspectRatio));
}

//Načtení cesty k obrázku tělesa - detekce těles.
void HoughTransform::on_GeneralizedButtonLoadTemplatePath_clicked() {
    const QString path = QFileDialog::getOpenFileName(this, "Načíst obrázek", QDir::homePath(), filter);
    ui->GeneralizedTemplatePath->setText(path);
}

//Načtení cesty k obrázku - detekce těles.
void HoughTransform::on_GeneralizedButtonLoadPath_clicked() {
    const QString path = QFileDialog::getOpenFileName(this, "Načíst obrázek", QDir::homePath(), filter);
    ui->GeneralizedPath->setText(path);
}

//Uložení obrázků detekce těles.
void HoughTransform::on_GeneralizedButtonSave_clicked() {
    QString folder = "C:/Houghova Transformace/Zobecnena detekce";
    QDir mDir;

    if(!mDir.exists(folder)) mDir.mkpath(folder);
    if(GeneralizedHough->Generated()) {
        imwrite(folder.toStdString() + "/obrazek.jpg", GeneralizedHough->getPicture());
        imwrite(folder.toStdString() + "/objekt.jpg", GeneralizedHough->getTemplate());

        imwrite(folder.toStdString() + "/hrany.png", GeneralizedHough->getEdges());
        imwrite(folder.toStdString() + "/objekt hrany.png", GeneralizedHough->getEdgesTemplate());

        imwrite(folder.toStdString() + "/vysledek.jpg", GeneralizedHough->getResult());
        imwrite(folder.toStdString() + "/akumulator.png", GeneralizedHough->getAccumulator());

        QMessageBox::information(this, "Uloženo", "Obrázky byly uloženy do C:/Houghova Transformace/Zobecnena detekce");
    } else {
        QMessageBox::information(this, "Nevygenerováno", "Pokus o uložení nevygenerovaných obrázků.");
    }
}

//Inicializace potenciometru Cannyho - detekce těles
void HoughTransform::setupGeneralizedSlider() {
    ui->GeneralizedCannyTresholdSlider->setMinimum(20);
    ui->GeneralizedCannyTresholdSlider->setMaximum(170);
    ui->GeneralizedCannyTresholdSlider->setSliderPosition(GeneralizedHough->getCannyTreshold());
}

//--------------------------------------------------------------------------------------------------------------

//Převod Mat na QPixmapu
QPixmap HoughTransform::convertToQPix(Mat& input, QImage::Format format){
    return QPixmap::fromImage(QImage(input.data, input.cols, input.rows, input.step, format));
}

//Kontrola jestli byl vytořen objekt s obrázkem.
bool HoughTransform::DoesntExist(MyHough* Hough) {
    if(Hough->Exist()) return false;
    QMessageBox::warning(this, "Nebyl nalezen obrázek",
                         "Byla zadaná chybná cesta k souboru nebo na obrázek "
                         "nebyl aplikován algoritmus detekce.");
    return true;

}

//Načtení obrázku z qrc.
Mat HoughTransform::loadFromQrc(QString qrc) {
    QFile file(qrc);
    Mat mat;
    if(file.open(QIODevice::ReadOnly)) {
        qint64 sz = file.size();
        std::vector<uchar> buf(sz);
        file.read((char*)buf.data(), sz);
        mat = imdecode(buf, IMREAD_COLOR);
    }
    return mat;
}

//Zpracování obrázku a vyobrazení.
void HoughTransform::DisplayPicture(const int transformation) {
    QPixmap qpixmap;
    switch (transformation) {
        case 1: {
            int MaxWidth = 550;
            int MaxHeight = 450;
            HoughLines->Detect();

            ui->LinesTreshold->setText(QString::number(HoughLines->getTreshold()));

            qpixmap = convertToQPix(HoughLines->getPicture(), QImage::Format_BGR888);
            ui->LinesOriginal->setPixmap(qpixmap.scaled(MaxWidth, MaxHeight, Qt::KeepAspectRatio));
            qpixmap = convertToQPix(HoughLines->getAccumulator(), QImage::Format_Grayscale8);
            ui->LinesAccumulator->setPixmap(qpixmap.scaled(MaxWidth*2.5, 320, Qt::KeepAspectRatio));
            qpixmap = convertToQPix(HoughLines->getEdges(), QImage::Format_Grayscale8);
            ui->LinesEdges->setPixmap(qpixmap.scaled(MaxWidth, MaxHeight, Qt::KeepAspectRatio));
            qpixmap = convertToQPix(HoughLines->getResult(), QImage::Format_BGR888);
            ui->LinesResult->setPixmap(qpixmap.scaled(MaxWidth, MaxHeight, Qt::KeepAspectRatio));
            break;
        }
        case 2: {
            int MaxWidth = 680;
            int MaxHeight = 380;

            HoughCircles->Detect();

            ui->CirclesTreshold->setText(QString::number(HoughCircles->getTreshold()));
            ui->CirclesMinRadius->setText(QString::number(HoughCircles->getMinRadius()));
            ui->CirclesMaxRadius->setText(QString::number(HoughCircles->getMaxRadius()));
            ui->CirclesPrecission->setText(QString::number(HoughCircles->getNumOfPoints()));

            ui->CirclesAccumulatorSlider->setMinimum(0);
            ui->CirclesAccumulatorSlider->setMaximum(HoughCircles->getAccumulatorSize()-1);

            qpixmap = convertToQPix(HoughCircles->getPicture(), QImage::Format_BGR888);
            ui->CirclesOriginal->setPixmap(qpixmap.scaled(MaxWidth, MaxHeight, Qt::KeepAspectRatio));
            HoughCircles->setAccumulator(0);
            qpixmap = convertToQPix(HoughCircles->getAccumulator(), QImage::Format_Grayscale8);
            ui->CirclesAccumulator->setPixmap(qpixmap.scaled(MaxWidth, MaxHeight, Qt::KeepAspectRatio));
            qpixmap = convertToQPix(HoughCircles->getEdges(), QImage::Format_Grayscale8);
            ui->CirclesEdges->setPixmap(qpixmap.scaled(MaxWidth, MaxHeight, Qt::KeepAspectRatio));
            qpixmap = convertToQPix(HoughCircles->getResult(), QImage::Format_BGR888);
            ui->CirclesResult->setPixmap(qpixmap.scaled(MaxWidth, MaxHeight, Qt::KeepAspectRatio));
            break;
        }
        case 3: {
            int MaxWidth = 550;
            int MaxHeight = 380;
            GeneralizedHough->Detect();

            ui->GeneralizedTreshold->setText(QString::number(GeneralizedHough->getTreshold()));

            qpixmap = convertToQPix(GeneralizedHough->getPicture(), QImage::Format_BGR888);
            ui->GeneralizedOriginal->setPixmap(qpixmap.scaled(MaxWidth, MaxHeight, Qt::KeepAspectRatio));
            qpixmap = convertToQPix(GeneralizedHough->getTemplate(), QImage::Format_BGR888);
            ui->GeneralizedTemplate->setPixmap(qpixmap.scaled(qpixmap.width(),qpixmap.height(),Qt::KeepAspectRatio));
            qpixmap = convertToQPix(GeneralizedHough->getEdges(), QImage::Format_Grayscale8);
            ui->GeneralizedOriginalEdges->setPixmap(qpixmap.scaled(MaxWidth, MaxHeight, Qt::KeepAspectRatio));
            qpixmap = convertToQPix(GeneralizedHough->getEdgesTemplate(), QImage::Format_Grayscale8);
            ui->GeneralizedTemplateEdges->setPixmap(qpixmap.scaled(qpixmap.width(), qpixmap.height(), Qt::KeepAspectRatio));
            qpixmap = convertToQPix(GeneralizedHough->getAccumulator(), QImage::Format_Grayscale8);
            ui->GeneralizedAccumulator->setPixmap(qpixmap.scaled(MaxWidth, MaxHeight, Qt::KeepAspectRatio));
            qpixmap = convertToQPix(GeneralizedHough->getResult(), QImage::Format_BGR888);
            ui->GeneralizedResult->setPixmap(qpixmap.scaled(MaxWidth, MaxHeight, Qt::KeepAspectRatio));
            break;
        }
        default: {
            QMessageBox::warning(this, "Chyba", "Chyba při zobrazení obrázku.");
            break;
        }
    }
}
