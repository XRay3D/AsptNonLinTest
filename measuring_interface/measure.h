#pragma once


#include <QSemaphore>
#include <QSound>

enum eDevice {
    DeviceAspt,
    DeviceUpt,
    DeviceProgres,
    DeviceStopSearch,
};

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
    void searchDevices();
    void stopWork(int count);

signals:
    void deviceFound(eDevice device, const QString& portName = "", double num = 0.0);
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


