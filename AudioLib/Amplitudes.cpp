#include "Amplitudes.h"

Amplitudes::Amplitudes(QObject *parent) : QObject(parent) {
    // loadAudioFile(":/Audio/tropicLove.wav");
    m_maMovAvg.setCapacity(MOVINGAVG_WINDOWSIZE);
}

QVector<double> Amplitudes::amplitudes() {
    return m_qvAmplitude;
}

void Amplitudes::load(const QUrl& path, uint32_t amplitudeRecordRateMS) {
    m_arAmplitudeReader.setPath(path.toLocalFile().toStdString());
    m_arAmplitudeReader.setAmplitudeRecordRateMS(amplitudeRecordRateMS);

    std::vector<int16_t> *collectedAmplitudes = m_arAmplitudeReader.collectAmplitudes();

    m_qvAmplitude.clear();
    m_qvAmplitude.reserve(collectedAmplitudes->size());

    m_maMovAvg.clear();

    for(int i = 0; i < collectedAmplitudes->size(); i++) {
        m_maMovAvg.add(abs(collectedAmplitudes->at(i)));
        m_qvAmplitude.push_back(m_maMovAvg.averageDouble());
    }

    emit amplitudeUpdated();
}

