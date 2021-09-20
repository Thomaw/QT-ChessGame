#include <QApplication>
#include "mainwindow.h"

/**
 * @brief qMain
 * @param argc
 * @param argv
 * @return
 *
 * Fonction appelant l'application
 */
int main(int argc, char *argv[]){
    QApplication a(argc, argv); // Lancement de l'application

    MainWindow mw;
    mw.show();

    return a.exec();
}
