#pragma once

// libraries
#include <KJob>
#include <QtCore/QProcess>

class InstallJob : public KJob {
Q_OBJECT
public:
    explicit InstallJob(QString  target, QObject* parent = nullptr);

    void start() override;

protected slots:

    void onProcessFinished(int exitCode);

private:
    QString target;
    QProcess process;
};
