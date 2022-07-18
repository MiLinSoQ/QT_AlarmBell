/********************************************************************************
** Form generated from reading UI file 'AlarmBell.ui'
**
** Created by: Qt User Interface Compiler version 5.12.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ALARMBELL_H
#define UI_ALARMBELL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AlarmBell
{
public:

    void setupUi(QWidget *AlarmBell)
    {
        if (AlarmBell->objectName().isEmpty())
            AlarmBell->setObjectName(QString::fromUtf8("AlarmBell"));
        AlarmBell->resize(500, 300);

        retranslateUi(AlarmBell);

        QMetaObject::connectSlotsByName(AlarmBell);
    } // setupUi

    void retranslateUi(QWidget *AlarmBell)
    {
        AlarmBell->setWindowTitle(QApplication::translate("AlarmBell", "AlarmBell", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AlarmBell: public Ui_AlarmBell {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ALARMBELL_H
