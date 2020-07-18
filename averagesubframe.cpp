﻿#include "averagesubframe.h"

AverageSubFrame::AverageSubFrame(QFrame *parent) : QFrame(parent)
{
    // size(200, 100)
    this->setObjectName("FrameQss");
    this->setStyleSheet("QFrame#FrameQss {border:1px solid black; background-color: rgb(96, 96, 96, 100);\
                            border-top-left-radius:4px;         \
                            border-top-right-radius:4px;        \
                            border-bottom-left-radius:4px;      \
                            border-bottom-right-radius:4px}");
    keyValue = 1;

    int fontId = QFontDatabase::addApplicationFont(":/ZhuoJianGanLanJianTi.ttf");
    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
    font.setFamily(fontFamilies.at(0));
    QString qssRadio = "QRadioButton { border-radius: 2px; color:red; text-align:left; padding:2px; font-size:14px;} \
            QRadioButton::indicator:checked { background-color: red; } \
            QRadioButton::indicator:unchecked { background-color: white; }";
    QString qssLabel_1 = "QLabel { color:red; text-align:left; padding:2px; font-size:14px;}";
    QString qssLabel_2 = "QLabel { color:red; text-align:left; padding:2px; font-size:20px;}";

    T1_Text = new QRadioButton(this);
    T1_Text->setText("T1 = 00:00:00.000");
    T1_Text->setGeometry(5, 10, 300, 25);
    T1_Text->setStyleSheet(qssRadio);      // QRadioButton {color:red;}
    T1_Text->setFont(font);
    T1_Text->setChecked(true);
    connect(T1_Text, SIGNAL(clicked(bool)), this, SLOT(slotButtonT1Check(bool)));
//    connect(T1_Text, &QAbstractButton::clicked, this, &AverageSubFrame::slotButtonCheck());
    T2_Text = new QRadioButton(this);
    T2_Text->setText("T2 = 00:00:00.000");
    T2_Text->setGeometry(5, 35, 300, 25);
    T2_Text->setStyleSheet(qssRadio);      // QRadioButton {color:red;}
    T2_Text->setFont(font);
    connect(T2_Text, SIGNAL(clicked(bool)), this, SLOT(slotButtonT2Check(bool)));
    T2_T1_Label = new QLabel(this);
    T2_T1_Label->setText("T2-T1 = 00:00:00.000");
    T2_T1_Label->setGeometry(5, 60, 300, 25);
    T2_T1_Label->setStyleSheet(qssLabel_1);      // QRadioButton {color:red;}
    T2_T1_Label->setFont(font);
    AverageLabel = new QLabel(this);
    AverageLabel->setText("平均值 = --");
    AverageLabel->setGeometry(5, 85, 300, 25);
    AverageLabel->setStyleSheet(qssLabel_2);      // QRadioButton {color:red;}
    AverageLabel->setFont(font);
    MaxLabel = new QLabel(this);
    MaxLabel->setText("最大值 = --");
    MaxLabel->setGeometry(5, 110, 300, 25);
    MaxLabel->setStyleSheet(qssLabel_2);      // QRadioButton {color:red;}
    MaxLabel->setFont(font);
    MinLabel = new QLabel(this);
    MinLabel->setText("最小值 = --");
    MinLabel->setGeometry(5, 135, 300, 25);
    MinLabel->setStyleSheet(qssLabel_2);      // QRadioButton {color:red;}
    MinLabel->setFont(font);
}

qint8 AverageSubFrame::getKeyValue(void)
{
    return keyValue;
}
void AverageSubFrame::setKeyValue(qint8 val)
{
    keyValue = val;
}

qint8 AverageSubFrame::getCurVolFlag(void)
{
    return CurVolFlag;
}
void AverageSubFrame::setCurVolFlag(qint8 val)
{
    CurVolFlag = val;
}

void AverageSubFrame::initFrameDisplay(void)
{
    keyValue = 1;
    T1_Text->setChecked(true);
    T1_Text->setText("T1 = 00:00:00.000");
    T2_Text->setText("T2 = 00:00:00.000");
    T2_T1_Label->setText("T2-T1 = 00:00:00.000");
    AverageLabel->setText("平均值 = --");
    MaxLabel->setText("最大值 = --");
    MinLabel->setText("最小值 = --");
}

void AverageSubFrame::slotButtonT1Check(bool val)
{
//    qDebug() << "Click ButtonT1 = " << val;
    keyValue = 1;
}

void AverageSubFrame::slotButtonT2Check(bool val)
{
//    qDebug() << "Click ButtonT2 = " << val;
    keyValue = 2;
}

void AverageSubFrame::slotUpdateT1AndT2(qint8 T1OrT2, qint64 Time)
{
    if(T1OrT2 == 1)
        T1_Text->setText("T1 = " + QDateTime::fromMSecsSinceEpoch(Time).toString("hh:mm:ss.zzz"));
    else if(T1OrT2 == 2)
        T2_Text->setText("T2 = " + QDateTime::fromMSecsSinceEpoch(Time).toString("hh:mm:ss.zzz"));
}

void AverageSubFrame::slotUpdateAverage(qint64 DifferT, double avergeVal, double maxVal, double minVal)
{
    T2_T1_Label->setText("T2-T1 = " + QDateTime::fromMSecsSinceEpoch(DifferT).toString("00:mm:ss.zzz"));
    if(CurVolFlag == 1)
    {
        if(avergeVal < 1)
            AverageLabel->setText("平均值 = " + QString::number(avergeVal * 1000, 'f', 3) + "uA");
        else
            AverageLabel->setText("平均值 = " + QString::number(avergeVal, 'f', 3) + "mA");
        if(maxVal < 1)
            MaxLabel->setText("最大值 = " + QString::number(maxVal * 1000, 'f', 3) + "uA");
        else
            MaxLabel->setText("最大值 = " + QString::number(maxVal, 'f', 3) + "mA");
        if(minVal < 1)
            MinLabel->setText("最小值 = " + QString::number(minVal * 1000, 'f', 3) + "uA");
        else
            MinLabel->setText("最小值 = " + QString::number(minVal, 'f', 3) + "mA");
    }
    else if(CurVolFlag == 2)
    {
        AverageLabel->setText("平均值 = " + QString::number(avergeVal, 'f', 3) + "V");
        MaxLabel->setText("最大值 = " + QString::number(maxVal, 'f', 3) + "V");
        MinLabel->setText("最小值 = " + QString::number(minVal, 'f', 3) + "V");
    }
}