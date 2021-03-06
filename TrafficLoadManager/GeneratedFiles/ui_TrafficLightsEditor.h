/********************************************************************************
** Form generated from reading UI file 'TrafficLightsEditor.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRAFFICLIGHTSEDITOR_H
#define UI_TRAFFICLIGHTSEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_TrafficLightsEditor
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label_skrzyzowanie;
    QHBoxLayout *horizontalLayout;
    QLabel *label_colour;
    QLabel *label_kolejny_krok;
    QComboBox *comboBox_kierunki;
    QLabel *label_3;
    QSpinBox *spinBox_green;
    QLabel *label;
    QPushButton *pushButton_dodaj;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_3;
    QTableWidget *tableWidget;
    QPushButton *pushButton_reset;
    QHBoxLayout *horizontalLayout_2;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *TrafficLightsEditor)
    {
        if (TrafficLightsEditor->objectName().isEmpty())
            TrafficLightsEditor->setObjectName(QStringLiteral("TrafficLightsEditor"));
        TrafficLightsEditor->resize(874, 498);
        verticalLayout = new QVBoxLayout(TrafficLightsEditor);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label_skrzyzowanie = new QLabel(TrafficLightsEditor);
        label_skrzyzowanie->setObjectName(QStringLiteral("label_skrzyzowanie"));
        QFont font;
        font.setPointSize(16);
        label_skrzyzowanie->setFont(font);
        label_skrzyzowanie->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_skrzyzowanie);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_colour = new QLabel(TrafficLightsEditor);
        label_colour->setObjectName(QStringLiteral("label_colour"));
        label_colour->setMinimumSize(QSize(50, 0));
        label_colour->setMaximumSize(QSize(50, 16777215));

        horizontalLayout->addWidget(label_colour);

        label_kolejny_krok = new QLabel(TrafficLightsEditor);
        label_kolejny_krok->setObjectName(QStringLiteral("label_kolejny_krok"));
        label_kolejny_krok->setMinimumSize(QSize(150, 0));
        label_kolejny_krok->setMaximumSize(QSize(150, 16777215));
        label_kolejny_krok->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(label_kolejny_krok);

        comboBox_kierunki = new QComboBox(TrafficLightsEditor);
        comboBox_kierunki->setObjectName(QStringLiteral("comboBox_kierunki"));
        comboBox_kierunki->setMinimumSize(QSize(300, 0));

        horizontalLayout->addWidget(comboBox_kierunki);

        label_3 = new QLabel(TrafficLightsEditor);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setMinimumSize(QSize(200, 0));
        label_3->setMaximumSize(QSize(200, 16777215));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(label_3);

        spinBox_green = new QSpinBox(TrafficLightsEditor);
        spinBox_green->setObjectName(QStringLiteral("spinBox_green"));
        spinBox_green->setMaximumSize(QSize(50, 16777215));
        spinBox_green->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(spinBox_green);

        label = new QLabel(TrafficLightsEditor);
        label->setObjectName(QStringLiteral("label"));
        label->setMinimumSize(QSize(20, 0));
        label->setMaximumSize(QSize(20, 16777215));

        horizontalLayout->addWidget(label);

        pushButton_dodaj = new QPushButton(TrafficLightsEditor);
        pushButton_dodaj->setObjectName(QStringLiteral("pushButton_dodaj"));
        pushButton_dodaj->setMinimumSize(QSize(0, 0));
        pushButton_dodaj->setMaximumSize(QSize(120, 16777215));

        horizontalLayout->addWidget(pushButton_dodaj);


        verticalLayout->addLayout(horizontalLayout);

        label_2 = new QLabel(TrafficLightsEditor);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        tableWidget = new QTableWidget(TrafficLightsEditor);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));

        horizontalLayout_3->addWidget(tableWidget);

        pushButton_reset = new QPushButton(TrafficLightsEditor);
        pushButton_reset->setObjectName(QStringLiteral("pushButton_reset"));

        horizontalLayout_3->addWidget(pushButton_reset);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));

        verticalLayout->addLayout(horizontalLayout_2);

        buttonBox = new QDialogButtonBox(TrafficLightsEditor);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(TrafficLightsEditor);
        QObject::connect(buttonBox, SIGNAL(accepted()), TrafficLightsEditor, SLOT(acceptButton()));
        QObject::connect(buttonBox, SIGNAL(rejected()), TrafficLightsEditor, SLOT(rejectButton()));
        QObject::connect(pushButton_dodaj, SIGNAL(pressed()), TrafficLightsEditor, SLOT(addToTableAction()));
        QObject::connect(tableWidget, SIGNAL(cellDoubleClicked(int,int)), TrafficLightsEditor, SLOT(deleteFromTableAction(int,int)));
        QObject::connect(pushButton_reset, SIGNAL(clicked()), TrafficLightsEditor, SLOT(resetSettings()));
        QObject::connect(comboBox_kierunki, SIGNAL(currentIndexChanged(int)), TrafficLightsEditor, SLOT(comboBoxIndexChanged(int)));

        QMetaObject::connectSlotsByName(TrafficLightsEditor);
    } // setupUi

    void retranslateUi(QDialog *TrafficLightsEditor)
    {
        TrafficLightsEditor->setWindowTitle(QApplication::translate("TrafficLightsEditor", "Edytor \305\233wiate\305\202", nullptr));
        label_skrzyzowanie->setText(QApplication::translate("TrafficLightsEditor", "W\304\231ze\305\202", nullptr));
        label_colour->setText(QString());
        label_kolejny_krok->setText(QApplication::translate("TrafficLightsEditor", "Krok numer ", nullptr));
        label_3->setText(QApplication::translate("TrafficLightsEditor", "d\305\202ugo\305\233\304\207 zielonego \305\233wiat\305\202a", nullptr));
        label->setText(QApplication::translate("TrafficLightsEditor", "[s]", nullptr));
        pushButton_dodaj->setText(QApplication::translate("TrafficLightsEditor", "Dodaj krok", nullptr));
        label_2->setText(QApplication::translate("TrafficLightsEditor", "Aktualna sekwencja", nullptr));
        pushButton_reset->setText(QApplication::translate("TrafficLightsEditor", "Reset", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TrafficLightsEditor: public Ui_TrafficLightsEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRAFFICLIGHTSEDITOR_H
