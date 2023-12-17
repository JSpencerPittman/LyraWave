#include "Amplitudes.h"

Amplitudes::Amplitudes(QObject *parent) : QObject(parent) {
    QFile file(":/Audio/tropicLove.wav");
    file.open(QIODevice::ReadOnly);
    QByteArray dataByteArray = file.readAll();
    std::vector<uint8_t> rawByteVec(dataByteArray.begin(), dataByteArray.end());
    m_audioFile.loadFromMemory(rawByteVec);

    m_sampleRate = m_audioFile.getSampleRate();
    m_numSamples = m_audioFile.getNumSamplesPerChannel();

    m_shiftMoveAvg.setCapacity(MOVINGAVG_WINDOWSIZE);
}

QVector<double> Amplitudes::y() {
    return m_yVec;
}

void Amplitudes::loadMS(int startMS, int endMS) {
    int startPos = (m_sampleRate / 1000) * startMS;
    int endPos = ((m_sampleRate / 1000) * endMS) - 1;

    if(startMS >= endMS) return;
    if(startMS < 0) return;
    if(endPos >= m_numSamples) return;

    int jump = endPos - startPos - 1 / TICKCOUNT;

    m_yVec.clear();
    for(int i = 0; i < TICKCOUNT-1; i++) {
        int idx = startPos + (i * jump);
        m_yVec.push_back(abs(m_audioFile.samples[0][idx]));
    }
    m_yVec.push_back(abs(m_audioFile.samples[0][endPos]));

    emit yUpdated();
}

void Amplitudes::initShift(int startMS, int refreshRateMS, int timeSpanMS) {
    m_shiftStartPos = (m_sampleRate / 1000) * startMS;
    m_shiftJump = (m_sampleRate / 1000) * refreshRateMS;
    m_tickCnt = timeSpanMS / refreshRateMS;

    m_shiftMoveAvg.clear();
    m_shiftCount = 0;

    m_yVec.clear();
    while(m_shiftCount < m_tickCnt) {
        int idx = m_shiftStartPos + (m_shiftCount * m_shiftJump);
        double val = m_audioFile.samples[0][idx];
        m_shiftMoveAvg.add(abs(val));
        m_yVec.push_back(m_shiftMoveAvg.averageDouble());
        ++m_shiftCount;
    }

    emit yUpdated();
}

void Amplitudes::shift() {
    m_yVec.pop_front();

    int idx = m_shiftStartPos + (m_shiftCount * m_shiftJump);
    double val = m_audioFile.samples[0][idx];

    m_shiftMoveAvg.add(abs(val));
    m_yVec.push_back(m_shiftMoveAvg.averageDouble());
    ++m_shiftCount;

    emit yUpdated();
}
