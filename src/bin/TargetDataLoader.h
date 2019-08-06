#pragma once

// libraries
#include <QtCore/QString>
#include <QtCore/QObject>

class TargetDataLoader : public QObject {
Q_OBJECT
public:
    TargetDataLoader(QString  target);

    void loadTargetDataIntoApplication();

private:
    QString target;

};