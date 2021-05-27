#pragma once

#include <QSemaphore>
#include <QSound>

enum eMessageType {
    ConnectUptToAspt,
    CheckFinished,
    TerminateCheck,
    CheckUptConnection,
    CheckAsptConnection,
    CheckUptToAsptConnection,
};

class Measure : public QObject {
    Q_OBJECT

public:
    explicit Measure(QObject* parent = 0);

    void measure(const QVector<QPair<int, int>>& channels, int points);
    void stopWork(int count);

signals:
    void measureReady(const double value, int ch, int r, int num);
    void doMessage(eMessageType msgType, int row);

private:
    bool checkStop();
    void resetSemaphore();
    int connectUpt(eMessageType msgType, int row);

    const QString m_AdcCfgList[2];
    QSound m_beep;
    QSemaphore m_semaphore;
};
