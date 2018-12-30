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
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TrafficLoadManagerClass
{
public:
    QAction *actionDroga_jednokierunkowa_z_jednym_pasem;
    QAction *actionDroga_jednokierunkowa_z_dwoma_pasami_ruchu;
    QAction *actionDroga_jednokierunkowa_z_trzema_pasami_ruchu;
    QAction *actionDroga_dwukierunkowa_z_jednym_pasem_ruchu;
    QAction *actionDroga_dwukierunkowa_z_dwoma_pasami_ruchu;
    QAction *actionCofnij;
    QAction *actionKonfiguracja;
    QAction *actionWczytaj_plansz_z_pliku;
    QAction *actionZapisz_plansz_do_pliku;
    QAction *actionSymuluj;
    QAction *actionPunkt_odradzania_pojazd_w;
    QAction *action_sygnalizacja;
    QAction *action_sygnalizacja_edytuj;
    QAction *actionPunkt_odradzania_pojazd_w_2;
    QAction *action_koniec_symulacji;
    QAction *actionPo_czenia_na_skrzy_owaniu;
    QAction *actionZatrzymaj_symulacj;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    DesignArea *designArea;
    QMenuBar *menuBar;
    QMenu *menuPliki;
    QMenu *menuCzynno_ci;
    QMenu *menuRysuj;
    QMenu *menuEdytuj;
    QMenu *menuUstawienia;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *TrafficLoadManagerClass)
    {
        if (TrafficLoadManagerClass->objectName().isEmpty())
            TrafficLoadManagerClass->setObjectName(QStringLiteral("TrafficLoadManagerClass"));
        TrafficLoadManagerClass->resize(882, 732);
        actionDroga_jednokierunkowa_z_jednym_pasem = new QAction(TrafficLoadManagerClass);
        actionDroga_jednokierunkowa_z_jednym_pasem->setObjectName(QStringLiteral("actionDroga_jednokierunkowa_z_jednym_pasem"));
        actionDroga_jednokierunkowa_z_dwoma_pasami_ruchu = new QAction(TrafficLoadManagerClass);
        actionDroga_jednokierunkowa_z_dwoma_pasami_ruchu->setObjectName(QStringLiteral("actionDroga_jednokierunkowa_z_dwoma_pasami_ruchu"));
        actionDroga_jednokierunkowa_z_trzema_pasami_ruchu = new QAction(TrafficLoadManagerClass);
        actionDroga_jednokierunkowa_z_trzema_pasami_ruchu->setObjectName(QStringLiteral("actionDroga_jednokierunkowa_z_trzema_pasami_ruchu"));
        actionDroga_dwukierunkowa_z_jednym_pasem_ruchu = new QAction(TrafficLoadManagerClass);
        actionDroga_dwukierunkowa_z_jednym_pasem_ruchu->setObjectName(QStringLiteral("actionDroga_dwukierunkowa_z_jednym_pasem_ruchu"));
        actionDroga_dwukierunkowa_z_dwoma_pasami_ruchu = new QAction(TrafficLoadManagerClass);
        actionDroga_dwukierunkowa_z_dwoma_pasami_ruchu->setObjectName(QStringLiteral("actionDroga_dwukierunkowa_z_dwoma_pasami_ruchu"));
        actionCofnij = new QAction(TrafficLoadManagerClass);
        actionCofnij->setObjectName(QStringLiteral("actionCofnij"));
        actionKonfiguracja = new QAction(TrafficLoadManagerClass);
        actionKonfiguracja->setObjectName(QStringLiteral("actionKonfiguracja"));
        actionWczytaj_plansz_z_pliku = new QAction(TrafficLoadManagerClass);
        actionWczytaj_plansz_z_pliku->setObjectName(QStringLiteral("actionWczytaj_plansz_z_pliku"));
        actionZapisz_plansz_do_pliku = new QAction(TrafficLoadManagerClass);
        actionZapisz_plansz_do_pliku->setObjectName(QStringLiteral("actionZapisz_plansz_do_pliku"));
        actionSymuluj = new QAction(TrafficLoadManagerClass);
        actionSymuluj->setObjectName(QStringLiteral("actionSymuluj"));
        actionPunkt_odradzania_pojazd_w = new QAction(TrafficLoadManagerClass);
        actionPunkt_odradzania_pojazd_w->setObjectName(QStringLiteral("actionPunkt_odradzania_pojazd_w"));
        action_sygnalizacja = new QAction(TrafficLoadManagerClass);
        action_sygnalizacja->setObjectName(QStringLiteral("action_sygnalizacja"));
        action_sygnalizacja_edytuj = new QAction(TrafficLoadManagerClass);
        action_sygnalizacja_edytuj->setObjectName(QStringLiteral("action_sygnalizacja_edytuj"));
        actionPunkt_odradzania_pojazd_w_2 = new QAction(TrafficLoadManagerClass);
        actionPunkt_odradzania_pojazd_w_2->setObjectName(QStringLiteral("actionPunkt_odradzania_pojazd_w_2"));
        action_koniec_symulacji = new QAction(TrafficLoadManagerClass);
        action_koniec_symulacji->setObjectName(QStringLiteral("action_koniec_symulacji"));
        actionPo_czenia_na_skrzy_owaniu = new QAction(TrafficLoadManagerClass);
        actionPo_czenia_na_skrzy_owaniu->setObjectName(QStringLiteral("actionPo_czenia_na_skrzy_owaniu"));
        actionZatrzymaj_symulacj = new QAction(TrafficLoadManagerClass);
        actionZatrzymaj_symulacj->setObjectName(QStringLiteral("actionZatrzymaj_symulacj"));
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
        menuPliki = new QMenu(menuBar);
        menuPliki->setObjectName(QStringLiteral("menuPliki"));
        menuCzynno_ci = new QMenu(menuBar);
        menuCzynno_ci->setObjectName(QStringLiteral("menuCzynno_ci"));
        menuRysuj = new QMenu(menuCzynno_ci);
        menuRysuj->setObjectName(QStringLiteral("menuRysuj"));
        menuEdytuj = new QMenu(menuCzynno_ci);
        menuEdytuj->setObjectName(QStringLiteral("menuEdytuj"));
        menuUstawienia = new QMenu(menuBar);
        menuUstawienia->setObjectName(QStringLiteral("menuUstawienia"));
        TrafficLoadManagerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(TrafficLoadManagerClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        TrafficLoadManagerClass->addToolBar(Qt::TopToolBarArea, mainToolBar);

        menuBar->addAction(menuPliki->menuAction());
        menuBar->addAction(menuCzynno_ci->menuAction());
        menuBar->addAction(menuUstawienia->menuAction());
        menuPliki->addAction(actionWczytaj_plansz_z_pliku);
        menuPliki->addAction(actionZapisz_plansz_do_pliku);
        menuCzynno_ci->addAction(actionCofnij);
        menuCzynno_ci->addSeparator();
        menuCzynno_ci->addAction(menuRysuj->menuAction());
        menuCzynno_ci->addAction(menuEdytuj->menuAction());
        menuCzynno_ci->addSeparator();
        menuCzynno_ci->addAction(actionSymuluj);
        menuCzynno_ci->addAction(action_koniec_symulacji);
        menuRysuj->addAction(actionDroga_jednokierunkowa_z_jednym_pasem);
        menuRysuj->addAction(actionDroga_jednokierunkowa_z_dwoma_pasami_ruchu);
        menuRysuj->addAction(actionDroga_dwukierunkowa_z_jednym_pasem_ruchu);
        menuRysuj->addAction(actionPunkt_odradzania_pojazd_w);
        menuRysuj->addAction(action_sygnalizacja);
        menuEdytuj->addAction(action_sygnalizacja_edytuj);
        menuEdytuj->addAction(actionPunkt_odradzania_pojazd_w_2);
        menuEdytuj->addAction(actionPo_czenia_na_skrzy_owaniu);
        menuUstawienia->addAction(actionKonfiguracja);

        retranslateUi(TrafficLoadManagerClass);
        QObject::connect(actionDroga_jednokierunkowa_z_jednym_pasem, SIGNAL(triggered()), designArea, SLOT(handleAction()));
        QObject::connect(actionCofnij, SIGNAL(triggered()), designArea, SLOT(handleAction()));
        QObject::connect(actionDroga_jednokierunkowa_z_dwoma_pasami_ruchu, SIGNAL(triggered()), designArea, SLOT(handleAction()));
        QObject::connect(actionPo_czenia_na_skrzy_owaniu, SIGNAL(triggered()), designArea, SLOT(handleAction()));
        QObject::connect(actionPunkt_odradzania_pojazd_w, SIGNAL(triggered()), designArea, SLOT(handleAction()));
        QObject::connect(actionDroga_dwukierunkowa_z_jednym_pasem_ruchu, SIGNAL(triggered()), designArea, SLOT(handleAction()));
        QObject::connect(actionPunkt_odradzania_pojazd_w_2, SIGNAL(triggered()), designArea, SLOT(handleAction()));
        QObject::connect(action_sygnalizacja, SIGNAL(triggered()), designArea, SLOT(handleAction()));
        QObject::connect(action_sygnalizacja_edytuj, SIGNAL(triggered()), designArea, SLOT(handleAction()));
        QObject::connect(actionSymuluj, SIGNAL(triggered()), designArea, SLOT(startSimulation()));
        QObject::connect(action_koniec_symulacji, SIGNAL(triggered()), designArea, SLOT(stopSimulation()));

        QMetaObject::connectSlotsByName(TrafficLoadManagerClass);
    } // setupUi

    void retranslateUi(QMainWindow *TrafficLoadManagerClass)
    {
        TrafficLoadManagerClass->setWindowTitle(QApplication::translate("TrafficLoadManagerClass", "TrafficLoadManager", nullptr));
        actionDroga_jednokierunkowa_z_jednym_pasem->setText(QApplication::translate("TrafficLoadManagerClass", "Droga jednokierunkowa z jednym pasem ruchu", nullptr));
        actionDroga_jednokierunkowa_z_dwoma_pasami_ruchu->setText(QApplication::translate("TrafficLoadManagerClass", "Droga jednokierunkowa z dwoma pasami ruchu", nullptr));
        actionDroga_jednokierunkowa_z_trzema_pasami_ruchu->setText(QApplication::translate("TrafficLoadManagerClass", "Droga jednokierunkowa z trzema pasami ruchu", nullptr));
        actionDroga_dwukierunkowa_z_jednym_pasem_ruchu->setText(QApplication::translate("TrafficLoadManagerClass", "Droga dwukierunkowa z jednym pasem ruchu", nullptr));
        actionDroga_dwukierunkowa_z_dwoma_pasami_ruchu->setText(QApplication::translate("TrafficLoadManagerClass", "Droga dwukierunkowa z dwoma pasami ruchu", nullptr));
        actionCofnij->setText(QApplication::translate("TrafficLoadManagerClass", "Cofnij", nullptr));
        actionKonfiguracja->setText(QApplication::translate("TrafficLoadManagerClass", "Konfiguracja", nullptr));
        actionWczytaj_plansz_z_pliku->setText(QApplication::translate("TrafficLoadManagerClass", "Wczytaj plansz\304\231 z pliku", nullptr));
        actionZapisz_plansz_do_pliku->setText(QApplication::translate("TrafficLoadManagerClass", "Zapisz plansz\304\231 do pliku", nullptr));
        actionSymuluj->setText(QApplication::translate("TrafficLoadManagerClass", "Symuluj", nullptr));
        actionPunkt_odradzania_pojazd_w->setText(QApplication::translate("TrafficLoadManagerClass", "Punkt odradzania pojazd\303\263w", nullptr));
        action_sygnalizacja->setText(QApplication::translate("TrafficLoadManagerClass", "Sygnalizacja \305\233wietlna", nullptr));
        action_sygnalizacja_edytuj->setText(QApplication::translate("TrafficLoadManagerClass", "Sygnalizacja \305\233wietlna", nullptr));
        actionPunkt_odradzania_pojazd_w_2->setText(QApplication::translate("TrafficLoadManagerClass", "Punkt odradzania pojazd\303\263w", nullptr));
        action_koniec_symulacji->setText(QApplication::translate("TrafficLoadManagerClass", "Zako\305\204cz symulacj\304\231", nullptr));
        actionPo_czenia_na_skrzy_owaniu->setText(QApplication::translate("TrafficLoadManagerClass", "Po\305\202\304\205czenia na skrzy\305\274owaniu", nullptr));
        actionZatrzymaj_symulacj->setText(QApplication::translate("TrafficLoadManagerClass", "Zatrzymaj symulacj\304\231", nullptr));
#ifndef QT_NO_ACCESSIBILITY
        designArea->setAccessibleName(QApplication::translate("TrafficLoadManagerClass", "designAreaWidget", nullptr));
#endif // QT_NO_ACCESSIBILITY
        menuPliki->setTitle(QApplication::translate("TrafficLoadManagerClass", "Pliki", nullptr));
        menuCzynno_ci->setTitle(QApplication::translate("TrafficLoadManagerClass", "Czynno\305\233ci", nullptr));
        menuRysuj->setTitle(QApplication::translate("TrafficLoadManagerClass", "Rysuj", nullptr));
        menuEdytuj->setTitle(QApplication::translate("TrafficLoadManagerClass", "Edytuj", nullptr));
        menuUstawienia->setTitle(QApplication::translate("TrafficLoadManagerClass", "Ustawienia", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TrafficLoadManagerClass: public Ui_TrafficLoadManagerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRAFFICLOADMANAGER_H
