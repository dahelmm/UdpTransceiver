#ifndef UDPTRANSCEIVERMAIN_H
#define UDPTRANSCEIVERMAIN_H

#include <QMainWindow>

#include "Elcus/elcushelper.h"
#include "Elcus/connecttoelcus.h"

QT_BEGIN_NAMESPACE
namespace Ui { class UdpTransceiverMain; }
QT_END_NAMESPACE

class UdpTransceiverMain : public QMainWindow
{
        Q_OBJECT

    public:
        UdpTransceiverMain(QWidget *parent = nullptr);
        ~UdpTransceiverMain();

    private slots:
        void on_actionConnectToElcus_triggered();
        void on_actionDisconnectFromElcus_triggered();

    private:
        Ui::UdpTransceiverMain *ui;

        ConnectToElcus *m_connectToElcus;
        ElcusHelper *m_elcus;

};
#endif // UDPTRANSCEIVERMAIN_H
