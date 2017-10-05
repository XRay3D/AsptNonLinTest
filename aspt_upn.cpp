#include "aspt_upn.h"

#include <QCoreApplication>
#include <QDebug>
#include <QSerialPortInfo>
#include <QThread>
#include <QVector>

int id1 = qRegisterMetaType<Device>("Device");
int id2 = qRegisterMetaType<MessageType>("MessageType");
int id3 = qRegisterMetaType<QVector<QPair<int, int> > >("QVector<QPair<int, int>>");
int id4 = qRegisterMetaType<QVector<double> >("QVector<double>");

enum {
    UptPing = 0,
    UptSetResistor = 1,
    UptWriteResistorValue = 2,
    UptReadResistorValue = 3,
};

enum {
    UptResistor1,
    UptResistor2,
    UptResistor3,
    UptResistor4,
    UptResistor5,
    UptResistor6,
    UptResistorFix
};

AsptUpn::AsptUpn(QObject* parent)
    : QSerialPort(parent)
    , beep("beep.wav", this)
    , resistors({ 150.0, 150.0, 300.0, 0.0, 0.0, 1.0 })
    , AdcCfg({ "|1|1|1|0|0|0|3|6|28|3|0|0|5|0|20|6|1|28|3|0|0|5|6|20|6|1|28|3|0|0|5|5|20|6|1|0|0|0|0|5|0|1|1|1|0|0|0|0|0|0|1|1|1|0|0|0|0|0|0|1|1|1|220|0|\0",
          "|1|1|1|0|0|0|3|6|28|3|0|0|5|1|20|6|1|28|3|0|0|5|6|20|6|1|28|3|0|0|5|5|20|6|1|0|0|0|0|5|0|1|1|1|0|0|0|0|0|0|1|1|1|0|0|0|0|0|0|1|1|1|221|0|\0" })
{
    setBaudRate(9600);
    setParity(NoParity);
}

bool AsptUpn::pingUpn(QString portName)
{
    setPortName(portName);
    bool fl(false);
    if (open(QIODevice::ReadWrite)) {
        uint8_t tmp = UptPing;
        write(Parcel(&tmp));
        waitForReadyRead(50);
        waitForReadyRead(50);
        fl = CheckData(readAll());
        close();
    }
    return fl;
}

bool AsptUpn::pingAspt(QString portName)
{
    setPortName(portName);
    bool fl(false);
    if (open(QIODevice::ReadWrite)) {
        write(QByteArray::fromHex("AA010611000000C200"));
        waitForReadyRead(50);
        waitForReadyRead(50);
        char key[] = { -86, 1, 4, 0 };
        fl = (readAll().indexOf(key) == 0);
        close();
    }
    return fl;
}

void AsptUpn::searchDevices()
{
    resetSemaphore();
    QList<QSerialPortInfo> ports(QSerialPortInfo::availablePorts());
    for (QSerialPortInfo& portInfo : ports) {
        if (checkStop())
            break;
        if (pingAspt(portInfo.portName())) {
            DevSetTieParameters(QString(portInfo.portName()), 9600, 1);
            TParameter par;
            if (DevReadParameter(dpNumber, par) == 0) {
                beep.play();
                emit deviceFound(DeviceAspt, portInfo.portName(), par.Value);
            }
        }
        emit deviceFound(DeviceProgres);
        if (checkStop())
            break;
        if (pingUpn(portInfo.portName())) {
            if (readResistorValue()) {
                beep.play();
                emit deviceFound(DeviceUpt, portInfo.portName(), resistors[5]);
            }
        }
        emit deviceFound(DeviceProgres);
    }
    emit deviceFound(DeviceStopSearch);
}

void AsptUpn::measure(const QVector<QPair<int, int> >& channels, int points)
{
    resetSemaphore();

    if (DevInitialize() != 0) {
        emit doMessage(CheckAsptConnection, 0);
        return;
    }

    for (QPair<int, int> stage : channels) {
        beep.play();
        emit doMessage(ConnectUptToAspt, stage.first);
        while (!checkStop())
            continue;
        if (checkStop()) {
            emit doMessage(TerminateCheck, 0);
            return;
        }
        if (DevCorrection() != 0) {
            emit doMessage(CheckAsptConnection, 0);
            return;
        }
        if (checkStop())
            return;
        int res;
        double v = 0.0, t = 0.0;
        for (res = 0; res < 6; ++res) {
            if (!setResistor(res)) {
                emit doMessage(CheckUptConnection, 0);
                return;
            }
            if (res == 0) {
                if (!(stage.second & 0x1)) {
                    res = 2;
                    continue;
                }
                AdcSetPack(AdcCfg.at(0));
                AdcSetMeasureChannel(stage.first);
                AdcSetSupportingResistor(sr150);
                AdcSetValuePolarityCurrent(c10);
            }
            if (res == 3) {
                if (!(stage.second & 0x2)) {
                    res = 6;
                    continue;
                }
                AdcSetPack(AdcCfg.at(1));
                AdcSetMeasureChannel(stage.first);
                AdcSetSupportingResistor(sr150);
                AdcSetValuePolarityCurrent(c10);
            }
            if (RetCode() != 0) {
                emit doMessage(CheckAsptConnection, 0);
                return;
            }
            thread()->msleep(100);
            bool fl = false;
            for (int i = 0; i < points; ++i) {
                do {
                    if (checkStop())
                        return;
                    if (DevGetMeasureValue(vtR4W, rPt500, 0.0, 0.0, 1.0, nullptr, -1, v, t) != 0) {
                        emit doMessage(CheckAsptConnection, 0);
                        return;
                    }
                    if (checkStop())
                        return;
                    qDebug() << v << t;
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
    beep.play();
    emit doMessage(CheckFinished, 0);
}

bool AsptUpn::readResistorValue()
{
    uint8_t tmp[][2] = {
        { UptReadResistorValue, UptResistor1 },
        { UptReadResistorValue, UptResistor2 },
        { UptReadResistorValue, UptResistor3 },
        { UptReadResistorValue, UptResistor4 },
        { UptReadResistorValue, UptResistor5 },
        { UptReadResistorValue, UptResistor6 }
    };
    bool fl(false);
    if (open(QIODevice::ReadWrite)) {
        for (int i = 0; i < 6; ++i) {
            write(Parcel(tmp[i], 2));
            waitForReadyRead(50);
            data = readAll();
            if (CheckData(data)) {
                resistors[i] = *(double*)&data.data()[3];
                thread()->msleep(50);
                fl = true;
            }
            else {
                fl = false;
                break;
            }
        }
        close();
    }
    return fl;
}

bool AsptUpn::writeResistorValue()
{
    bool fl(false);
    if (open(QIODevice::ReadWrite)) {
        for (uint8_t i = 0; i < 6; ++i) {
            uint8_t tmp[] = { UptWriteResistorValue, i, 1, 2, 3, 4, 5, 6, 7, 8 };
            *(double*)(tmp + 2) = resistors[i];
            write(Parcel(tmp, 10));
            waitForReadyRead(50);
            if (CheckData(readAll())) {
                fl = true;
                thread()->msleep(50);
            }
            else {
                fl = false;
                break;
            }
        }
        if (fl) {
            uint8_t tmp[] = { UptWriteResistorValue, 6 };
            write(Parcel(tmp, 2));
            waitForReadyRead(50);
            if (CheckData(readAll())) {
                fl = true;
            }
        }
        close();
    }
    return fl;
}

bool AsptUpn::setResistor(int ResNum)
{
    enum Rel {
        R1 = 1,
        R2 = 4,
        R3 = 2,
        C0 = 8,
        C1 = 16
    };
    uint8_t d[][2] = {
        { UptSetResistor, C0 | R1 },
        { UptSetResistor, C0 | R2 },
        { UptSetResistor, C0 | R3 },
        { UptSetResistor, C1 | R1 },
        { UptSetResistor, C1 | R2 },
        { UptSetResistor, C1 | R3 }
    };
    bool fl(false);
    if (open(QIODevice::ReadWrite)) {
        write(Parcel(d[ResNum], 2));
        waitForReadyRead(10000);
        waitForReadyRead(50);
        data = readAll();
        if (CheckData(data)) {
            fl = true;
        }
        close();
    }
    return fl;
}

void AsptUpn::stopWork()
{
    semaphore.release();
}

QVector<double>& AsptUpn::getResistos() { return resistors; }

bool AsptUpn::checkStop()
{
    thread()->msleep(100);
    QCoreApplication::processEvents(QEventLoop::AllEvents); //call void AsptUpt::stopWork()
    return semaphore.tryAcquire();
}

void AsptUpn::resetSemaphore()
{
    if (semaphore.available())
        semaphore.acquire(semaphore.available());
}

QStringList AsptUpn::getAdcCfg() const
{
    return AdcCfg;
}

void AsptUpn::setAdcCfg(const QStringList& value)
{
    AdcCfg = value;
}
