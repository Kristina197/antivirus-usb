#include "QuarantineDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QFileDialog>
#include <QApplication>
#include <QDateTime>
#include <QDir>
#include <filesystem>

namespace fs = std::filesystem;

QuarantineDialog::QuarantineDialog(std::shared_ptr<QuarantineManager> quarantineManager,
                                   QWidget *parent)
    : QDialog(parent), quarantineManager_(quarantineManager), restoreThread_(nullptr) {
    setupUI();
    loadQuarantineFiles();
}

QuarantineDialog::~QuarantineDialog() {
    if (restoreThread_ && restoreThread_->isRunning()) {
        restoreThread_->quit();
        restoreThread_->wait(3000);
    }
}

void QuarantineDialog::setupUI() {
    setWindowTitle("Менеджер карантина");
    setMinimumSize(700, 400);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QLabel* titleLabel = new QLabel("Файлы в карантине");
    QFont titleFont;
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    mainLayout->addWidget(titleLabel);

    filesTable_ = new QTableWidget();
    filesTable_->setColumnCount(4);
    filesTable_->setHorizontalHeaderLabels({"Имя файла", "Исходный путь", "Дата", "Размер"});
    filesTable_->horizontalHeader()->setStretchLastSection(true);
    filesTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    filesTable_->setSelectionMode(QAbstractItemView::SingleSelection);
    filesTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mainLayout->addWidget(filesTable_);

    statusLabel_ = new QLabel("Загрузка...");
    mainLayout->addWidget(statusLabel_);

    QHBoxLayout* buttonLayout = new QHBoxLayout();

    restoreButton_ = new QPushButton("Восстановить выбранное");
    restoreButton_->setEnabled(false);
    connect(restoreButton_, &QPushButton::clicked, this, &QuarantineDialog::onRestoreClicked);
    buttonLayout->addWidget(restoreButton_);

    deleteButton_ = new QPushButton("Удалить выбранное");
    deleteButton_->setEnabled(false);
    connect(deleteButton_, &QPushButton::clicked, this, &QuarantineDialog::onDeleteClicked);
    buttonLayout->addWidget(deleteButton_);

    refreshButton_ = new QPushButton("Обновить");
    connect(refreshButton_, &QPushButton::clicked, this, &QuarantineDialog::onRefreshClicked);
    buttonLayout->addWidget(refreshButton_);

    buttonLayout->addStretch();

    QPushButton* closeButton = new QPushButton("Закрыть");
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
    buttonLayout->addWidget(closeButton);

    mainLayout->addLayout(buttonLayout);

    connect(filesTable_, &QTableWidget::itemSelectionChanged,
            this, &QuarantineDialog::onSelectionChanged);
}

void QuarantineDialog::loadQuarantineFiles() {
    filesTable_->setRowCount(0);

    std::string quarantinePath = quarantineManager_->getQuarantinePath();
    if (!fs::exists(quarantinePath)) {
        statusLabel_->setText("Папка карантина пуста");
        return;
    }

    int fileCount = 0;
    try {
        for (const auto& entry : fs::directory_iterator(quarantinePath)) {
            if (entry.is_regular_file()) {
                int row = filesTable_->rowCount();
                filesTable_->insertRow(row);

                QString filename = QString::fromStdString(entry.path().filename().string());
                filesTable_->setItem(row, 0, new QTableWidgetItem(filename));

                QString originalName = filename;
                std::string suffix = ".quarantine";
                if (filename.contains("_") &&
                    filename.size() >= suffix.size() &&
                    filename.toStdString().compare(filename.size() - suffix.size(), suffix.size(), suffix) == 0) {
                    int lastUnderscore = filename.lastIndexOf('_');
                    originalName = filename.left(lastUnderscore);
                }

                filesTable_->setItem(row, 1, new QTableWidgetItem(originalName));

                auto ftime = fs::last_write_time(entry.path());
                auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                    ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now()
                );
                auto time_t = std::chrono::system_clock::to_time_t(sctp);
                QDateTime dateTime = QDateTime::fromSecsSinceEpoch(time_t);
                filesTable_->setItem(row, 2, new QTableWidgetItem(dateTime.toString("yyyy-MM-dd HH:mm:ss")));

                uintmax_t size = fs::file_size(entry.path());
                QString sizeStr;
                if (size < 1024) {
                    sizeStr = QString::number(size) + " Б";
                } else if (size < 1024 * 1024) {
                    sizeStr = QString::number(size / 1024.0, 'f', 2) + " КБ";
                } else {
                    sizeStr = QString::number(size / (1024.0 * 1024.0), 'f', 2) + " МБ";
                }

                filesTable_->setItem(row, 3, new QTableWidgetItem(sizeStr));
                filesTable_->item(row, 0)->setData(Qt::UserRole,
                    QString::fromStdString(entry.path().string()));
                fileCount++;
            }
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", QString("Не удалось загрузить карантин: %1").arg(e.what()));
    }

    statusLabel_->setText(QString("Всего файлов в карантине: %1").arg(fileCount));
}

void QuarantineDialog::onRestoreClicked() {
    auto selectedRows = filesTable_->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) return;

    int row = selectedRows.first().row();
    QString filePath = filesTable_->item(row, 0)->data(Qt::UserRole).toString();
    QString filename = filesTable_->item(row, 1)->text();

    QString restoreDir = QFileDialog::getExistingDirectory(
        this,
        "Выберите папку для восстановления файла",
        QDir::homePath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );

    if (restoreDir.isEmpty()) {
        return;
    }

    statusLabel_->setText("Восстановление файла...");
    QApplication::processEvents();

    bool success = quarantineManager_->restoreFile(
        filePath.toStdString(),
        restoreDir.toStdString()
    );

    QApplication::processEvents();

    if (success) {
        QMessageBox::information(this, "Успешно",
            QString("Файл '%1' восстановлен в:\n%2/%1").arg(filename, restoreDir));
        loadQuarantineFiles();
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось восстановить файл");
        statusLabel_->setText(QString("Всего файлов в карантине: %1").arg(filesTable_->rowCount()));
    }
}

void QuarantineDialog::onRestoreFinished(bool success) {
}

void QuarantineDialog::onDeleteClicked() {
    auto selectedRows = filesTable_->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) return;

    int row = selectedRows.first().row();
    QString filePath = filesTable_->item(row, 0)->data(Qt::UserRole).toString();
    QString filename = filesTable_->item(row, 0)->text();

    QString message = QString("Удалить '%1' навсегда?\n\nЭто действие нельзя отменить!")
        .arg(filename);

    QMessageBox::StandardButton reply = QMessageBox::warning(
        this, "Удалить файл", message,
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        statusLabel_->setText("Удаление файла...");
        QApplication::processEvents();

        if (quarantineManager_->deleteFile(filePath.toStdString())) {
            QMessageBox::information(this, "Успешно", "Файл удален навсегда");
            loadQuarantineFiles();
        } else {
            QMessageBox::critical(this, "Ошибка", "Не удалось удалить файл");
            statusLabel_->setText(QString("Всего файлов в карантине: %1").arg(filesTable_->rowCount()));
        }
    }
}

void QuarantineDialog::onRefreshClicked() {
    loadQuarantineFiles();
}

void QuarantineDialog::onSelectionChanged() {
    bool hasSelection = !filesTable_->selectionModel()->selectedRows().isEmpty();
    restoreButton_->setEnabled(hasSelection);
    deleteButton_->setEnabled(hasSelection);
}
