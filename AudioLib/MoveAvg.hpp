#ifndef MOVE_AVG_H
#define MOVE_AVG_H

#include <queue>
#include <stdio.h>

template <class T>
class MovingAverage {
public:
    MovingAverage(size_t capacity=0);

    size_t capacity();
    size_t size();
    bool empty();

    T average();
    double averageDouble();

    void add(T val);

    void clear();
    void setCapacity(size_t capacity);

private:
    void pop();

    size_t m_siCapacity;
    size_t m_siSize;

    T m_tSum;
    std::queue<T> m_qHistory;
};

template <class T>
MovingAverage<T>::MovingAverage(size_t capacity) {
    m_siCapacity = capacity;
    m_siSize = 0;

    m_tSum = 0;
}

template <class T>
size_t MovingAverage<T>::capacity() {
    return m_siCapacity;
}

template <class T>
size_t MovingAverage<T>::size() {
    return m_siSize;
}

template <class T>
bool MovingAverage<T>::empty() {
    return m_siSize == 0;
}

template <class T>
T MovingAverage<T>::average() {
    if(empty()) return 0;
    return m_tSum / T(m_siSize);
}

template <class T>
double MovingAverage<T>::averageDouble() {
    if(empty()) return 0.0;
    return double(m_tSum) / double(m_siSize);
}

template <class T>
void MovingAverage<T>::add(T val) {
    // If at capacity remove an element
    if(m_siSize == m_siCapacity) pop();

    // Add the new element
    m_qHistory.push(val);
    m_tSum += val;
    ++m_siSize;
}

template <class T>
void MovingAverage<T>::clear() {
    while(!m_qHistory.empty()) m_qHistory.pop();
    m_tSum = 0;
    m_siSize = 0;
}

template <class T>
void MovingAverage<T>::setCapacity(size_t capacity) {
    while(m_siSize > capacity) pop();
    m_siCapacity = capacity;
}

template <class T>
void MovingAverage<T>::pop() {
    m_tSum -= m_qHistory.front();
    m_qHistory.pop();
    --m_siSize;
}

#endif
