#ifndef ASPT_RELE_H
#define ASPT_RELE_H

#include "aspt.h"
#include "myprotocol.h"
#include <QSemaphore>
#include <QSerialPort>
#include <QSound>
#include <QVector>

enum Device {
    DeviceAspt,
    DeviceUpt,
    DeviceProgres,
    DeviceStopSearch,
};

enum MessageType {
    ConnectUptToAspt,
    CheckFinished,
    TerminateCheck,
    CheckUptConnection,
    CheckAsptConnection,
    CheckUptToAsptConnection,
};

class AsptUpn : public QSerialPort, public AsptClass, public MyProtocol {
    Q_OBJECT
public:
    explicit AsptUpn(QObject* parent = 0);
    bool pingAspt(QString portName);
    bool pingUpn(QString portName);

    bool readResistorValue();
    bool writeResistorValue();

    bool setResistor(int ResNum);
    void measure(const QVector<QPair<int, int> >& channels, int points);
    void searchDevices();
    void stopWork();
    QVector<double>& getResistos();
    void setAdcCfg(const QStringList& value);

    QStringList getAdcCfg() const;

signals:
    void deviceFound(Device device, const QString& portName = "", double num = 0.0);
    void measureReady(const double value, int ch, int r, int num);
    void doMessage(MessageType msgType, int row);

private:
    bool checkStop();
    void resetSemaphore();
    QStringList AdcCfg;
    QVector<double> resistors;
    QSound beep;
    QByteArray data;
    QSemaphore semaphore;
};

#endif // ASPT_RELE_H
