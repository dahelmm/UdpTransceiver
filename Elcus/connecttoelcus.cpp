#include "connecttoelcus.h"

#include <QDebug>

extern HANDLE OpenDeviceBySN(USHORT SerialNumber, PDWORD pError );

ConnectToElcus::ConnectToElcus(QWidget * parent, USHORT serialNumberSettings) :
    QDialog(parent),
    ui(new Ui::ConnectToElcus)
{
    ui->setupUi(this);
    if(serialNumberSettings)
        ui->lE_serNumber->setText(QString::number(serialNumberSettings));
}

ConnectToElcus::~ConnectToElcus()
{
    delete ui;
}

void ConnectToElcus::on_pB_connect_clicked()
{
    DWORD m_error;
    USHORT SN = ui->lE_serNumber->text().toInt();
    HANDLE hDevice = OpenDeviceBySN(SN,&m_error);
    if(hDevice == INVALID_HANDLE_VALUE)
    {
        QMessageBox::critical(this,"Ошибка", "Неверный серийный номер платы, либо плата не подключена, попробуйте снова");
        return;
    }
    emit connectTrue(hDevice, SN);
    this->close();
}

void ConnectToElcus::closeEvent(QCloseEvent * event)
{
    emit closeWindow();
    event->accept();
}
