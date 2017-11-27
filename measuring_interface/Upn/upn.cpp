#include "upn.h"

#include <QSerialPortInfo>
#include <QDebug>
#include <QTimer>

#define DBG 1

enum Relay {
    R1 = 1,
    R2 = 4,
    R3 = 2,
    C0 = 8,
    C1 = 16,
    Rl1 = C0 | R1,
    Rl2 = C0 | R2,
    Rl3 = C0 | R3,
    Rl4 = C1 | R1,
    Rl5 = C1 | R2,
    Rl6 = C1 | R3
};

enum COMMAND {
    PING,

    SET_RESISTOR,
    WRITE_RESISTOR_VALUE,
    READ_RESISTOR_VALUE,

    BUFFER_OVERFLOW = 0xDF,
    WRONG_COMMAND = 0xEF,
    TEXTUAL_PARCEL = 0xFF,
    CRC_ERROR = 0xCF
};

Upn::Upn(QObject* parent)
    : QObject(parent)
    , m_resistors({ 150.0, 150.0, 300.0, 0.0, 0.0, 1.0 })
{
    m_port = new UpnPort(this);
    m_port->moveToThread(&m_portThread);

    connect(&m_portThread, &QThread::finished, m_port, &QObject::deleteLater);

    connect(this, &Upn::Close, m_port, &UpnPort::Close);
    connect(this, &Upn::Open, m_port, &UpnPort::Open);
    connect(this, &Upn::Write, m_port, &UpnPort::Write);

    m_portThread.start(QThread::NormalPriority);
}

Upn::~Upn()
{
    m_portThread.quit();
    m_portThread.wait();
}

bool Upn::Ping(const QString& PortName)
{
    QMutexLocker Locker(&m_mutex);
    m_connected = false;
    m_semaphore.acquire(m_semaphore.available());
    do {
        emit Close();
        if (!m_semaphore.tryAcquire(1, 1000))
            break;

        if (!PortName.isEmpty())
            m_port->setPortName(PortName);

        PortControl control(this);

        //        emit Open(QSerialPort::ReadWrite);
        //        if (!m_semaphore.tryAcquire(1, 1000))
        //            break;

        emit Write(Parcel(PING));
        if (!m_semaphore.tryAcquire(1, 100))
            break;

        m_connected = true;
    } while (0);

    //    emit Close();
    //    m_semaphore.tryAcquire(1, 1000);

    return m_connected;
}

bool Upn::setResistor(int r)
{
    QMutexLocker Locker(&m_mutex);
    PortControl control(this);
    const quint8 m_relay[] = { Rl1, Rl2, Rl3, Rl4, Rl5, Rl6 };
    do {
        if (!IsConnected())
            break;
        emit Write(Parcel<quint8>(SET_RESISTOR, m_relay[r]));
        if (m_semaphore.tryAcquire(1, 100))
            m_result = true;
    } while (0);
    return m_result;
}

bool Upn::writeResistorValue(const QVector<double>& r)
{
    QMutexLocker Locker(&m_mutex);
    PortControl control(this);
    do {
        if (!IsConnected())
            break;
        for (quint8 i = 0; i < 6; ++i) {
            WriteResistor_t t = { i, r[i] };
            emit Write(Parcel<WriteResistor_t>(WRITE_RESISTOR_VALUE, t));
            if (!m_semaphore.tryAcquire(1, 100))
                return m_result;
        }
        emit Write(Parcel<quint8>(WRITE_RESISTOR_VALUE, 6));
        if (!m_semaphore.tryAcquire(1, 100))
            break;
        m_resistors = r;
        m_result = true;
    } while (0);

    return m_result;
}

bool Upn::readResistorValue()
{
    QMutexLocker Locker(&m_mutex);
    PortControl control(this);
    do {
        if (!IsConnected())
            break;
        for (quint8 r = 0; r < 6; ++r) {
            emit Write(Parcel<quint8>(READ_RESISTOR_VALUE, r));
            if (!m_semaphore.tryAcquire(1, 1000))
                break;
            m_resistors[r] = *reinterpret_cast<double*>(&m_data.data()[3]);
        }
        m_result = true;
    } while (0);
    return m_result;
}

QVector<double> Upn::resistors() const
{
    return m_resistors;
}

void Upn::RxPing(const QByteArray& data)
{
    Q_UNUSED(data)
    m_semaphore.release();
}

void Upn::RxSetResistor(const QByteArray& data)
{
    Q_UNUSED(data)
    m_semaphore.release();
}

void Upn::RxWriteResistorValue(const QByteArray& data)
{
    Q_UNUSED(data)
    m_semaphore.release();
}

void Upn::RxReadResistorValue(const QByteArray& data)
{
    Q_UNUSED(data)
    m_semaphore.release();
}

void Upn::RxBufferOverflow(const QByteArray& data)
{
    if (DBG)
        qDebug() << "BUFFER_OVERFLOW" << data.toHex().toUpper();
    else
        Q_UNUSED(data)
    //m_semaphore.release();
}

void Upn::RxWrongCommand(const QByteArray& data)
{
    if (DBG)
        qDebug() << "WRONG_COMMAND" << data.toHex().toUpper();
    else
        Q_UNUSED(data)
    //m_semaphore.release();
}

void Upn::RxTextualParcel(const QByteArray& data)
{
    if (DBG)
        qDebug() << "TEXTUAL_PARCEL" << data.toHex().toUpper();
    else
        Q_UNUSED(data)
    //m_semaphore.release();
}

void Upn::RxCrcError(const QByteArray& data)
{
    if (DBG)
        qDebug() << "CRC_ERROR" << data.toHex().toUpper();
    else
        Q_UNUSED(data)
    //m_semaphore.release();
}

void Upn::RxNullFunction(const QByteArray& data)
{
    m_data = data;
    m_semaphore.release();
}

bool Upn::IsConnected() const
{
    m_semaphore.acquire(m_semaphore.available());
    m_result = false;
    return m_connected;
}

////////////////////////////////////////////
/// \brief SerialPort::SerialPort
/// \param upn
///
UpnPort::UpnPort(Upn* upn)
    : m_func(0x100, &Upn::RxNullFunction)
    , m_upn(upn)
{
    m_func[/*PING*/ 0x37] = &Upn::RxPing;
    m_func[/*SET_RESISTOR*/ Rl1] = &Upn::RxSetResistor;
    m_func[/*SET_RESISTOR*/ Rl2] = &Upn::RxSetResistor;
    m_func[/*SET_RESISTOR*/ Rl3] = &Upn::RxSetResistor;
    m_func[/*SET_RESISTOR*/ Rl4] = &Upn::RxSetResistor;
    m_func[/*SET_RESISTOR*/ Rl5] = &Upn::RxSetResistor;
    m_func[/*SET_RESISTOR*/ Rl6] = &Upn::RxSetResistor;
    m_func[/*WRITE_RESISTOR_VALUE*/ 0x01] = &Upn::RxWriteResistorValue;
    m_func[READ_RESISTOR_VALUE] = &Upn::RxReadResistorValue;
    m_func[BUFFER_OVERFLOW] = &Upn::RxBufferOverflow;
    m_func[WRONG_COMMAND] = &Upn::RxWrongCommand;
    m_func[TEXTUAL_PARCEL] = &Upn::RxTextualParcel;
    m_func[CRC_ERROR] = &Upn::RxCrcError;

    setBaudRate(QSerialPort::Baud9600);
    setParity(QSerialPort::NoParity);
    setFlowControl(QSerialPort::NoFlowControl);
    connect(this, &QSerialPort::readyRead, this, &UpnPort::ReadyRead);
}

void UpnPort::Close()
{
    close();
    m_upn->m_semaphore.release();
}

void UpnPort::Open(int mode)
{
    if (open(static_cast<QIODevice::OpenMode>(mode)))
        m_upn->m_semaphore.release();
}

void UpnPort::Write(const QByteArray& data)
{
    write(data);
}

void UpnPort::ReadyRead()
{
    QMutexLocker Locker(&m_mutex);
    m_data.append(readAll());
    for (int i = 0; i < m_data.size() - 3; ++i) {
        const Parcel_tp* p = reinterpret_cast<const Parcel_tp*>(m_data.constData() + i);
        if (p->Start == ReceiveStartPattern) {
            if (p->Size <= m_data.size()) {
                QByteArray tmp = m_data.mid(i, p->Size);
                if (CheckData(tmp))
                    (m_upn->*m_func[p->Command])(tmp);
                else
                    (m_upn->*m_func[CRC_ERROR])(tmp);
                m_data.remove(0, i + p->Size);
                i = 0;
            }
        }
    }
}
