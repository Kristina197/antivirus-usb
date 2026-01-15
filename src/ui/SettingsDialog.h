#pragma once
#include <QDialog>
#include <QSpinBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include "../entities/ScanConfig.h"

class SettingsDialog : public QDialog {
    Q_OBJECT
    
public:
    explicit SettingsDialog(ScanConfig& config, QWidget *parent = nullptr);
    
private slots:
    void onSaveClicked();
    void onCancelClicked();
    
private:
    void setupUI();
    void loadSettings();
    void saveSettings();
    
    ScanConfig& config_;
    
    QSpinBox* maxDepthSpinBox_;
    QSpinBox* maxFileSizeSpinBox_;
    QCheckBox* scanHiddenCheckBox_;
    QCheckBox* followSymlinksCheckBox_;
    QLineEdit* extensionsLineEdit_;
    QPushButton* saveButton_;
    QPushButton* cancelButton_;
};
