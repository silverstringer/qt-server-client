#include "clientwidget.h"
#include <QtCore>
#include <QApplication>

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);

    RBT::ClientWidget client;
    client.show();
    app.exec();

}
