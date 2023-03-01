#ifndef ELCUSHELPER_H
#define ELCUSHELPER_H

#include <QObject>

#include <windows.h>
#include <setupapi.h>
#include <winioctl.h>

#include "MainElcusDocs/ArincPCI_4.h"
#include "MainElcusDocs/intrfacePCI4.h"
#include "MainElcusDocs/ioctlPCI4.h"


enum typeElcus{
    ///8 входных, 8 выходных каналов
    PCI429_4_1 = 0x4010,
    ///16 входных, 8 выходных каналов
    PCI429_4_2 = 0x4020,
    ///16 входных, 16 выходных каналов
    PCI429_4_3 = 0x4030
};

class ElcusHelper : public QObject
{
    Q_OBJECT
    public:
        ElcusHelper(QObject *parent = nullptr);
        ~ElcusHelper();
        void setHDevice(HANDLE hDevice);
        HANDLE getHDevice() const;
        USHORT getSerialNumber() const;
        void setSerialNumber(USHORT serialNumber);
        void setNumberInputChannels(USHORT numberChannels);
        void setNumberOutputChannels(USHORT numberChannels);
        void setTypeElcus(typeElcus type);
        typeElcus getTypeElcus() const;

        /// Пункт 3.1.3 драйвера - чтение блока состояния модуля
        void getState(USHORT *buffer);
        /// Пункт 3.6.10 драйвера - запись в регистр флагов и вектора прерывания РВ
        void writeRV(int flagReset);
        /// Пункт 3.6.6 драйвера - запись в регистр управления модуля РМ
        void writeRM(unsigned short regModule);
        /// Пункт 3.6.5 драйвера - чтение регистра идентификатора РИД (RESET)
        USHORT readRID();
        /// Пункт 3.2.2  драйвера - задание командного слова режима
        void setKSR(int codeKSR);
        /// Пункт 3.2.1 драйвера - задание кодоа настройки частоты каналов
        void setFreeqChannels(USHORT *buffer);
        /// Пункт 3.2.3 драйвера - переход в режим "Работа" (задание кода КСИ)
        void setKCI();
        /// Записать данные в память
        void wtiteDataToMemory(uint8_t address, uint32_t value, uint8_t state);
        /// Записать полученные данные (записанные функцией wtiteDataToMemory) в выходной буфер (БКВ) и передать
        /// Пункт 3.4.5.1 драйвера - буфер БКВ
        void writeBRV(uint16_t channel);
        /// Закончить работу с платой
        void finishConnection();

    private:
        HANDLE  m_hEvent,
                m_hThread,
                m_hDevice;
        DWORD   m_dwThreadID,
                m_mError;
        ULONG   m_nOut,
                m_interTimer;
        USHORT  m_bufOut[0x400],
                m_bufIn[0x400],
                arrayInput[0x400],
                arrayInputOneTime[4],
                m_numberSI,
                m_numberSO,
                m_adapterPRM[3],
                m_SN;
        typeElcus m_type;
        int m_index;
};

#endif // ELCUSHELPER_H
