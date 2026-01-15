#include "SettingsDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <sstream>
#include <algorithm>

SettingsDialog::SettingsDialog(ScanConfig& config, QWidget *parent)
    : QDialog(parent), config_(config) {
    setupUI();
    loadSettings();
}

void SettingsDialog::setupUI() {
    setWindowTitle("Scan Settings");
    setMinimumWidth(500);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Группа "Scanning Limits"
    QGroupBox* limitsGroup = new QGroupBox("Scanning Limits");
    QFormLayout* limitsLayout = new QFormLayout();
    
    maxDepthSpinBox_ = new QSpinBox();
    maxDepthSpinBox_->setRange(1, 100);
    maxDepthSpinBox_->setToolTip("Maximum directory recursion depth (1-100)");
    limitsLayout->addRow("Max Directory Depth:", maxDepthSpinBox_);
    
    maxFileSizeSpinBox_ = new QSpinBox();
    maxFileSizeSpinBox_->setRange(0, 10240);
    maxFileSizeSpinBox_->setSuffix(" MB");
    maxFileSizeSpinBox_->setSpecialValueText("No limit");
    maxFileSizeSpinBox_->setToolTip("Maximum file size to scan (0 = no limit)");
    limitsLayout->addRow("Max File Size:", maxFileSizeSpinBox_);
    
    limitsGroup->setLayout(limitsLayout);
    mainLayout->addWidget(limitsGroup);
    
    // Группа "File Options"
    QGroupBox* optionsGroup = new QGroupBox("File Options");
    QVBoxLayout* optionsLayout = new QVBoxLayout();
    
    scanHiddenCheckBox_ = new QCheckBox("Scan hidden files");
    scanHiddenCheckBox_->setToolTip("Scan files starting with dot (.)");
    optionsLayout->addWidget(scanHiddenCheckBox_);
    
    followSymlinksCheckBox_ = new QCheckBox("Follow symbolic links");
    followSymlinksCheckBox_->setToolTip("Follow symlinks during directory scanning");
    optionsLayout->addWidget(followSymlinksCheckBox_);
    
    optionsGroup->setLayout(optionsLayout);
    mainLayout->addWidget(optionsGroup);
    
    // Группа "File Types"
    QGroupBox* typesGroup = new QGroupBox("File Types to Scan");
    QVBoxLayout* typesLayout = new QVBoxLayout();
    
    QLabel* infoLabel = new QLabel("Enter file extensions separated by commas (e.g., .exe,.dll,.pdf)\nLeave empty to scan all files");
    infoLabel->setWordWrap(true);
    infoLabel->setStyleSheet("color: gray; font-size: 10pt;");
    typesLayout->addWidget(infoLabel);
    
    extensionsLineEdit_ = new QLineEdit();
    extensionsLineEdit_->setPlaceholderText(".exe, .dll, .so, .pdf, .doc, .zip");
    typesLayout->addWidget(extensionsLineEdit_);
    
    typesGroup->setLayout(typesLayout);
    mainLayout->addWidget(typesGroup);
    
    // Кнопки
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    
    saveButton_ = new QPushButton("Save");
    cancelButton_ = new QPushButton("Cancel");
    
    buttonLayout->addWidget(saveButton_);
    buttonLayout->addWidget(cancelButton_);
    
    mainLayout->addLayout(buttonLayout);
    
    // Соединения
    connect(saveButton_, &QPushButton::clicked, this, &SettingsDialog::onSaveClicked);
    connect(cancelButton_, &QPushButton::clicked, this, &SettingsDialog::onCancelClicked);
}

void SettingsDialog::loadSettings() {
    maxDepthSpinBox_->setValue(config_.maxDepth);
    maxFileSizeSpinBox_->setValue(config_.maxFileSizeMB);
    scanHiddenCheckBox_->setChecked(config_.scanHiddenFiles);
    followSymlinksCheckBox_->setChecked(config_.followSymlinks);
    
    // Формируем строку расширений
    std::string extStr;
    for (size_t i = 0; i < config_.allowedExtensions.size(); ++i) {
        extStr += config_.allowedExtensions[i];
        if (i < config_.allowedExtensions.size() - 1) {
            extStr += ", ";
        }
    }
    extensionsLineEdit_->setText(QString::fromStdString(extStr));
}

void SettingsDialog::saveSettings() {
    config_.maxDepth = maxDepthSpinBox_->value();
    config_.maxFileSizeMB = maxFileSizeSpinBox_->value();
    config_.scanHiddenFiles = scanHiddenCheckBox_->isChecked();
    config_.followSymlinks = followSymlinksCheckBox_->isChecked();
    
    // Парсим расширения
    config_.allowedExtensions.clear();
    QString extText = extensionsLineEdit_->text().trimmed();
    
    if (!extText.isEmpty()) {
        QStringList extList = extText.split(',', Qt::SkipEmptyParts);
        for (const QString& ext : extList) {
            QString trimmed = ext.trimmed();
            if (!trimmed.isEmpty()) {
                std::string extStr = trimmed.toStdString();
                // Добавляем точку если её нет
                if (extStr[0] != '.') {
                    extStr = "." + extStr;
                }
                // Приводим к нижнему регистру
                std::transform(extStr.begin(), extStr.end(), extStr.begin(), ::tolower);
                config_.allowedExtensions.push_back(extStr);
            }
        }
    }
}

void SettingsDialog::onSaveClicked() {
    saveSettings();
    accept();
}

void SettingsDialog::onCancelClicked() {
    reject();
}
