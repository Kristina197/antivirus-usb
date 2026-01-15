#pragma once
#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QThread>
#include <memory>
#include "../usecases/QuarantineManager.h"

class QuarantineDialog : public QDialog {
    Q_OBJECT
    
public:
    explicit QuarantineDialog(std::shared_ptr<QuarantineManager> quarantineManager, 
                             QWidget *parent = nullptr);
    ~QuarantineDialog();
    
private slots:
    void onRestoreClicked();
    void onDeleteClicked();
    void onRefreshClicked();
    void onSelectionChanged();
    void onRestoreFinished(bool success);
    
private:
    void setupUI();
    void loadQuarantineFiles();
    
    std::shared_ptr<QuarantineManager> quarantineManager_;
    QTableWidget* filesTable_;
    QPushButton* restoreButton_;
    QPushButton* deleteButton_;
    QPushButton* refreshButton_;
    QLabel* statusLabel_;
    QThread* restoreThread_;
    QString pendingRestoreFile_;
};
