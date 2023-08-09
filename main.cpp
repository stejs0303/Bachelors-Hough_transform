#include "houghtransform.h"
#include "myhoughlines.h"

#include <QApplication>

/*
 * Zkompilovanou verzi aplikace se všemi potřebnými knihovnami nebylo možné odevzdat,
 * protože byla větší než je maximální povolená velikost přílohy. Pro zkompilování bude
 * nutné upravit cesty k používaným knihovnám OpenCV (LIBS a INCLUDEPATH) v souboru
 * HoughovaTransformace.pro.
 *
 * MyHough - obsahuje některé metody a ukazatele využívané všemi třemi technikami detekce.
 *
 * MyHoughLines - implementuje algoritmus detekce přímek a přetěžuje metodu pro vypočítání
 *                sinů a cosinů.
 *
 * MyHoughCircles - implementuje algoritmus detekce kružnic, přidává třírozměrný akumulátor
 *                  gettery a settery pro práci se vstupními parametry, přetěžuje metodu pro
 *                  vypočítání sinů a cosinů a metodu pro vymazání alokované paměti akumulátoru
 *                  a výsledku při překreslení.
 *
 * MyGeneralizedHoughBallard - implementuje algoritmus detekce obecných těles, přidává ukazatele
 *                             na hledaný objekt, přetěžuje metodu pro detekci hran, metodu pro
 *                             vymazání alokované paměti akumulátoru a výsledku při překreslení
 *                             a metodu pro kontorlu zdali detekce již proběhla nebo ne. Dále
 *                             přidává metodu pro detekci gradientu hran a ukazatele na objekty
 *                             vygenerované touto metodou.
 *
 * HoughTransform - obsahuje vše co se týká operací s uživatelským rozhraním.
 */

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    HoughTransform w;

    w.showMaximized();

    return a.exec();
}
