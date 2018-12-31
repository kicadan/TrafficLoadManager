/********************************************************************************
** Form generated from reading UI file 'StatisticsDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STATISTICSDIALOG_H
#define UI_STATISTICSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_StatisticsDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QTableWidget *tableWidget;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *StatisticsDialog)
    {
        if (StatisticsDialog->objectName().isEmpty())
            StatisticsDialog->setObjectName(QStringLiteral("StatisticsDialog"));
        StatisticsDialog->resize(1400, 800);
        StatisticsDialog->setMinimumSize(QSize(800, 600));
        verticalLayout = new QVBoxLayout(StatisticsDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(StatisticsDialog);
        label->setObjectName(QStringLiteral("label"));
        QFont font;
        font.setPointSize(16);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        tableWidget = new QTableWidget(StatisticsDialog);
        if (tableWidget->columnCount() < 5)
            tableWidget->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        __qtablewidgetitem->setTextAlignment(Qt::AlignTrailing|Qt::AlignVCenter);
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        __qtablewidgetitem1->setTextAlignment(Qt::AlignTrailing|Qt::AlignVCenter);
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        __qtablewidgetitem2->setTextAlignment(Qt::AlignTrailing|Qt::AlignVCenter);
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        __qtablewidgetitem3->setTextAlignment(Qt::AlignTrailing|Qt::AlignVCenter);
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        __qtablewidgetitem4->setTextAlignment(Qt::AlignTrailing|Qt::AlignVCenter);
        tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget->setColumnCount(5);

        verticalLayout->addWidget(tableWidget);

        buttonBox = new QDialogButtonBox(StatisticsDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(StatisticsDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), StatisticsDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), StatisticsDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(StatisticsDialog);
    } // setupUi

    void retranslateUi(QDialog *StatisticsDialog)
    {
        StatisticsDialog->setWindowTitle(QApplication::translate("StatisticsDialog", "Statystyki", nullptr));
        label->setText(QApplication::translate("StatisticsDialog", "Statystyki symulacji", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("StatisticsDialog", "Nazwa skrzy\305\274owania", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("StatisticsDialog", "Powinno powsta\304\207", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("StatisticsDialog", "Pojazdy stworzone", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("StatisticsDialog", "Kierunek", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("StatisticsDialog", "Przejecha\305\202o samochod\303\263w", nullptr));
    } // retranslateUi

};

namespace Ui {
    class StatisticsDialog: public Ui_StatisticsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STATISTICSDIALOG_H
