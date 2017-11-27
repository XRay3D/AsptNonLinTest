#include "measure.h"
#include "measuringinterface.h"
#include <QCoreApplication>
#include <QSerialPortInfo>

static int id1 = qRegisterMetaType<eDevice>("eDevice");
static int id2 = qRegisterMetaType<eMessageType>("eMessageType");
static int id3 = qRegisterMetaType<QVector<QPair<int, int> > >("QVector<QPair<int, int>>");
static int id4 = qRegisterMetaType<QVector<double> >("QVector<double>");

Measure::Measure(QObject* parent)
    : QSerialPort(parent)
    , m_AdcCfgList({ "|1|1|1|0|0|0|3|6|28|3|0|0|5|0|20|6|1|28|3|0|0|5|6|20|6|1|28|3|0|0|5|5|20|6|1|0|0|0|0|5|0|1|1|1|0|0|0|0|0|0|1|1|1|0|0|0|0|0|0|1|1|1|220|0|\0",
          "|1|1|1|0|0|0|3|6|28|3|0|0|5|1|20|6|1|28|3|0|0|5|6|20|6|1|28|3|0|0|5|5|20|6|1|0|0|0|0|5|0|1|1|1|0|0|0|0|0|0|1|1|1|0|0|0|0|0|0|1|1|1|221|0|\0" })
    , m_beep("beep.wav", this)
{
    setBaudRate(9600);
    setParity(NoParity);
}

void Measure::searchDevices()
{
    resetSemaphore();
    QList<QSerialPortInfo> ports(QSerialPortInfo::availablePorts());
    for (QSerialPortInfo& portInfo : ports) {
        if (checkStop())
            break;
        if (MI::aspt()->Ping(portInfo.portName())) {
            m_beep.play();
            emit deviceFound(DeviceAspt, portInfo.portName(), MI::aspt()->SerialNumber());
        }
        emit deviceFound(DeviceProgres);

        if (checkStop())
            break;
        if (MI::upn()->Ping(portInfo.portName())) {
            if (MI::upn()->readResistorValue()) {
                m_beep.play();
                emit deviceFound(DeviceUpt, portInfo.portName(), MI::upn()->resistors().last());
            }
        }
        emit deviceFound(DeviceProgres);
    }
    emit deviceFound(DeviceStopSearch);
}

void Measure::measure(const QVector<QPair<int, int> >& channels, int points)
{
    resetSemaphore();

    if (MI::aspt()->Initialize() != ASPT_OK) {
        emit doMessage(CheckAsptConnection, 0);
        return;
    }

    for (QPair<int, int> stage : channels) {
        m_beep.play();
        emit doMessage(ConnectUptToAspt, stage.first);
        while (!checkStop())
            continue;
        if (checkStop()) {
            emit doMessage(TerminateCheck, 0);
            return;
        }

        if (MI::aspt()->Correction() != ASPT_OK) {
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
                ADCCfg.SetPack(m_AdcCfgList.at(0));
            }
            else {
                if (!(stage.second & 0x2)) {
                    res = 6;
                    continue;
                }
                ADCCfg.SetPack(m_AdcCfgList.at(1));
            }

            QVector<quint8> arr;
            arr.fill(static_cast<quint8>(stage.first), 6);
            ADCCfg.SetMeasureChannel(arr);
            arr.fill(sr150, 6);
            ADCCfg.SetSupportingResistor(arr);
            arr.fill(c10, 6);
            ADCCfg.SetValuePolarityCurrent(arr);

            thread()->msleep(100);
            bool fl = false;
            for (int i = 0; i < points; ++i) {
                do {
                    if (checkStop())
                        return;
                    if (MI::aspt()->GetMeasureValue(ADCCfg, vtR4W, 1.0, v) != 0) {
                        emit doMessage(CheckAsptConnection, 0);
                        return;
                    }
                    if (checkStop())
                        return;
                    qDebug() << v;
                    switch (res) {
                    case 2:
                    case 5:
                        if (290 > v || 310 < v) {
                            fl = true;
                            emit doMessage(CheckUptToAsptConnection, stage.first);
                            while (!checkStop())
                                continue;
                        }
                        else
                            fl = false;
                        break;
                    default:
                        if (140 > v || 160 < v) {
                            fl = true;
                            emit doMessage(CheckUptToAsptConnection, stage.first);
                            while (!checkStop())
                                continue;
                        }
                        else
                            fl = false;
                        break;
                    }
                    thread()->msleep(200);
                } while (fl);
                emit measureReady(v, stage.first, res, i);
            }
        }
    }
    m_beep.play();
    emit doMessage(CheckFinished, 0);
}

void Measure::stopWork()
{
    m_semaphore.release();
}

bool Measure::checkStop()
{
    //thread()->msleep(100);
    QCoreApplication::processEvents(QEventLoop::AllEvents); //call void AsptUpt::stopWork()
    return m_semaphore.tryAcquire();
}

void Measure::resetSemaphore()
{
    if (m_semaphore.available())
        m_semaphore.acquire(m_semaphore.available());
}

QStringList Measure::getAdcCfg() const
{
    return m_AdcCfgList;
}

void Measure::setAdcCfg(const QStringList& value)
{
    m_AdcCfgList = value;
}
