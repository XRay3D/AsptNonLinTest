#include "measure.h"
#include "mi.h"
#include <QCoreApplication>
#include <QSerialPortInfo>

const int id1 = qRegisterMetaType<eDevice>("eDevice");
const int id2 = qRegisterMetaType<eMessageType>("eMessageType");
const int id3 = qRegisterMetaType<QVector<QPair<int, int>>>("QVector<QPair<int, int>>");
const int id4 = qRegisterMetaType<QVector<double>>("QVector<double>");

Measure::Measure(QObject* parent)
    : QObject(parent)
    , m_AdcCfgList{
        "|1|1|1|0|0|0|3|6|28|3|0|0|5|0|20|6|1|28|3|0|0|5|6|20|6|1|28|3|0|0|5|5|20|6|1|0|0|0|0|5|0|1|1|1|0|0|0|0|0|0|1|1|1|0|0|0|0|0|0|1|1|1|220|0|\0",
        "|1|1|1|0|0|0|3|6|28|3|0|0|5|1|20|6|1|28|3|0|0|5|6|20|6|1|28|3|0|0|5|5|20|6|1|0|0|0|0|5|0|1|1|1|0|0|0|0|0|0|1|1|1|0|0|0|0|0|0|1|1|1|221|0|\0"
    }
    , m_beep("beep.wav", this)
{
}

void Measure::searchDevices()
{
    resetSemaphore();
    QList<QSerialPortInfo> ports(QSerialPortInfo::availablePorts());
    std::sort(ports.begin(), ports.end(), [](const QSerialPortInfo& a, const QSerialPortInfo& b) { return a.portName().mid(3).toInt() < b.portName().mid(3).toInt(); });
    QElapsedTimer t;
    for (QSerialPortInfo& portInfo : ports) {
        if (checkStop())
            break;
        t.start();
        if (MI::upn()->ping(portInfo.portName()) && MI::upn()->readResistorValue()) {
            emit deviceFound(DeviceUpt, portInfo.portName(), MI::upn()->resistors().last());
            MI::upn()->close();
        }
        qDebug() << "upn" << t.elapsed();

        if (checkStop())
            break;
        t.start();
        if (MI::aspt()->ping(portInfo.portName())) {
            emit deviceFound(DeviceAspt, portInfo.portName(), MI::aspt()->serialNumber());
            MI::aspt()->close();
        }
        qDebug() << "aspt" << t.elapsed();
        emit deviceFound(DeviceProgres);

        emit deviceFound(DeviceProgres);
    }
    emit deviceFound(DeviceStopSearch);
}

void Measure::measure(const QVector<QPair<int, int>>& channels, int points)
{
    resetSemaphore();

    if (MI::aspt()->initialize() != Error::ASPT_OK) {
        emit doMessage(CheckAsptConnection, 0);
        return;
    }

    for (QPair<int, int> stage : channels) {
        m_beep.play();

        if (connectUpt(ConnectUptToAspt, stage.first) == 2) {
            emit doMessage(TerminateCheck, 0);
            return;
        }

        if (MI::aspt()->correction() != Error::ASPT_OK) {
            emit doMessage(CheckAsptConnection, 0);
            return;
        }

        if (checkStop())
            return;

        int res;
        double v = 0.0;

        for (res = 0; res < 6; ++res) {
            if (!MI::upn()->setResistor(res)) {
                emit doMessage(CheckUptConnection, 0);
                return;
            }
            AdcCfg ADCCfg;
            if (res < 3) {
                if (!(stage.second & 0x1)) {
                    res = 2;
                    continue;
                }
                ADCCfg.setPack(m_AdcCfgList[0]);
            } else {
                if (!(stage.second & 0x2)) {
                    res = 6;
                    continue;
                }
                ADCCfg.setPack(m_AdcCfgList[1]);
            }

            const quint8 channel = stage.first;
            ADCCfg.setMeasureChannel({ channel, channel, channel, channel, channel, channel });

            const quint8 resistor = sr150;
            ADCCfg.setSupportingResistor({ resistor, resistor, resistor, resistor, resistor, resistor });

            const quint8 current = c10;
            ADCCfg.setValuePolarityCurrent({ current, current, current, current, current, current });

            thread()->msleep(100);
            //            bool fl = false;
            for (int i = 0; i < points; ++i) {
                //                do {
                //                    if (checkStop())
                //                        return;
                if (checkStop())
                    return;
                if (MI::aspt()->getMeasureValue(ADCCfg, vtR4W, 1.0, v) != 0) {
                    emit doMessage(CheckAsptConnection, 0);
                    return;
                }
                //                    qDebug() << v;
                //                    switch (res) {
                //                    case 2:
                //                    case 5:
                //                        if (290 > v || 310 < v) {
                //                            fl = true;
                //                            emit doMessage(CheckUptToAsptConnection, stage.first);
                //                            while (!checkStop())
                //                                continue;
                //                        } else
                //                            fl = false;
                //                        break;
                //                    default:
                //                        if (140 > v || 160 < v) {
                //                            fl = true;
                //                            emit doMessage(CheckUptToAsptConnection, stage.first);
                //                            while (!checkStop())
                //                                continue;
                //                        } else
                //                            fl = false;
                //                        break;
                //                    }
                //                    thread()->msleep(200);
                //                } while (fl);
                emit measureReady(v, stage.first, res, i);
            }
        }
    }
    m_beep.play();
    emit doMessage(CheckFinished, 0);
}

void Measure::stopWork(int count) { m_semaphore.release(count); }

bool Measure::checkStop()
{
    QCoreApplication::processEvents(QEventLoop::AllEvents);
    return m_semaphore.tryAcquire();
}

void Measure::resetSemaphore()
{
    if (m_semaphore.available())
        m_semaphore.acquire(m_semaphore.available());
}

int Measure::connectUpt(eMessageType msgType, int row)
{
    emit doMessage(msgType, row);
    int fl = 0;
    while (!fl) {
        thread()->msleep(100);
        QCoreApplication::processEvents(QEventLoop::AllEvents); //call void AsptUpt::stopWork()
        fl = m_semaphore.available();
    }
    m_semaphore.acquire(m_semaphore.available());
    return fl;
}
