#include "udptransceivermain.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UdpTransceiverMain w;
    w.show();
    return a.exec();
}
