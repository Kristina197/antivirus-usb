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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QLabel *titleLabel;
    QLabel *label;
    QTableWidget *deviceTable;
    QHBoxLayout *horizontalLayout;
    QPushButton *scanButton;
    QPushButton *settingsButton;
    QPushButton *quarantineButton;
    QLabel *label_2;
    QTableWidget *resultsTable;
    QProgressBar *progressBar;
    QLabel *statusLabel;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(900, 600);
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

        label = new QLabel(centralwidget);
        label->setObjectName("label");

        verticalLayout->addWidget(label);

        deviceTable = new QTableWidget(centralwidget);
        deviceTable->setObjectName("deviceTable");

        verticalLayout->addWidget(deviceTable);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        scanButton = new QPushButton(centralwidget);
        scanButton->setObjectName("scanButton");

        horizontalLayout->addWidget(scanButton);

        settingsButton = new QPushButton(centralwidget);
        settingsButton->setObjectName("settingsButton");

        horizontalLayout->addWidget(settingsButton);

        quarantineButton = new QPushButton(centralwidget);
        quarantineButton->setObjectName("quarantineButton");

        horizontalLayout->addWidget(quarantineButton);


        verticalLayout->addLayout(horizontalLayout);

        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");

        verticalLayout->addWidget(label_2);

        resultsTable = new QTableWidget(centralwidget);
        resultsTable->setObjectName("resultsTable");

        verticalLayout->addWidget(resultsTable);

        progressBar = new QProgressBar(centralwidget);
        progressBar->setObjectName("progressBar");
        progressBar->setValue(0);

        verticalLayout->addWidget(progressBar);

        statusLabel = new QLabel(centralwidget);
        statusLabel->setObjectName("statusLabel");

        verticalLayout->addWidget(statusLabel);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "USB Antivirus Scanner", nullptr));
        titleLabel->setText(QCoreApplication::translate("MainWindow", "USB Antivirus Scanner", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\264\320\272\320\273\321\216\321\207\320\265\320\275\320\275\321\213\320\265 \321\203\321\201\321\202\321\200\320\276\320\271\321\201\321\202\320\262\320\260:", nullptr));
        scanButton->setText(QCoreApplication::translate("MainWindow", "Scan Selected Device", nullptr));
        settingsButton->setText(QCoreApplication::translate("MainWindow", "\342\232\231\357\270\217 Settings", nullptr));
        quarantineButton->setText(QCoreApplication::translate("MainWindow", " Quarantine", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\320\240\320\265\320\267\321\203\320\273\321\214\321\202\320\260\321\202\321\213 \321\201\320\272\320\260\320\275\320\270\321\200\320\276\320\262\320\260\320\275\320\270\321\217:", nullptr));
        statusLabel->setText(QCoreApplication::translate("MainWindow", "Ready", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
