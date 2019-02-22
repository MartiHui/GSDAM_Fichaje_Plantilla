#include <QCoreApplication>

#include "connectionmanager.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    ConnectionManager::getInstance(1234);

    return a.exec();
}
