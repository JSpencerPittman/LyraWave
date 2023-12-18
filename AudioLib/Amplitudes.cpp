#include "Amplitudes.h"

Amplitudes::Amplitudes(QObject *parent) : QObject(parent) {
    loadAudioFile(":/Audio/tropicLove.wav");
}

QVector<double> Amplitudes::amplitudes() {
    return m_qvAmplitude;
}

void Amplitudes::loadAudioFile(const QString& path) {
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QByteArray byteArray = file.readAll();
    std::vector<uint8_t> byteVec(byteArray.begin(), byteArray.end());
    m_afAudioFile.loadFromMemory(byteVec);

    m_nSampleRate = m_afAudioFile.getSampleRate();
    m_nNumSamples = m_afAudioFile.getNumSamplesPerChannel();

    m_maMovAvg.setCapacity(MOVINGAVG_WINDOWSIZE);

    m_dMaxSample = 0;
}

void Amplitudes::loadAmplitudes(int refreshRateMS) {
    int jump = (m_nSampleRate/1000) * refreshRateMS;
    int samples = ceil(m_nNumSamples/refreshRateMS);

    m_maMovAvg.clear();

    m_qvAmplitude.clear();
    m_qvAmplitude.reserve(samples);

    m_dMaxSample = 0;

    int idx = 0;
    while(idx < m_nNumSamples) {
        m_maMovAvg.add(abs(m_afAudioFile.samples[0][idx]));
        double val = m_maMovAvg.averageDouble();
        m_qvAmplitude.push_back(val);
        m_dMaxSample = fmax(m_dMaxSample, val);
        idx += jump;
    }

    emit amplitudeUpdated();
}

int Amplitudes::sampleRate() {
    return m_nSampleRate;
}

int Amplitudes::numSamples() {
    return m_nNumSamples;
}

double Amplitudes::maxSampleValue() {
    return m_dMaxSample;
}


