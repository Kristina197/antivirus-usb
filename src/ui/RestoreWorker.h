#pragma once
#include <QObject>
#include <QString>
#include <memory>
#include "../usecases/QuarantineManager.h"

class RestoreWorker : public QObject {
    Q_OBJECT
    
public:
    RestoreWorker(std::shared_ptr<QuarantineManager> manager,
                  const QString& filePath,
                  const QString& restoreDir)
        : manager_(manager), filePath_(filePath), restoreDir_(restoreDir) {}
    
public slots:
    void doRestore() {
        bool success = manager_->restoreFile(
            filePath_.toStdString(), 
            restoreDir_.toStdString()
        );
        emit restoreFinished(success);
    }
    
signals:
    void restoreFinished(bool success);
    
private:
    std::shared_ptr<QuarantineManager> manager_;
    QString filePath_;
    QString restoreDir_;
};
