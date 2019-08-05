#pragma once

// libraries
#include <KJob>
#include <QtCore/QProcess>

class UninstallJob : public KJob {
Q_OBJECT
public:
    explicit UninstallJob(QString  target, QObject* parent = nullptr);

    void start() override;

protected slots:

    void onProcessFinished(int exitCode);

private:
    QString target;
    QProcess process;
};
