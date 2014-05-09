/********************************************************************************
** Form generated from reading UI file 'TrayMenu.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRAYMENU_H
#define UI_TRAYMENU_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_TrayMenu
{
public:

    void setupUi(QFrame *TrayMenu)
    {
        if (TrayMenu->objectName().isEmpty())
            TrayMenu->setObjectName(QStringLiteral("TrayMenu"));
        TrayMenu->setFrameShadow(QFrame::Raised);
        TrayMenu->resize(400, 300);
        TrayMenu->setFrameShape(QFrame::StyledPanel);

        retranslateUi(TrayMenu);

        QMetaObject::connectSlotsByName(TrayMenu);
    } // setupUi

    void retranslateUi(QFrame *TrayMenu)
    {
        TrayMenu->setWindowTitle(QApplication::translate("TrayMenu", "Frame", 0));
    } // retranslateUi

};

namespace Ui {
    class TrayMenu: public Ui_TrayMenu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRAYMENU_H
