#include "udptransceivermain.h"
#include "ui_udptransceivermain.h"

UdpTransceiverMain::UdpTransceiverMain(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::UdpTransceiverMain),
      m_connectToElcus(nullptr),
      m_elcus(nullptr)
{
    ui->setupUi(this);
}

UdpTransceiverMain::~UdpTransceiverMain()
{
    delete ui;
}


void UdpTransceiverMain::on_actionConnectToElcus_triggered()
{
    if(!m_elcus)
        m_elcus = new ElcusHelper(this);
    m_connectToElcus = new ConnectToElcus(this);
}

void UdpTransceiverMain::on_actionDisconnectFromElcus_triggered()
{

}

