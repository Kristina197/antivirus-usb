#include "SettingsDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <algorithm>

SettingsDialog::SettingsDialog(ScanConfig& config, QWidget *parent)
    : QDialog(parent), config_(config) {
    setupUI();
    loadSettings();
}

void SettingsDialog::setupUI() {
    setWindowTitle("Настройки сканирования");
    setMinimumWidth(500);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QGroupBox* limitsGroup = new QGroupBox("Ограничения сканирования");
    QFormLayout* limitsLayout = new QFormLayout();

    maxDepthSpinBox_ = new QSpinBox();
    maxDepthSpinBox_->setRange(1, 100);
    maxDepthSpinBox_->setToolTip("Максимальная глубина рекурсии по директориям (1-100)");
    limitsLayout->addRow("Макс. глубина директорий:", maxDepthSpinBox_);

    maxFileSizeSpinBox_ = new QSpinBox();
    maxFileSizeSpinBox_->setRange(0, 10240);
    maxFileSizeSpinBox_->setSuffix(" МБ");
    maxFileSizeSpinBox_->setSpecialValueText("Без ограничений");
    maxFileSizeSpinBox_->setToolTip("Максимальный размер файла для сканирования (0 = без ограничений)");
    limitsLayout->addRow("Макс. размер файла:", maxFileSizeSpinBox_);

    limitsGroup->setLayout(limitsLayout);
    mainLayout->addWidget(limitsGroup);

    QGroupBox* optionsGroup = new QGroupBox("Параметры файлов");
    QVBoxLayout* optionsLayout = new QVBoxLayout();

    scanHiddenCheckBox_ = new QCheckBox("Сканировать скрытые файлы");
    scanHiddenCheckBox_->setToolTip("Сканировать файлы, начинающиеся с точки (.)");
    optionsLayout->addWidget(scanHiddenCheckBox_);

    followSymlinksCheckBox_ = new QCheckBox("Следовать символическим ссылкам");
    followSymlinksCheckBox_->setToolTip("Следовать символическим ссылкам при сканировании директорий");
    optionsLayout->addWidget(followSymlinksCheckBox_);

    optionsGroup->setLayout(optionsLayout);
    mainLayout->addWidget(optionsGroup);

    QGroupBox* typesGroup = new QGroupBox("Типы файлов для сканирования");
    QVBoxLayout* typesLayout = new QVBoxLayout();

    QLabel* infoLabel = new QLabel("Введите расширения файлов через запятую (например: .exe,.dll,.pdf)\nОставьте пустым для сканирования всех файлов");
    infoLabel->setWordWrap(true);
    infoLabel->setStyleSheet("color: gray; font-size: 10pt;");
    typesLayout->addWidget(infoLabel);

    extensionsLineEdit_ = new QLineEdit();
    extensionsLineEdit_->setPlaceholderText(".exe, .dll, .so, .pdf, .doc, .zip");
    typesLayout->addWidget(extensionsLineEdit_);

    typesGroup->setLayout(typesLayout);
    mainLayout->addWidget(typesGroup);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    saveButton_ = new QPushButton("Сохранить");
    cancelButton_ = new QPushButton("Отмена");

    buttonLayout->addWidget(saveButton_);
    buttonLayout->addWidget(cancelButton_);

    mainLayout->addLayout(buttonLayout);

    connect(saveButton_, &QPushButton::clicked, this, &SettingsDialog::onSaveClicked);
    connect(cancelButton_, &QPushButton::clicked, this, &SettingsDialog::onCancelClicked);
}

void SettingsDialog::loadSettings() {
    maxDepthSpinBox_->setValue(config_.maxDepth);
    maxFileSizeSpinBox_->setValue(config_.maxFileSizeMB);
    scanHiddenCheckBox_->setChecked(config_.scanHiddenFiles);
    followSymlinksCheckBox_->setChecked(config_.followSymlinks);

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

    config_.allowedExtensions.clear();
    QString extText = extensionsLineEdit_->text().trimmed();

    if (!extText.isEmpty()) {
        QStringList extList = extText.split(',', Qt::SkipEmptyParts);
        for (const QString& ext : extList) {
            QString trimmed = ext.trimmed();
            if (!trimmed.isEmpty()) {
                std::string extStr = trimmed.toStdString();
                if (extStr[0] != '.') {
                    extStr = "." + extStr;
                }
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
