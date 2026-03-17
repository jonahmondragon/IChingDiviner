#include "mainwindow.h"

#include <QApplication>
#include<QDir>
#include "constants.h"
#include<QIcon>

int main(int argc, char *argv[])
{

    QCoreApplication::setApplicationName("IChingDiviner");
    QCoreApplication::setOrganizationName("Alamahant");
    QCoreApplication::setApplicationVersion("1.0.0");

    QDir().mkpath(Constants::appDirPath);
    QDir().mkpath(Constants::decksDirPath);
    QDir().mkpath(Constants::saveDirPath);

    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/io.github.alamahant.IChingDiviner.png"));
    MainWindow w;
    w.show();
    return a.exec();
}
