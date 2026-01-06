/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QLabel *titleLabel;
    QGroupBox *devicesGroup;
    QVBoxLayout *vboxLayout;
    QListWidget *deviceList;
    QHBoxLayout *hboxLayout;
    QPushButton *refreshButton;
    QPushButton *scanButton;
    QProgressBar *progressBar;
    QGroupBox *resultsGroup;
    QVBoxLayout *vboxLayout1;
    QTableWidget *resultsTable;
    QHBoxLayout *hboxLayout1;
    QPushButton *quarantineSelectedButton;
    QPushButton *clearResultsButton;
    QSpacerItem *horizontalSpacer;
    QGroupBox *logGroup;
    QVBoxLayout *vboxLayout2;
    QTextEdit *logText;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        titleLabel = new QLabel(centralwidget);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setAlignment(Qt::AlignCenter);
        QFont font;
        font.setPointSize(16);
        font.setBold(true);
        titleLabel->setFont(font);

        verticalLayout->addWidget(titleLabel);

        devicesGroup = new QGroupBox(centralwidget);
        devicesGroup->setObjectName("devicesGroup");
        vboxLayout = new QVBoxLayout(devicesGroup);
        vboxLayout->setObjectName("vboxLayout");
        deviceList = new QListWidget(devicesGroup);
        deviceList->setObjectName("deviceList");

        vboxLayout->addWidget(deviceList);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName("hboxLayout");
        refreshButton = new QPushButton(devicesGroup);
        refreshButton->setObjectName("refreshButton");

        hboxLayout->addWidget(refreshButton);

        scanButton = new QPushButton(devicesGroup);
        scanButton->setObjectName("scanButton");

        hboxLayout->addWidget(scanButton);


        vboxLayout->addLayout(hboxLayout);


        verticalLayout->addWidget(devicesGroup);

        progressBar = new QProgressBar(centralwidget);
        progressBar->setObjectName("progressBar");
        progressBar->setValue(0);

        verticalLayout->addWidget(progressBar);

        resultsGroup = new QGroupBox(centralwidget);
        resultsGroup->setObjectName("resultsGroup");
        vboxLayout1 = new QVBoxLayout(resultsGroup);
        vboxLayout1->setObjectName("vboxLayout1");
        resultsTable = new QTableWidget(resultsGroup);
        resultsTable->setObjectName("resultsTable");
        resultsTable->setSelectionBehavior(QAbstractItemView::SelectRows);

        vboxLayout1->addWidget(resultsTable);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setObjectName("hboxLayout1");
        quarantineSelectedButton = new QPushButton(resultsGroup);
        quarantineSelectedButton->setObjectName("quarantineSelectedButton");
        quarantineSelectedButton->setEnabled(false);

        hboxLayout1->addWidget(quarantineSelectedButton);

        clearResultsButton = new QPushButton(resultsGroup);
        clearResultsButton->setObjectName("clearResultsButton");

        hboxLayout1->addWidget(clearResultsButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout1->addItem(horizontalSpacer);


        vboxLayout1->addLayout(hboxLayout1);


        verticalLayout->addWidget(resultsGroup);

        logGroup = new QGroupBox(centralwidget);
        logGroup->setObjectName("logGroup");
        vboxLayout2 = new QVBoxLayout(logGroup);
        vboxLayout2->setObjectName("vboxLayout2");
        logText = new QTextEdit(logGroup);
        logText->setObjectName("logText");
        logText->setReadOnly(true);

        vboxLayout2->addWidget(logText);


        verticalLayout->addWidget(logGroup);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "USB Antivirus Scanner", nullptr));
        titleLabel->setText(QCoreApplication::translate("MainWindow", "USB Antivirus Scanner v2.0", nullptr));
        devicesGroup->setTitle(QCoreApplication::translate("MainWindow", "Connected USB Devices", nullptr));
        refreshButton->setText(QCoreApplication::translate("MainWindow", "Refresh", nullptr));
        scanButton->setText(QCoreApplication::translate("MainWindow", "Scan Selected Device", nullptr));
        resultsGroup->setTitle(QCoreApplication::translate("MainWindow", "Scan Results", nullptr));
        quarantineSelectedButton->setText(QCoreApplication::translate("MainWindow", "Quarantine Selected", nullptr));
        clearResultsButton->setText(QCoreApplication::translate("MainWindow", "Clear Results", nullptr));
        logGroup->setTitle(QCoreApplication::translate("MainWindow", "Activity Log", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
