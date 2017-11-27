#include "aspt.h"

#include <QDate>
#include <QDebug>
#include <QElapsedTimer>
#include <QSerialPort>

enum { dbg = 1 };

////////////////////////////////////////////
/// \brief Aspt::Aspt
/// \param parent
///
Aspt::Aspt(QObject* parent)
    : QObject(parent)
{
    m_port = new AsptPort(this);
    m_port->moveToThread(&m_portThread);
    connect(&m_portThread, &QThread::finished, m_port, &QObject::deleteLater);
    connect(this, &Aspt::Close, m_port, &AsptPort::Close);
    connect(this, &Aspt::Open, m_port, &AsptPort::Open);
    connect(this, &Aspt::Write, m_port, &AsptPort::Write);
    connect(this, &Aspt::Read, m_port, &AsptPort::Read);
    m_portThread.start(QThread::NormalPriority);
}

Aspt::~Aspt()
{
    m_portThread.quit();
    m_portThread.wait();
}

bool Aspt::Ping(const QString& portName)
{
    QMutexLocker Locker(&m_mutex);
    m_connected = false;
    m_semaphore.acquire(m_semaphore.available());
    do {
        emit Close();
        if (!m_semaphore.tryAcquire(1, 1000))
            break;

        if (!portName.isEmpty())
            m_port->setPortName(portName);

        Parameter_t<quint32> par;
        if (ReadParameter(dpNumber, par) != ASPT_OK)
            break;
        m_serialNumber = par.Value;

        m_connected = true;
    } while (0);
    return m_connected;
}

int Aspt::ChangeNumber(quint8 NewDevNum)
{
    QVector<quint8> B;
    do {
        PortControl control(this);
        Q_UNUSED(control)
        if (m_err != ASPT_OK)
            break;
        if (ConvertSectionToParcel(bnCaptionTransmit, B, ChangeNetworkNumber, 0x0E700 + NewDevNum, 0, 0) != ASPT_OK)
            break;
        if (TransmitBlock(B) != ASPT_OK)
            break;
        if (ReceiveBlock(B, 9, 2000) != ASPT_OK)
            break;
        if (AnalysisCaptionReceive(B) != ASPT_OK)
            break;
        if (m_captionReceive.CommandCode != 5)
            return (m_err = ASPT_ERROR);
        if (ReceiveBlock(B, 9, 2000) != ASPT_OK)
            break;
        if (AnalysisCaptionReceive(B) != ASPT_OK)
            break;
        if (m_captionReceive.CommandCode != 9)
            return (m_err = ASPT_ERROR);
        if (ReceiveBlock(B, m_captionReceive.TransmitMasLength + 2, 2000) != ASPT_OK)
            break;
        if (CheckControlAmount(B) != ASPT_OK)
            break;
        if (B[0] != 0)
            return (m_err = ASPT_ERROR);
        m_captionTransmit.DeviceNumber = NewDevNum;
    } while (0);
    return m_err;
}

int Aspt::StepReadEEPROM(quint16 Address, QVector<quint8>& B, quint16& BS)
{
    QVector<quint8> _B;
    do {
        if (Address > 0x03E8 - BS)
            return (m_err = ASPT_ERROR);
        if (BS > MaxBuffSize)
            return (m_err = ASPT_ERROR);
        PortControl control(this);
        Q_UNUSED(control)
        if (m_err != ASPT_OK)
            break;
        if (ConvertSectionToParcel(bnCaptionTransmit, _B, ReadEEPROM_, Address, 0, BS) != ASPT_OK)
            break;
        if (TransmitBlock(_B) != ASPT_OK)
            break;
        if (ReceiveBlock(_B, 9, 2000) != ASPT_OK)
            break;
        if (AnalysisCaptionReceive(_B) != ASPT_OK)
            break;
        if (m_captionReceive.CommandCode != 4)
            return (m_err = ASPT_ERROR);
        if (ReceiveBlock(_B, m_captionReceive.TransmitMasLength + 2, 2000) != ASPT_OK)
            break;
        if (CheckControlAmount(_B) != ASPT_OK)
            break;
        B.clear();
        for (int i = 0, stop = BS - 2; i < stop; i++)
            B.append(_B[i]);
    } while (0);
    return m_err;
}

int Aspt::ReadEEPROM(quint16 Address, QVector<quint8>& B, quint16& BS)
{
    int result = 0;
    quint16 i = 0, j = 0;
    quint16 Steps = 0;
    quint16 LastStep = 0;
    QVector<quint8> _B;
    quint16 _BS = 0;

    if (Address > 0x03E8 - BS)
        return (m_err = ASPT_ERROR);
    _BS = 0;
    Steps = BS / MaxBuffSize;
    LastStep = BS - Steps * MaxBuffSize;
    BS = 0;
    if (Steps > 0)
        for (int stop = Steps, i = 0; i < stop; i++) {
            Address = i * MaxBuffSize;
            _BS = MaxBuffSize;
            if (StepReadEEPROM(Address, _B, _BS) != ASPT_OK)
                return m_err;
            for (int stop = _BS - 1, j = 0; j <= stop; j++) {
                B[i * MaxBuffSize + j] = _B[j];
            }
            BS = BS + _BS;
        }
    if (LastStep != 0) {
        Address = Steps * MaxBuffSize;
        _BS = LastStep;
        if (StepReadEEPROM(Address, _B, _BS) != ASPT_OK)
            return m_err;
        for (int stop = _BS, j = 0; j < stop; j++) {
            B[Steps * MaxBuffSize + j] = _B[j];
        }
        BS = BS + _BS;
    }
    return m_err;
}

int Aspt::StepWriteEEPROM(quint16 Address, QVector<quint8> B, quint16 BS)
{
    QVector<quint8> _B1;
    do {
        if (Address > 0x03E8 - BS)
            return (m_err = ASPT_ERROR);
        if (BS > MaxBuffSize)
            return (m_err = ASPT_ERROR);
        PortControl control(this);
        Q_UNUSED(control)
        if (m_err != ASPT_OK)
            break;
        if (ConvertSectionToParcel(bnCaptionTransmit, _B1, RecordEEPROM_, Address, BS, 0) != ASPT_OK)
            break;
        _B1.append(B);
        _B1.append({ 0, 0 });
        ControlAmount(_B1);
        if (TransmitBlock(_B1) != ASPT_OK)
            break;
        if (ReceiveBlock(_B1, 9, 2000) != ASPT_OK)
            break;
        if (AnalysisCaptionReceive(_B1) != ASPT_OK)
            break;
        if (m_captionReceive.CommandCode != 5)
            return (m_err = ASPT_ERROR);
        if (ReceiveBlock(_B1, 9, 2000) != ASPT_OK)
            break;
        if (AnalysisCaptionReceive(_B1) != ASPT_OK)
            break;
        if (m_captionReceive.CommandCode != 9)
            return (m_err = ASPT_ERROR);
        if (ReceiveBlock(_B1, m_captionReceive.TransmitMasLength + 2, 2000) != ASPT_OK)
            break;
        if (CheckControlAmount(_B1) != ASPT_OK)
            break;
        if (_B1[0] != 0)
            return (m_err = ASPT_ERROR);
    } while (0);
    return m_err;
}

int Aspt::WriteEEPROM(quint16 Address, QVector<quint8> B, quint16 BS)
{
    quint16 i = 0, j = 0;
    quint16 Steps = 0;
    quint16 LastStep = 0;
    QVector<quint8> _B;
    quint16 _BS = 0;
    if (Address > 0x03E8 - BS)
        return (m_err = ASPT_ERROR);
    for (int stop = MaxParcelLength, i = 0; i <= stop; i++)
        _B[i] = 0; //_BS:=0;
    Steps = BS / MaxBuffSize;
    LastStep = BS - Steps * MaxBuffSize;
    if (Steps > 0)
        for (int stop = Steps - 1, i = 0; i <= stop; i++) {
            for (int stop = MaxBuffSize - 1, j = 0; j <= stop; j++)
                _B[j] = B[i * MaxBuffSize + j];
            Address = i * MaxBuffSize;
            _BS = MaxBuffSize;
            if (StepWriteEEPROM(Address, _B, _BS) != ASPT_OK)
                return m_err;
        }
    if (LastStep != 0) {
        for (int stop = LastStep - 1, j = 0; j <= stop; j++)
            _B[j] = B[Steps * MaxBuffSize + j];
        Address = Steps * MaxBuffSize;
        _BS = LastStep;
        if (StepWriteEEPROM(Address, _B, _BS) != ASPT_OK)
            return m_err;
    }
    return m_err;
}

template <typename T>
int Aspt::ReadParameter(DeviceParameter_e DevParam, Parameter_t<T>& P)
{
    QVector<quint8> B1;
    const QSet<int> check({ dpROP1, dpROP2, dpROP3, dpROP4, dpKU0, dpKU1, dpRT0, dpKOEF1, dpKOEF2, dpALFAROP1, dpALFAROP2, dpALFAROP3, dpALFAROP4, dpALFAKU0, dpALFAKU1, dpRATIO, dpTIP, dpNumber });
    if (!check.contains(DevParam))
        return (m_err = ASPT_ERROR);

    P.Date = QDate();
    P.Value = 0;

    do {
        PortControl control(this);
        Q_UNUSED(control)
        if (m_err != ASPT_OK)
            break;
        if (ConvertSectionToParcel(bnCaptionTransmit, B1, ReadingFromEEPROM_, quint16(DevParam), 0, 0) != ASPT_OK)
            break;
        if (TransmitBlock(B1) != ASPT_OK)
            break;
        if (ReceiveBlock(B1, sizeof(Caption_t), 100) != ASPT_OK)
            break;
        if (AnalysisCaptionReceive(B1) != ASPT_OK)
            break;
        if (ReceiveBlock(B1, m_captionReceive.TransmitMasLength + 2, 2000) != ASPT_OK)
            break;
        if (CheckControlAmount(B1) != ASPT_OK)
            break;

        P.Date = QDate(1899, 12, 30).addDays(*reinterpret_cast<quint32*>(B1.data()));

        if ((typeid(T) == typeid(quint8)))
            P.Value = *reinterpret_cast<quint8*>(B1.data() + 4) & 0xFF;
        else if ((typeid(T) == typeid(quint32)))
            P.Value = *reinterpret_cast<quint32*>(B1.data() + 4) & 0xFFFFFF;
        else if ((typeid(T) == typeid(double)))
            P.Value = Byte5ToDouble(B1.data() + 4);

        const QStringList dpList({ "dpROP1",
            "dpROP2",
            "dpROP3",
            "dpROP4",
            "dpKU0",
            "dpKU1",
            "dpRT0",
            "dpKOEF1",
            "dpKOEF2",
            "dpALFAROP1",
            "dpALFAROP2",
            "dpALFAROP3",
            "dpALFAROP4",
            "dpALFAKU0",
            "dpALFAKU1",
            "dpRATIO",
            "dpTIP",
            "dpNumber" });
        qDebug() << dpList[DevParam] << "Date" << P.Date.toString("dd.MM.yyyy") << "Value" << P.Value; // QString().setNum(T(P.Value), 'f', 16);
        m_err = ASPT_OK;
    } while (0);
    return m_err;
}

int Aspt::Measure(AdcCfg& ADCCfg, double* M)
{
    quint16 k = 0;
    double V = 0.0;
    QVector<quint8> B1;
    QVector<quint8> B2;
    do {
        PortControl control(this);
        Q_UNUSED(control)
        if (m_err != ASPT_OK)
            break;
        if ((m_err = ADCCfg.ConvertSectionToParcel(B2)) != ASPT_OK)
            break;
        if (ConvertSectionToParcel(bnCaptionTransmit, B1, ProcessSteps, 0, B2.size() - 2, 0) != ASPT_OK)
            break;
        B1.append(B2);
        if (TransmitBlock(B1) != ASPT_OK)
            break;
        if (ReceiveBlock(B1, 9, 2000) != ASPT_OK)
            break;
        if (AnalysisCaptionReceive(B1) != ASPT_OK)
            break;
        if (m_captionReceive.CommandCode != 5) {
            // ReceiveBlock(B1, 9, 100);
            m_err = ASPT_ERROR;
            break;
        }
        if (ReceiveBlock(B1, 9, 60000) != ASPT_OK)
            break;
        if (AnalysisCaptionReceive(B1) != ASPT_OK)
            break;
        if (m_captionReceive.CommandCode != 4) {
            // ReceiveBlock(B1, 9, 100);
            m_err = ASPT_ERROR;
            break;
        }
        if (ReceiveBlock(B1, m_captionReceive.TransmitMasLength + 2, 2000) != ASPT_OK)
            break;
        if (B1[0] != ADCCfg.GetMeasureStepCount()) {
            m_err = ASPT_ERROR;
            break;
        }
        for (int stop = B1[0] - 1, i = 0; i <= stop; i++) {
            while (!(ADCCfg.UseMeasureStep[k]))
                ++k;
            //V = B1[1 + 0 + 4 * i] + B1[1 + 1 + 4 * i] * 0x100 + B1[1 + 2 + 4 * i] * 0x10000 + B1[1 + 3 + 4 * i] * 0x1000000;
            V = *reinterpret_cast<quint32*>(B1.data() + 1 + 4 * i);
            // if (V=8388607)or(V=-8388608) then begin for j:=0 to 5 do M[j]:=0; break; end;
            M[k] = V / ADCCfg.MeasureStep[k].AmountAverage;

            // хрень для отслеживания превышения макс. возможного, в этом случае в программе
            // должно выводиться соответствующее сообщение. было закомментарено до выяснения ряда вопросов...
            // if M[k] > $7FFFF0 then begin m_err := ASPT_ADC_OVERFLOW; break; end;

            ++k;
        }

        m_err = ASPT_OK;
    } while (0);
    return m_err;
}

int Aspt::GetMeasureValue(AdcCfg& ADCCfg, TSensorType SensorType, double Cable, double& Value)
{
    double M[6]{ 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    double Rop = 0.0;

    Value = ErrValue;

    const QSet<TSensorType> check({ vtR2W, vtR3W, vtR4W, vtCurr, vtVolt });
    if (!check.contains(SensorType))
        return ASPT_FUNCTION_INVALID_PARAMETER;

    if (Measure(ADCCfg, M) != ASPT_OK)
        return m_err;

    switch (ADCCfg.MeasureStep[0].SupportingResistorNumber) {
    case 0:
        Rop = m_eepromParameters.ROP1Correct;
        break;
    case 1:
        Rop = m_eepromParameters.ROP2Correct;
        break;
    case 2:
        Rop = m_eepromParameters.ROP3Correct;
        break;
    case 3:
        Rop = m_eepromParameters.ROP4Correct;
        break;
    }

    switch (SensorType) {
    // сопротивление 2xW (считается также как 4xW, минус сопротивление проводов)
    case vtR2W:
        if (!qFuzzyIsNull(M[1] - M[3]))
            Value = ((M[0] - M[2]) / (M[1] - M[3])) * Rop - Cable; // учитываем сопротивление кабеля!
        else
            m_err = ASPT_CALCULATION_ERROR;
        break;
    // сопротивление 3xW
    case vtR3W:
        if (!qFuzzyIsNull(M[2] - M[5]))
            Value = ((-2 * (M[1] - M[4]) + (M[0] - M[3])) / (M[2] - M[5])) * Rop;
        else
            m_err = ASPT_CALCULATION_ERROR;
        break;
    // сопротивление 4xW
    case vtR4W:
        if (!qFuzzyIsNull(M[1] - M[3]))
            Value = ((M[0] - M[2]) / (M[1] - M[3])) * Rop;
        else
            m_err = ASPT_CALCULATION_ERROR;
        break;
    // ток
    case vtCurr:
        if (!qFuzzyIsNull(M[1] - M[2]) && !qFuzzyIsNull(Cable))
            Value = (((M[0] - M[2]) / (M[1] - M[2])) * m_eepromParameters.KU1Correct) / Cable;
        else
            m_err = ASPT_CALCULATION_ERROR;
        break;
    // напряжение
    case vtVolt:
        if (!qFuzzyIsNull(M[1] - M[2]))
            Value = ((M[0] - M[2]) / (M[1] - M[2])) * m_eepromParameters.KU1Correct;
        else
            m_err = ASPT_CALCULATION_ERROR;
        break;
    default:
        break;
    }

    // отладка
    //    if (  != ASPT_OK)
    //        ShowMessage(/*IntToStr*/ QString().setNum(m_err) + ' : ' + FloatToStr(M[0]) + ';' + FloatToStr(M[1]) + ';' + FloatToStr(M[2]) + ';' + FloatToStr(M[3]) + ';' + FloatToStr(M[4]) + ';' + FloatToStr(M[5]));

    return m_err;
}

int Aspt::Initialize()
{
    do {
        if (ReadParameter<double>(dpROP1, m_eepromParameters.ROP1) != ASPT_OK)
            break;
        m_eepromParameters.ROP1Correct = m_eepromParameters.ROP1.Value;

        if (ReadParameter(dpROP2, m_eepromParameters.ROP2) != ASPT_OK)
            break;
        m_eepromParameters.ROP2Correct = m_eepromParameters.ROP2.Value;

        if (ReadParameter(dpROP3, m_eepromParameters.ROP3) != ASPT_OK)
            break;
        m_eepromParameters.ROP3Correct = m_eepromParameters.ROP3.Value;

        if (ReadParameter(dpROP4, m_eepromParameters.ROP4) != ASPT_OK)
            break;
        m_eepromParameters.ROP4Correct = m_eepromParameters.ROP4.Value;

        if (ReadParameter(dpKU0, m_eepromParameters.KU0) != ASPT_OK)
            break;
        m_eepromParameters.KU0Correct = m_eepromParameters.KU0.Value;

        if (ReadParameter(dpKU1, m_eepromParameters.KU1) != ASPT_OK)
            break;
        m_eepromParameters.KU1Correct = m_eepromParameters.KU1.Value;

        if (ReadParameter(dpRT0, m_eepromParameters.RT0) != ASPT_OK)
            break;

        if (ReadParameter(dpKOEF1, m_eepromParameters.KOEF1) != ASPT_OK)
            break;

        if (ReadParameter(dpKOEF2, m_eepromParameters.KOEF2) != ASPT_OK)
            break;

        if (ReadParameter(dpALFAROP1, m_eepromParameters.ALFAROP1) != ASPT_OK)
            break;

        if (ReadParameter(dpALFAROP2, m_eepromParameters.ALFAROP2) != ASPT_OK)
            break;

        if (ReadParameter(dpALFAROP3, m_eepromParameters.ALFAROP3) != ASPT_OK)
            break;

        if (ReadParameter(dpALFAROP4, m_eepromParameters.ALFAROP4) != ASPT_OK)
            break;

        if (ReadParameter(dpALFAKU0, m_eepromParameters.ALFAKU0) != ASPT_OK)
            break;

        if (ReadParameter(dpALFAKU1, m_eepromParameters.ALFAKU1) != ASPT_OK)
            break;

        if (ReadParameter(dpRATIO, m_eepromParameters.RATIO) != ASPT_OK)
            break;

        //        if (ReadParameter(dpTIP, m_eepromParameters.TIP) != ASPT_OK)
        //            break;

        if (ReadParameter(dpNumber, m_eepromParameters.Number) != ASPT_OK)
            break;
        m_err = ASPT_OK;
    } while (0);
    return m_err;
}

int Aspt::GetTemperature(double& Temperature)
{
    int k = 0;
    double M[6];
    double X = 0.0, Rt = 0.0, V = 0.0;
    double T[3];
    double TMiddle = 0.0; // среднее
    double TResult = 0.0; // результирующая температура устройства

    if (m_eepromParameters.RT0.Value == 0) {
        Temperature = 20;
        return ASPT_OK;
    }

    AdcCfg ADCCfg(AdcCfg::PackCorrection);
    if (ADCCfg.getErr() != ASPT_OK)
        return (m_err = ADCCfg.getErr());

    for (int i = 0; i < 3; i++) {
        if (Measure(ADCCfg, M) != ASPT_OK)
            return m_err;
        if (qFuzzyIsNull(M[1] - M[2])) {
            Temperature = 20;
            return m_err;
        }
        X = ((M[0] - M[2]) / (M[1] - M[2])) * m_eepromParameters.RATIO.Value;
        //X := ((M[1]-M[3])/(M[2]-M[3]))*m_eepromParameters.RATIO.Value;
        if (qFuzzyIsNull(m_eepromParameters.KOEF2.Value - X)) {
            Temperature = 20;
            return m_err;
        }
        Rt = ((X / (m_eepromParameters.KOEF2.Value - X)) * m_eepromParameters.KOEF1.Value) / m_eepromParameters.RT0.Value;
        if (qFuzzyIsNull(trunc(m_eepromParameters.TIP.Value)))
            T[i] = A6 * pow(Rt, 6) + A5 * pow(Rt, 5) + A4 * pow(Rt, 4) + A3 * pow(Rt, 3) + A2 * pow(Rt, 2) + A1 * Rt + A0;
        else
            T[i] = 20;
    }
    TMiddle = (T[0] + T[1] + T[2]) / 3;
    qDebug() << "TMiddle" << TMiddle;
    k = 0;
    V = 0;
    for (int i = 0; i < 3; i++) {
        if (abs(T[i] - TMiddle) > V) {
            V = abs(T[i] - TMiddle);
            k = i;
        }
    }
    TResult = 0;
    for (int i = 0; i < 3; i++) {
        if (i != k)
            TResult = TResult + T[i];
    }
    TResult = TResult / 2;
    Temperature = TResult;
    qDebug() << "Temperature" << Temperature;
    return m_err;
}

int Aspt::Correction()
{
    double T = 0.0;
    do {
        if (GetTemperature(T) != ASPT_OK)
            break;
        m_eepromParameters.ROP1Correct = m_eepromParameters.ROP1.Value + m_eepromParameters.ALFAROP1.Value * (T - 20);
        m_eepromParameters.ROP2Correct = m_eepromParameters.ROP2.Value + m_eepromParameters.ALFAROP2.Value * (T - 20);
        m_eepromParameters.ROP3Correct = m_eepromParameters.ROP3.Value + m_eepromParameters.ALFAROP3.Value * (T - 20);
        m_eepromParameters.ROP4Correct = m_eepromParameters.ROP4.Value + m_eepromParameters.ALFAROP4.Value * (T - 20);
        m_eepromParameters.KU0Correct = m_eepromParameters.KU0.Value + m_eepromParameters.ALFAKU0.Value * (T - 20);
        m_eepromParameters.KU1Correct = m_eepromParameters.KU1.Value + m_eepromParameters.ALFAKU1.Value * (T - 20);
    } while (0);
    return m_err;
}

uint Aspt::SerialNumber() const
{
    return m_serialNumber;
}

int Aspt::TransmitBlock(const QVector<quint8>& B)
{
    emit Write(QByteArray::fromRawData(reinterpret_cast<const char*>(B.data()), B.size()));
    m_err = ASPT_OK;
    return m_err;
}

int Aspt::ReceiveBlock(QVector<quint8>& B, quint16 BlockSize, quint16 Timeout)
{
    QElapsedTimer t;
    t.start();
    m_err = ASPT_ERROR;
    do {
        emit Read(BlockSize, Timeout);
        if (!m_semaphore.tryAcquire(1, Timeout))
            break;
        B = m_data;
        m_err = ASPT_OK;
    } while (0);
    if (dbg)
        qDebug() << "ReceiveBlock" << BlockSize << m_data.size() << Timeout << t.elapsed();
    return m_err;
}

void Aspt::RxData(const QByteArray& data)
{
    if (data.size()) {
        m_data.resize(data.size());
        memcpy(m_data.data(), data.data(), data.size());
        if (dbg)
            qDebug() << "RxData" << data.toHex().toUpper();
    }
    else
        m_data.clear();
    m_semaphore.release();
}

////////////////////////////////////////////
/// \brief SerialPort::SerialPort
/// \param upn
///
AsptPort::AsptPort(Aspt* aspt)
    : m_aspt(aspt)
{
    setBaudRate(QSerialPort::Baud9600);
    setParity(QSerialPort::NoParity);
    setFlowControl(QSerialPort::NoFlowControl);
    connect(this, &QSerialPort::readyRead, this, &AsptPort::ReadyRead);
}

void AsptPort::Close()
{
    close();
    m_data.clear();
    m_aspt->m_semaphore.release();
}

void AsptPort::Open(int mode)
{
    m_data.clear();
    if (open((QSerialPort::OpenMode)mode))
        m_aspt->m_semaphore.release();
}

void AsptPort::Write(const QByteArray& data)
{
    if (dbg)
        qDebug() << "TransmitBlock" << write(data) << data.toHex().toUpper();
    else
        write(data);
}

void AsptPort::Read(int size, int timeout)
{
    m_size = size;
    m_timeout = timeout;
    m_timer.start();
}

void AsptPort::ReadyRead()
{
    QMutexLocker Locker(&m_mutex);
    m_data.append(readAll());
    if (m_data.size() >= m_size) {
        QByteArray tmp(m_data.mid(0, m_size));
        m_data.remove(0, m_size);
        if (dbg)
            qDebug() << "ReadyRead" << m_timer.elapsed() << m_timeout;
        m_aspt->RxData(tmp);
    }
}
