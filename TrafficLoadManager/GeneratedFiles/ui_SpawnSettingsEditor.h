/********************************************************************************
** Form generated from reading UI file 'SpawnSettingsEditor.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SPAWNSETTINGSEDITOR_H
#define UI_SPAWNSETTINGSEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_SpawnSettingsEditor
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLineEdit *lineEdit_Actual;
    QHBoxLayout *horizontalLayout_5;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QGridLayout *gridLayout;
    QSpinBox *spinBox_cars;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label_4;
    QSpinBox *spinBox_smart_drivers;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QComboBox *comboBox_available;
    QPushButton *pushButton_add;
    QLabel *label_2;
    QTableWidget *tableWidget;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *SpawnSettingsEditor)
    {
        if (SpawnSettingsEditor->objectName().isEmpty())
            SpawnSettingsEditor->setObjectName(QStringLiteral("SpawnSettingsEditor"));
        SpawnSettingsEditor->resize(800, 400);
        SpawnSettingsEditor->setMinimumSize(QSize(800, 400));
        verticalLayout_2 = new QVBoxLayout(SpawnSettingsEditor);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        lineEdit_Actual = new QLineEdit(SpawnSettingsEditor);
        lineEdit_Actual->setObjectName(QStringLiteral("lineEdit_Actual"));
        QFont font;
        font.setPointSize(16);
        lineEdit_Actual->setFont(font);
        lineEdit_Actual->setAutoFillBackground(true);
        lineEdit_Actual->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lineEdit_Actual);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label = new QLabel(SpawnSettingsEditor);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_3->addWidget(label);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        spinBox_cars = new QSpinBox(SpawnSettingsEditor);
        spinBox_cars->setObjectName(QStringLiteral("spinBox_cars"));
        spinBox_cars->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinBox_cars->setMaximum(60);

        gridLayout->addWidget(spinBox_cars, 0, 0, 1, 1);


        horizontalLayout_3->addLayout(gridLayout);


        horizontalLayout_5->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_4 = new QLabel(SpawnSettingsEditor);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(label_4);


        horizontalLayout_2->addLayout(horizontalLayout);

        spinBox_smart_drivers = new QSpinBox(SpawnSettingsEditor);
        spinBox_smart_drivers->setObjectName(QStringLiteral("spinBox_smart_drivers"));
        spinBox_smart_drivers->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinBox_smart_drivers->setMaximum(100);

        horizontalLayout_2->addWidget(spinBox_smart_drivers);


        horizontalLayout_5->addLayout(horizontalLayout_2);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_3 = new QLabel(SpawnSettingsEditor);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setMinimumSize(QSize(70, 0));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_4->addWidget(label_3);

        comboBox_available = new QComboBox(SpawnSettingsEditor);
        comboBox_available->setObjectName(QStringLiteral("comboBox_available"));

        horizontalLayout_4->addWidget(comboBox_available);

        pushButton_add = new QPushButton(SpawnSettingsEditor);
        pushButton_add->setObjectName(QStringLiteral("pushButton_add"));
        pushButton_add->setCursor(QCursor(Qt::ArrowCursor));

        horizontalLayout_4->addWidget(pushButton_add);


        verticalLayout->addLayout(horizontalLayout_4);

        label_2 = new QLabel(SpawnSettingsEditor);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_2);

        tableWidget = new QTableWidget(SpawnSettingsEditor);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));

        verticalLayout->addWidget(tableWidget);


        verticalLayout_2->addLayout(verticalLayout);

        buttonBox = new QDialogButtonBox(SpawnSettingsEditor);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setMinimumSize(QSize(70, 60));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout_2->addWidget(buttonBox);


        retranslateUi(SpawnSettingsEditor);
        QObject::connect(buttonBox, SIGNAL(accepted()), SpawnSettingsEditor, SLOT(acceptButton()));
        QObject::connect(buttonBox, SIGNAL(rejected()), SpawnSettingsEditor, SLOT(rejectButton()));
        QObject::connect(pushButton_add, SIGNAL(pressed()), SpawnSettingsEditor, SLOT(addToTableAction()));
        QObject::connect(tableWidget, SIGNAL(cellDoubleClicked(int,int)), SpawnSettingsEditor, SLOT(deleteFromTableAction(int,int)));

        QMetaObject::connectSlotsByName(SpawnSettingsEditor);
    } // setupUi

    void retranslateUi(QDialog *SpawnSettingsEditor)
    {
        SpawnSettingsEditor->setWindowTitle(QApplication::translate("SpawnSettingsEditor", "Edytor punktu odradzania pojazd\303\263w", nullptr));
        lineEdit_Actual->setText(QApplication::translate("SpawnSettingsEditor", "W\304\231ze\305\202", nullptr));
        label->setText(QApplication::translate("SpawnSettingsEditor", "Liczba pojazd\303\263w na minut\304\231", nullptr));
        label_4->setText(QApplication::translate("SpawnSettingsEditor", "Procent kierowc\303\263w z GPS", nullptr));
        label_3->setText(QApplication::translate("SpawnSettingsEditor", "Dost\304\231pne w\304\231z\305\202y", nullptr));
        pushButton_add->setText(QApplication::translate("SpawnSettingsEditor", "Dodaj w\304\231ze\305\202", nullptr));
        label_2->setText(QApplication::translate("SpawnSettingsEditor", "W\304\231z\305\202y docelowe", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SpawnSettingsEditor: public Ui_SpawnSettingsEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPAWNSETTINGSEDITOR_H
