#include "QuarantineDialog.h"
#include <QMessageBox>
#include <QHeaderView>
#include <QFileInfo>
#include <QDateTime>
#include <QFileDialog>
#include <QDir>
#include <QApplication>
#include <filesystem>
#include <iostream>

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
    setWindowTitle("Quarantine Manager");
    setMinimumSize(700, 400);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    QLabel* titleLabel = new QLabel("Quarantined Files");
    QFont titleFont;
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    mainLayout->addWidget(titleLabel);
    
    filesTable_ = new QTableWidget();
    filesTable_->setColumnCount(4);
    filesTable_->setHorizontalHeaderLabels({"Filename", "Original Path", "Date", "Size"});
    filesTable_->horizontalHeader()->setStretchLastSection(true);
    filesTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    filesTable_->setSelectionMode(QAbstractItemView::SingleSelection);
    filesTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mainLayout->addWidget(filesTable_);
    
    statusLabel_ = new QLabel("Loading...");
    mainLayout->addWidget(statusLabel_);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    
    restoreButton_ = new QPushButton("Restore Selected...");
    restoreButton_->setEnabled(false);
    connect(restoreButton_, &QPushButton::clicked, this, &QuarantineDialog::onRestoreClicked);
    buttonLayout->addWidget(restoreButton_);
    
    deleteButton_ = new QPushButton("Delete Selected");
    deleteButton_->setEnabled(false);
    connect(deleteButton_, &QPushButton::clicked, this, &QuarantineDialog::onDeleteClicked);
    buttonLayout->addWidget(deleteButton_);
    
    refreshButton_ = new QPushButton("Refresh");
    connect(refreshButton_, &QPushButton::clicked, this, &QuarantineDialog::onRefreshClicked);
    buttonLayout->addWidget(refreshButton_);
    
    buttonLayout->addStretch();
    
    QPushButton* closeButton = new QPushButton("Close");
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
        statusLabel_->setText("Quarantine folder is empty");
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
                    sizeStr = QString::number(size) + " B";
                } else if (size < 1024 * 1024) {
                    sizeStr = QString::number(size / 1024.0, 'f', 2) + " KB";
                } else {
                    sizeStr = QString::number(size / (1024.0 * 1024.0), 'f', 2) + " MB";
                }
                filesTable_->setItem(row, 3, new QTableWidgetItem(sizeStr));
                
                filesTable_->item(row, 0)->setData(Qt::UserRole, 
                    QString::fromStdString(entry.path().string()));
                
                fileCount++;
            }
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to load quarantine: %1").arg(e.what()));
    }
    
    statusLabel_->setText(QString("Total files in quarantine: %1").arg(fileCount));
}

void QuarantineDialog::onRestoreClicked() {
    auto selectedRows = filesTable_->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) return;
    
    int row = selectedRows.first().row();
    QString filePath = filesTable_->item(row, 0)->data(Qt::UserRole).toString();
    QString filename = filesTable_->item(row, 1)->text();
    
    std::cout << "=== Restore Dialog ===" << std::endl;
    std::cout << "File path: " << filePath.toStdString() << std::endl;
    std::cout << "Filename: " << filename.toStdString() << std::endl;
    
    // Выбор директории
    QString restoreDir = QFileDialog::getExistingDirectory(
        this,
        "Select Folder to Restore File",
        QDir::homePath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );
    
    if (restoreDir.isEmpty()) {
        std::cout << "User cancelled restore" << std::endl;
        return;
    }
    
    std::cout << "Restore dir: " << restoreDir.toStdString() << std::endl;
    
    // Обновляем UI
    statusLabel_->setText("Restoring file...");
    QApplication::processEvents();
    
    // Выполняем восстановление (синхронно, но с processEvents)
    bool success = quarantineManager_->restoreFile(
        filePath.toStdString(), 
        restoreDir.toStdString()
    );
    
    QApplication::processEvents();
    
    if (success) {
        std::cout << "✓ Restore SUCCESS" << std::endl;
        QMessageBox::information(this, "Success", 
            QString("File '%1' restored to:\n%2/%1").arg(filename, restoreDir));
        loadQuarantineFiles();
    } else {
        std::cout << "✗ Restore FAILED" << std::endl;
        QMessageBox::critical(this, "Error", "Failed to restore file");
        statusLabel_->setText(QString("Total files in quarantine: %1").arg(filesTable_->rowCount()));
    }
}

void QuarantineDialog::onRestoreFinished(bool success) {
    // Не используется в упрощённой версии
}

void QuarantineDialog::onDeleteClicked() {
    auto selectedRows = filesTable_->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) return;
    
    int row = selectedRows.first().row();
    QString filePath = filesTable_->item(row, 0)->data(Qt::UserRole).toString();
    QString filename = filesTable_->item(row, 0)->text();
    
    QString message = QString("Permanently delete '%1'?\n\n⚠️  This action cannot be undone!")
                     .arg(filename);
    
    QMessageBox::StandardButton reply = QMessageBox::warning(
        this, "Delete File", message,
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        statusLabel_->setText("Deleting file...");
        QApplication::processEvents();
        
        if (quarantineManager_->deleteFile(filePath.toStdString())) {
            QMessageBox::information(this, "Success", "File permanently deleted");
            loadQuarantineFiles();
        } else {
            QMessageBox::critical(this, "Error", "Failed to delete file");
            statusLabel_->setText(QString("Total files in quarantine: %1").arg(filesTable_->rowCount()));
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
