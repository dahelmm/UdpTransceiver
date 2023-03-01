#ifndef CONNECTTOELCUS_H
#define CONNECTTOELCUS_H

#include <QDialog>
#include <QAbstractButton>
#include <QMessageBox>
#include <QCloseEvent>

#include <windows.h>
#include <winioctl.h>
#include <setupapi.h>

namespace Ui {
class ConnectToElcus;
}

class ConnectToElcus : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectToElcus(QWidget *parent = nullptr,USHORT serialNumberSettings = 0);
    ~ConnectToElcus();

signals:
    void connectTrue(HANDLE, USHORT);
    void closeWindow();

private slots:
    void on_pB_connect_clicked();
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::ConnectToElcus *ui;
};

#endif // CONNECTTOELCUS_H
