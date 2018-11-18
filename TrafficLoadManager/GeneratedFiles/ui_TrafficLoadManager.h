/********************************************************************************
** Form generated from reading UI file 'TrafficLoadManager.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRAFFICLOADMANAGER_H
#define UI_TRAFFICLOADMANAGER_H

#include <DesignArea.h>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TrafficLoadManagerClass
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    DesignArea *designArea;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *TrafficLoadManagerClass)
    {
        if (TrafficLoadManagerClass->objectName().isEmpty())
            TrafficLoadManagerClass->setObjectName(QStringLiteral("TrafficLoadManagerClass"));
        TrafficLoadManagerClass->resize(882, 715);
        centralWidget = new QWidget(TrafficLoadManagerClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        designArea = new DesignArea(centralWidget);
        designArea->setObjectName(QStringLiteral("designArea"));

        horizontalLayout->addWidget(designArea);

        TrafficLoadManagerClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(TrafficLoadManagerClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 882, 18));
        TrafficLoadManagerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(TrafficLoadManagerClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        TrafficLoadManagerClass->addToolBar(Qt::TopToolBarArea, mainToolBar);

        retranslateUi(TrafficLoadManagerClass);

        QMetaObject::connectSlotsByName(TrafficLoadManagerClass);
    } // setupUi

    void retranslateUi(QMainWindow *TrafficLoadManagerClass)
    {
        TrafficLoadManagerClass->setWindowTitle(QApplication::translate("TrafficLoadManagerClass", "TrafficLoadManager", nullptr));
#ifndef QT_NO_ACCESSIBILITY
        designArea->setAccessibleName(QApplication::translate("TrafficLoadManagerClass", "designAreaWidget", nullptr));
#endif // QT_NO_ACCESSIBILITY
    } // retranslateUi

};

namespace Ui {
    class TrafficLoadManagerClass: public Ui_TrafficLoadManagerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRAFFICLOADMANAGER_H
