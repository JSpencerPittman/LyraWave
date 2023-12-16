#ifndef AMPLITUDES_H
#define AMPLITUDES_H

#include <QObject>
#include <qqml.h>

#include "Wav.h"

class Amplitudes : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVector<double> y READ y NOTIFY yUpdated)
    QML_ELEMENT

public:
    Amplitudes(QObject *parent = nullptr);

    QVector<double> y();

    Q_INVOKABLE void increment();

signals:
    void yUpdated();

private:
    QVector<double> m_yVec;
    AudioFile<double> m_audioFile;
};

#endif
