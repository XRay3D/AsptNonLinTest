#ifndef MEASURE_H
#define MEASURE_H

#include <QSemaphore>
#include <QSerialPort>
#include <QSound>
#include <QVector>

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

class Measure : public QSerialPort /*, public Device, public Upn*/ {
    Q_OBJECT
public:
    explicit Measure(QObject* parent = 0);

    void measure(const QVector<QPair<int, int> >& channels, int points);
    void searchDevices();
    void stopWork();

    void setAdcCfg(const QStringList& value);
    QStringList getAdcCfg() const;

signals:
    void deviceFound(eDevice device, const QString& portName = "", double num = 0.0);
    void measureReady(const double value, int ch, int r, int num);
    void doMessage(eMessageType msgType, int row);

private:
    bool checkStop();
    void resetSemaphore();

    QStringList m_AdcCfgList;
    QSound m_beep;
    QSemaphore m_semaphore;
};

#endif // MEASURE_H
