#include <QDebug>

#include "elcushelper.h"

ElcusHelper::ElcusHelper(QObject * parent):
    QObject(parent),
    m_hEvent(0),
    m_hThread(0),
    m_hDevice(0),
    m_SN(0),
    m_index(0)
{

}

ElcusHelper::~ElcusHelper()
{
    finishConnection();
}

void ElcusHelper::setHDevice(HANDLE hDevice)
{
    if(hDevice == INVALID_HANDLE_VALUE)
    {
        qDebug()<<tr("Error in setHDevice function (file %1 in line %2)").arg(__FILE__).arg(__LINE__);
        return;
    }
    m_hDevice = hDevice;
}

HANDLE ElcusHelper::getHDevice() const
{
    return m_hDevice;
}

USHORT ElcusHelper::getSerialNumber() const
{
    return m_SN;
}

void ElcusHelper::setSerialNumber(USHORT serialNumber)
{
    m_SN = serialNumber;
}

void ElcusHelper::setNumberInputChannels(USHORT numberChannels)
{
    m_numberSI = numberChannels;
}

void ElcusHelper::setNumberOutputChannels(USHORT numberChannels)
{
    m_numberSO = numberChannels;
}

void ElcusHelper::setTypeElcus(typeElcus type)
{
    m_type = type;
    switch(type){
        case typeElcus::PCI429_4_1:
            m_numberSI = 8;
            m_numberSO = 8;
            break;
        case typeElcus::PCI429_4_2:
            m_numberSI = 16;
            m_numberSO = 8;
            break;
        case typeElcus::PCI429_4_3:
            m_numberSI = 16;
            m_numberSO = 16;
            break;
    }
}

typeElcus ElcusHelper::getTypeElcus() const
{
    return m_type;
}

void ElcusHelper::getState(USHORT * buffer)
{
    DeviceIoControl(m_hDevice,
                     DRV2K_PCI429_4_getState,
                     NULL,
                     0,
                     buffer,
                     8,
                     &m_nOut,
                     NULL);
}

void ElcusHelper::writeRV(int flagReset)
{
    DeviceIoControl(m_hDevice,
                     DRV2K_PCI429_4_writeRV,
                     &flagReset,
                     0x2,
                     NULL,
                     0,
                     &m_nOut,
                    NULL);
}

void ElcusHelper::writeRM(unsigned short regModule)
{
    DeviceIoControl(m_hDevice,
                     DRV2K_PCI429_4_writeRM,
                     &regModule,
                     0x2,
                     NULL,
                     0,
                     &m_nOut,
                    NULL);
}

USHORT ElcusHelper::readRID()
{
    USHORT id = 0;
    DeviceIoControl(m_hDevice,
                    DRV2K_PCI429_4_readRID,
                    NULL,
                    0,
                    &id,
                    2,
                    &m_nOut,
                    NULL);
    //Из ТО: По снятию сигнала RESET стартует программа модуля,
    //процессор модуля устанавливает режим СБРОС
    //и в течение времени не более 10мс осуществляет
    //программную инициализацию модуля.
    Sleep(100);
    return id;
}

void ElcusHelper::setKSR(int codeKSR)
{
    DeviceIoControl(m_hDevice,
                    DRV2K_PCI429_4_setKCR,
                    &codeKSR,
                    0x2,
                    NULL,
                    0,
                    &m_nOut,
                    NULL);
}

void ElcusHelper::setFreeqChannels(USHORT * buffer)
{
    DeviceIoControl(m_hDevice,
                    DRV2K_PCI429_4_setFreq,
                    buffer,
                    0x40,
                    NULL,
                    0,
                    &m_nOut,
                    NULL);
}

void ElcusHelper::setKCI()
{
    USHORT kodeKCI = 0x7310;
    DeviceIoControl(m_hDevice,
                     DRV2K_PCI429_4_setKCI,
                     &kodeKCI,
                     0x2,
                     NULL,
                     0,
                     &m_nOut,
                     NULL);
    Sleep(10);
    qInfo()<<tr("Module with serial number %1 activated").arg(m_SN);
}

void ElcusHelper::wtiteDataToMemory(uint8_t address, uint32_t value, uint8_t state)
{
    arrayInput[2+m_index*2] = (value << 8) + address;
    arrayInput[3+m_index*2] = 0x8000 | (state << 13) | (value>>8);
    m_index+=1;
}

void ElcusHelper::writeBRV(uint16_t channel)
{
    arrayInput[0] = channel;
    arrayInput[1] = 0x100;
    DeviceIoControl(m_hDevice,
                     DRV2K_PCI429_4_writeBKV,
                     arrayInput,
                     0x404,
                     NULL,
                     0,
                     &m_nOut,
                     NULL);
    USHORT bufOut[4];
    VR_BKV(m_hDevice,arrayInput,m_nOut,channel,m_index,0);
    m_index = 0;
    getState(bufOut);
    if ((bufOut[1]!=0x7310)||(bufOut[0]!=0xbce)||(bufOut[3]))
    {
       qWarning()<<"Ошибка РАБОТА"<<"RESET:\n ОШИБКИ:\n "
                                                  "МР = " + QString::number(bufOut[0]) + "\n" +
                                                  "РМ = " + QString::number(bufOut[1]) + "\n" +
                                                  "ВСК = " + QString::number(bufOut[3]);
       return;
    }
}

void ElcusHelper::finishConnection()
{
    qInfo()<<tr("Module with serial number %1 is disabled").arg(m_SN);
    DeviceIoControl(m_hDevice,
                     DRV2K_PCI429_4_IntRst,
                     NULL,
                     0,
                     NULL,
                     0,
                     &m_nOut,
                     NULL);
    CloseHandle(m_hDevice);
    if(m_hEvent)
        CloseHandle(m_hEvent);
    if(m_hThread)
        CloseHandle(m_hThread);
}
