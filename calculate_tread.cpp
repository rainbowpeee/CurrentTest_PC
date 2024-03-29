﻿#include "calculate_tread.h"

Calculate_Tread::Calculate_Tread(QObject *parent, USB_HID *hid, ComData *comD) : QThread ()
{
    qDebug("计算线程create: %d", this->currentThreadId());
    m_UsbHid = hid;
    m_ComData = comD;
    isStop = false;
}

Calculate_Tread::~Calculate_Tread()
{
     qDebug() << "Calculate_Tread析构函数。";
}

void Calculate_Tread::run()
{
    qDebug("计算线程run: %d", this->currentThreadId());
    qint64 CurDifferT_A = 0;
    qint64 CurDifferT_V = 0;
    qint64 emitCnt = 0;
    qint64 workingHour = 0, workingMinter = 0, workingSecond = 0;
    while (!isStop) {

        try {
            if(m_ComData->T1_Cur_Index > 0 && m_ComData->T1_Cur_Index <= m_ComData->layer_currentIndex &&
                    m_ComData->T2_Cur_Index > 0 && m_ComData->T2_Cur_Index <= m_ComData->layer_currentIndex &&
                    CurDifferT_A != m_ComData->T2_Cur_Index - m_ComData->T1_Cur_Index)
            {
                CurDifferT_A = m_ComData->T2_Cur_Index - m_ComData->T1_Cur_Index;

                double sunCurrent = 0;
                double maxCurrent = 0;
                double minCurrent = 10000;  // 最大10A

                if(CurDifferT_A == 0)
                    emit signalUpdateCurAverage(0, m_ComData->layer_dataSeriesA[m_ComData->T2_Cur_Index - 1], m_ComData->layer_dataSeriesA[m_ComData->T2_Cur_Index - 1], m_ComData->layer_dataSeriesA[m_ComData->T2_Cur_Index - 1]);
                else if(CurDifferT_A > 0) {
                    for(qint64 i = m_ComData->T1_Cur_Index; i < m_ComData->T2_Cur_Index; i++)
                    {
                        sunCurrent += m_ComData->layer_dataSeriesA[i - 1];
                        if( m_ComData->layer_dataSeriesA[i - 1] > maxCurrent)
                            maxCurrent = m_ComData->layer_dataSeriesA[i - 1];
                        if( m_ComData->layer_dataSeriesA[i - 1] < minCurrent)
                            minCurrent = m_ComData->layer_dataSeriesA[i - 1];
                    }
                    if(CurDifferT_A != 0)
                        sunCurrent = sunCurrent / CurDifferT_A;
                    emit signalUpdateCurAverage(CurDifferT_A, sunCurrent, maxCurrent, minCurrent);
                } else if(CurDifferT_A < 0) {
                    CurDifferT_A = 0 - CurDifferT_A;
                    for(qint64 i = m_ComData->T2_Cur_Index; i < m_ComData->T1_Cur_Index; i++)
                    {
                        sunCurrent += m_ComData->layer_dataSeriesA[i - 1];
                        if( m_ComData->layer_dataSeriesA[i - 1] > maxCurrent)
                            maxCurrent = m_ComData->layer_dataSeriesA[i - 1];
                        if( m_ComData->layer_dataSeriesA[i - 1] < minCurrent)
                            minCurrent = m_ComData->layer_dataSeriesA[i - 1];
                    }
                    if(CurDifferT_A != 0)
                        sunCurrent = sunCurrent / CurDifferT_A;
                    emit signalUpdateCurAverage(CurDifferT_A, sunCurrent, maxCurrent, minCurrent);
                }

            }
        } catch (...) {
            qDebug() << "计算电流平均值异常";
        }

        try {
            if(m_ComData->T1_Vol_Index > 0 && m_ComData->T1_Vol_Index <= m_ComData->layer_currentIndex &&
                    m_ComData->T2_Vol_Index > 0 && m_ComData->T2_Vol_Index <= m_ComData->layer_currentIndex &&
                    CurDifferT_V != m_ComData->T2_Vol_Index - m_ComData->T1_Vol_Index)
            {
                CurDifferT_V = m_ComData->T2_Vol_Index - m_ComData->T1_Vol_Index;

                double sunVoltage = 0;
                double maxVoltage = 0;
                double minVoltage = 7.5;  // 最大7.5V

                if(CurDifferT_V == 0)
                    emit signalUpdateVolAverage(0, m_ComData->layer_dataSeriesV[m_ComData->T2_Vol_Index - 1], m_ComData->layer_dataSeriesV[m_ComData->T2_Vol_Index - 1], m_ComData->layer_dataSeriesV[m_ComData->T2_Vol_Index - 1]);
                else if (CurDifferT_V > 0) {
                    for(qint64 i = m_ComData->T1_Vol_Index; i < m_ComData->T2_Vol_Index; i++)
                    {
                        sunVoltage += m_ComData->layer_dataSeriesV[i - 1];
                        if( m_ComData->layer_dataSeriesV[i - 1] > maxVoltage)
                            maxVoltage = m_ComData->layer_dataSeriesV[i - 1];
                        if( m_ComData->layer_dataSeriesV[i - 1] < minVoltage)
                            minVoltage = m_ComData->layer_dataSeriesV[i - 1];
                    }
                    sunVoltage = sunVoltage / CurDifferT_V;
                    emit signalUpdateVolAverage(CurDifferT_V, sunVoltage, maxVoltage, minVoltage);
                } else if (CurDifferT_V < 0) {
                    CurDifferT_V = 0 - CurDifferT_V;
                    for(qint64 i = m_ComData->T2_Vol_Index; i < m_ComData->T1_Vol_Index; i++)
                    {
                        sunVoltage += m_ComData->layer_dataSeriesV[i - 1];
                        if( m_ComData->layer_dataSeriesV[i - 1] > maxVoltage)
                            maxVoltage = m_ComData->layer_dataSeriesV[i - 1];
                        if( m_ComData->layer_dataSeriesV[i - 1] < minVoltage)
                            minVoltage = m_ComData->layer_dataSeriesV[i - 1];
                    }
                    sunVoltage = sunVoltage / CurDifferT_V;
                    emit signalUpdateVolAverage(CurDifferT_V, sunVoltage, maxVoltage, minVoltage);
                }

            }
        } catch (...) {
            qDebug() << "计算电压平均值异常";
        }
        emitCnt++;
        if(emitCnt >= 10)
        {
            emitCnt = 0;
            qint64 buf = m_ComData->RunningCount / 60000;
            workingHour = buf / 60;
            workingMinter = buf % 60;
            workingSecond = (m_ComData->RunningCount % 60000) / 1000;
            emit signalUpdateWorkingTime(workingHour, workingMinter, workingSecond);
        }
        this->msleep(100);
    }
    emit end_Thread();      // 发送信号
    this->quit();
    this->wait();
}
