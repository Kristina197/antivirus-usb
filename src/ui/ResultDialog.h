#pragma once

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>

class ResultDialog : public QDialog {
    Q_OBJECT

public:
    explicit ResultDialog(QWidget *parent = nullptr) : QDialog(parent) {
        setModal(true);
        setMinimumSize(450, 400);
        setStyleSheet("QDialog { background-color: #2b2b2b; }");
    }

    static void showInfectedResult(QWidget *parent, int threatsFound, int totalFiles) {
        ResultDialog dialog(parent);
        dialog.setWindowTitle("Угроза заражения");

        QVBoxLayout *layout = new QVBoxLayout(&dialog);
        layout->setSpacing(25);
        layout->setContentsMargins(40, 40, 40, 40);

        QLabel *iconLabel = new QLabel(&dialog);
        QPixmap shieldIcon("/home/kristina/antivirus-usb/resources/shield-infected.jpg");
        if (!shieldIcon.isNull()) {
            iconLabel->setPixmap(shieldIcon.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            iconLabel->setText("!");
            iconLabel->setStyleSheet("font-size: 72px; color: #e74c3c; font-weight: bold;");
        }
        iconLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(iconLabel);

        QLabel *titleLabel = new QLabel("Осторожно! Найдены вирусы", &dialog);
        titleLabel->setStyleSheet(
            "font-size: 28pt; "
            "font-weight: bold; "
            "color: #e74c3c; "
            "margin: 10px 0;"
        );
        titleLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(titleLabel);

        QLabel *threatsLabel = new QLabel(&dialog);
        QString threatsText;
        if (threatsFound == 1) {
            threatsText = "Найден 1 зараженный файл";
        } else if (threatsFound >= 2 && threatsFound <= 4) {
            threatsText = QString("Найдено %1 зараженных файла").arg(threatsFound);
        } else {
            threatsText = QString("Найдено %1 зараженных файлов").arg(threatsFound);
        }
        threatsLabel->setText(threatsText);
        threatsLabel->setStyleSheet("font-size: 14pt; color: #bdc3c7;");
        threatsLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(threatsLabel);

        QLabel *scannedLabel = new QLabel(&dialog);
        scannedLabel->setText(QString("Просканировано файлов: %1").arg(totalFiles));
        scannedLabel->setStyleSheet("font-size: 12pt; color: #95a5a6; margin-top: 10px;");
        scannedLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(scannedLabel);

        layout->addStretch();

        QPushButton *okButton = new QPushButton("OK", &dialog);
        okButton->setMinimumSize(150, 50);
        okButton->setStyleSheet(
            "QPushButton {"
            "  background-color: #e74c3c;"
            "  color: white;"
            "  border: none;"
            "  border-radius: 8px;"
            "  font-size: 16pt;"
            "  font-weight: bold;"
            "  padding: 10px;"
            "}"
            "QPushButton:hover {"
            "  background-color: #c0392b;"
            "}"
            "QPushButton:pressed {"
            "  background-color: #a93226;"
            "}"
        );
        connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);

        QHBoxLayout *buttonLayout = new QHBoxLayout();
        buttonLayout->addStretch();
        buttonLayout->addWidget(okButton);
        buttonLayout->addStretch();
        layout->addLayout(buttonLayout);

        dialog.exec();
    }

    static void showCleanResult(QWidget *parent, int totalFiles) {
        ResultDialog dialog(parent);
        dialog.setWindowTitle("Система чиста");
        dialog.setStyleSheet("QDialog { background-color: #2b2b2b; }");

        QVBoxLayout *layout = new QVBoxLayout(&dialog);
        layout->setSpacing(25);
        layout->setContentsMargins(40, 40, 40, 40);

        QLabel *iconLabel = new QLabel(&dialog);
        QPixmap shieldIcon("/home/kristina/antivirus-usb/resources/shield-clean.jpg");
        if (!shieldIcon.isNull()) {
            iconLabel->setPixmap(shieldIcon.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            iconLabel->setText("✓");
            iconLabel->setStyleSheet("font-size: 72px; color: #27ae60; font-weight: bold;");
        }
        iconLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(iconLabel);

        QLabel *titleLabel = new QLabel("Система чиста", &dialog);
        titleLabel->setStyleSheet(
            "font-size: 28pt; "
            "font-weight: bold; "
            "color: #27ae60; "
            "margin: 10px 0;"
        );
        titleLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(titleLabel);

        QLabel *infoLabel = new QLabel(&dialog);
        infoLabel->setText("Угрозы не обнаружены");
        infoLabel->setStyleSheet("font-size: 14pt; color: #bdc3c7;");
        infoLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(infoLabel);

        QLabel *scannedLabel = new QLabel(&dialog);
        scannedLabel->setText(QString("Просканировано файлов: %1").arg(totalFiles));
        scannedLabel->setStyleSheet("font-size: 12pt; color: #95a5a6; margin-top: 10px;");
        scannedLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(scannedLabel);

        layout->addStretch();

        QPushButton *okButton = new QPushButton("OK", &dialog);
        okButton->setMinimumSize(150, 50);
        okButton->setStyleSheet(
            "QPushButton {"
            "  background-color: #27ae60;"
            "  color: white;"
            "  border: none;"
            "  border-radius: 8px;"
            "  font-size: 16pt;"
            "  font-weight: bold;"
            "  padding: 10px;"
            "}"
            "QPushButton:hover {"
            "  background-color: #229954;"
            "}"
            "QPushButton:pressed {"
            "  background-color: #1e8449;"
            "}"
        );
        connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);

        QHBoxLayout *buttonLayout = new QHBoxLayout();
        buttonLayout->addStretch();
        buttonLayout->addWidget(okButton);
        buttonLayout->addStretch();
        layout->addLayout(buttonLayout);

        dialog.exec();
    }
};
