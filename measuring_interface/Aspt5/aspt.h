
// описание функций работы с прибором АСПТ

/*$REALCOMPATIBILITY ON*/
#ifndef device_uH
#define device_uH

//#include "ex_com.h"
//#include "ExCOM_ASPT/com_u.h"
#include "adccfg.h"
#include "global.h"
//#include "nsh_u.h"

#include "../common_interfaces.h"
#include <QDate>
#include <QElapsedTimer>
#include <QMutex>
#include <QSemaphore>
#include <QSerialPort>
#include <QSet>
#include <QThread>

#pragma pack(push, 1)
// в блоке данных (чтение/запись параметров)
template <typename T>
struct Parameter_t {
    QDate Date = QDate(); // дата последнего изменения параметра
    T Value = 0; // значение параметра
};

// внутреннние параметры прибора
typedef struct EepromParameters_t {
    Parameter_t<double> ROP1; // значение опорного сопротивления 1
    double ROP1Correct = 0.0; // скорректированное значение сопротивления 1
    Parameter_t<double> ROP2; // значение опорного сопротивления 2
    double ROP2Correct = 0.0; // скорректированное значение сопротивления 2
    Parameter_t<double> ROP3; // значение опорного сопротивления 3
    double ROP3Correct = 0.0; // скорректированное значение сопротивления 3
    Parameter_t<double> ROP4; // значение опорного сопротивления 4
    double ROP4Correct = 0.0; // скорректированное значение сопротивления 4
    Parameter_t<double> KU0; // значение опорного напряжения 0
    double KU0Correct = 0.0; // скорректированное значение напряжения 0
    Parameter_t<double> KU1; // значение опорного напряжения 1
    double KU1Correct = 0.0; // скорректированное значение напряжения 1
    Parameter_t<double> RT0; //
    Parameter_t<double> KOEF1; //
    Parameter_t<double> KOEF2; //
    Parameter_t<double> ALFAROP1; //
    Parameter_t<double> ALFAROP2; //
    Parameter_t<double> ALFAROP3; //
    Parameter_t<double> ALFAROP4; //
    Parameter_t<double> ALFAKU0; //
    Parameter_t<double> ALFAKU1; //
    Parameter_t<double> RATIO; //
    Parameter_t<quint8> TIP; // тип внутреннего термометра прибора (0=Pt100)
    Parameter_t<quint32> Number; // заводской номер прибора
    quint16 ControlAmount = 0; // контрольная сумма блока
} EepromParameters_t;
#pragma pack(pop)

class AsptPort;
class Aspt : public QObject, public CommonInterfaces, protected AsptProtocol {
    Q_OBJECT
    friend class AsptPort;

public: // создание экземпляра объекта
    Aspt(QObject* parent = nullptr);
    // уничтожение экземпляра объекта
    ~Aspt();
    // CommonInterfaces interface
    bool Ping(const QString& portName) override;
    // получение подробной информации об ошибке
    //    QString ErrStr(int E) const
    //    {
    //        Q_UNUSED(E)
    //        QString ES;
    //        //    Err = GetExtendedErrorInfo(0, E, ES);
    //        //    if (Err != ASPT_OK) {
    //        //        EX_GetExtendedErrorInfo(0, Err, ES);
    //        //        return ES;
    //        //    }
    //        return ES;
    //    }
    // изменение сетевого адреса прибора
    int ChangeNumber(quint8 NewDevNum);
    // шаг чтения EEPROM
    int StepReadEEPROM(quint16 Address, QVector<quint8>& B, quint16& BS);
    // чтение EEPROM
    //    int ReadEEPROM(quint16 Address, QVector<quint8>& B, quint16& BS);
    // шаг записи EEPROM
    int StepWriteEEPROM(quint16 Address, QVector<quint8> B, quint16 BS);
    // запись в EEPROM
    //    int WriteEEPROM(quint16 Address, QVector<quint8> B, quint16 BS);
    // чтение параметра прибора
    template <typename T>
    int ReadParameter(DeviceParameter_e DevParam, Parameter_t<T>& P);
    // запись параметра прибора
    //    int WriteParameter(DeviceParameter_e DevParam, Parameter_t P)
    //    {
    //        int result = 0;
    //        //    quint16 i = 0;
    //        //    TParameter _P;
    //        //    TParcelMas _B1;
    //        //    quint16 _BS1 = 0;
    //        //    TParcelMas _B2;
    //        //    quint16 _BS2 = 0;
    //        //    /*#
    //        //  EX0:
    //        //  EX1:
    //        //  EX2:
    //        //  */
    //        //    if (!(device_u__0.contains(DevParam)))
    //        //        goto EX2;
    //        //    _P.Date = P.Date;
    //        //    _P.Value = P.Value;
    //        //    for (int stop = MaxParcelLength, i = 0; i <= stop; i++)
    //        //        _B1[i] = 0;
    //        //    _BS1 = 0;
    //        //    for (int stop = MaxParcelLength, i = 0; i <= stop; i++)
    //        //        _B2[i] = 0;
    //        //    _BS2 = 0;
    //        //    Err = OpenCOM();
    //        //    if (Err != ASPT_OK)
    //        //        goto EX1;
    //        //    bCaptionTransmit.CommandCode = 7;
    //        //    bCaptionTransmit.EEPROMAddress = 0x0E700 + int(DevParam);
    //        //    bCaptionTransmit.TransmitMasLength = 9;
    //        //    bCaptionTransmit.ResiveMasLength = 0;
    //        //    Err = ConvertSectionToParcel(bnCaptionTransmit, _B1, _BS1);
    //        //    if (Err != ASPT_OK)
    //        //        goto EX1;
    //        //    switch (int(DevParam)) {
    //        //    case /*# 0 .. 15 */ 0:
    //        //    case 1:
    //        //    case 2:
    //        //    case 3:
    //        //    case 4:
    //        //    case 5:
    //        //    case 6:
    //        //    case 7:
    //        //    case 8:
    //        //    case 9:
    //        //    case 10:
    //        //    case 11:
    //        //    case 12:
    //        //    case 13:
    //        //    case 14:
    //        //    case 15:
    //        //        RealToByte5(_P.Value, _B2);
    //        //        for (int stop = 0, i = 4; i >= stop; i--)
    //        //            _B2[i + 4] = _B2[i];
    //        //        _B2[0] = (((quint32)trunc(_P.Date)) & 0x000000FF);
    //        //        _B2[1] = (((quint32)trunc(_P.Date)) & 0x0000FF00) / 0x100;
    //        //        _B2[2] = (((quint32)trunc(_P.Date)) & 0x00FF0000) / 0x10000;
    //        //        _B2[3] = (((quint32)trunc(_P.Date)) & 0xFF000000) / 0x1000000;
    //        //        _BS2 = 11;
    //        //    break;
    //        //    case 16:
    //        //        _B2[0] = (((quint32)trunc(_P.Date)) & 0x000000FF);
    //        //        _B2[1] = (((quint32)trunc(_P.Date)) & 0x0000FF00) / 0x100;
    //        //        _B2[2] = (((quint32)trunc(_P.Date)) & 0x00FF0000) / 0x10000;
    //        //        _B2[3] = (((quint32)trunc(_P.Date)) & 0xFF000000) / 0x1000000;
    //        //        _B2[4] = ((quint8)Round(_P.Value));
    //        //        _BS2 = 7;
    //        //    break;
    //        //    case 17:
    //        //        _B2[0] = (((quint32)trunc(_P.Date)) & 0x000000FF);
    //        //        _B2[1] = (((quint32)trunc(_P.Date)) & 0x0000FF00) / 0x100;
    //        //        _B2[2] = (((quint32)trunc(_P.Date)) & 0x00FF0000) / 0x10000;
    //        //        _B2[3] = (((quint32)trunc(_P.Date)) & 0xFF000000) / 0x1000000;
    //        //        _B2[4] = (((quint32)trunc(_P.Value)) & 0x000000FF);
    //        //        _B2[5] = (((quint32)trunc(_P.Value)) & 0x0000FF00) / 0x100;
    //        //        _B2[6] = (((quint32)trunc(_P.Value)) & 0x00FF0000) / 0x10000;
    //        //        _BS2 = 9;
    //        //    break;
    //        //    }
    //        //    Err = ControlAmount(_B2, _BS2);
    //        //    if (Err != ASPT_OK)
    //        //        goto EX1;
    //        //    for (int stop = _BS2 - 1, i = 0; i <= stop; i++)
    //        //        _B1[9 + i] = _B2[i];
    //        //    _BS1 = _BS1 + _BS2;
    //        //    Err= TransmitBlock( _B1, _BS1);
    //        //    if (Err != ASPT_OK)
    //        //        goto EX1;
    //        //    _BS1 = 9;
    //        //    Err= SetTimeout( 2000);
    //        //    if (Err != ASPT_OK)
    //        //        goto EX1;
    //        //    Err= ReceiveBlock( _B1, _BS1);
    //        //    if (Err != ASPT_OK)
    //        //        goto EX1;
    //        //    Err = AnalysisCaptionReceive(_B1, _BS1);
    //        //    if (Err != ASPT_OK)
    //        //        goto EX1;
    //        //    if (bCaptionReceive.CommandCode != 5)
    //        //        goto EX2;
    //        //    _BS1 = 9;
    //        //    Err= SetTimeout( 2000);
    //        //    if (Err != ASPT_OK)
    //        //        goto EX1;
    //        //    Err= ReceiveBlock( _B1, _BS1);
    //        //    if (Err != ASPT_OK)
    //        //        goto EX1;
    //        //    Err = AnalysisCaptionReceive(_B1, _BS1);
    //        //    if (Err != ASPT_OK)
    //        //        goto EX1;
    //        //    if (bCaptionReceive.CommandCode != 9)
    //        //        goto EX2;
    //        //    _BS1 = bCaptionReceive.TransmitMasLength + 2;
    //        //    Err= SetTimeout( 2000);
    //        //    if (Err != ASPT_OK)
    //        //        goto EX1;
    //        //    Err= ReceiveBlock( _B1, _BS1);
    //        //    if (Err != ASPT_OK)
    //        //        goto EX1;
    //        //    Err = CheckControlAmount(_B1, _BS1);
    //        //    if (Err != ASPT_OK)
    //        //        goto EX1;
    //        //    if (_B1[0] != 0)
    //        //        goto EX2;
    //        //    result = ASPT_OK;
    //        //    goto EX0;
    //        //EX1:
    //        //    result = Err;
    //        //    goto EX0;
    //        //EX2:
    //        //    result = ASPT_ERROR;
    //        //    goto EX0;
    //        //EX0:
    //        //    CloseCOM();
    //        return result;
    //    }
    // измерение
    int Measure(AdcCfg& ADCCfg, double* M);
    // чтение из прибора измеренного значения
    //    int GetMeasureValue(AdcCfg& ADCCfg, TSensorType SensorType, TNSH NSH, double Tmin, double Tmax, double Cable, const long double* Coefs, int Coefs_maxidx, double& Value, double& Temperature)
    //    {
    //        int result = 0;
    //        //    double M[6]{ 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }; /*# range 0..5*/
    //        //    int i = 0;
    //        //    double P = 0.0;
    //        //    long double V = ErrValue;
    //        //    long double T = ErrValue;
    //        //    TNSH tcNSH;
    //        //    /*#
    //        //      EX0:
    //        //      EX1:
    //        //      EX2:
    //        //      */
    //        //    Err = Measure(ADCCfg, M);
    //        //    if (Err != ASPT_OK)
    //        //        goto EX2;
    //        //    /*# with bEEPROMParameters do */
    //        //    switch (ADCCfg.MeasureStep[0].Supm_portingResistorNumber) {
    //        //    case 0:
    //        //        P = bEEPROMParameters.ROP1Correct;
    //        //        break;
    //        //    case 1:
    //        //        P = bEEPROMParameters.ROP2Correct;
    //        //        break;
    //        //    case 2:
    //        //        P = bEEPROMParameters.ROP3Correct;
    //        //        break;
    //        //    case 3:
    //        //        P = bEEPROMParameters.ROP4Correct;
    //        //        break;
    //        //    }
    //        //    //    try {
    //        //    switch (SensorType) {
    //        //    //        case
    //        //    //            // нихрена тут нет
    //        //    //            vtNil:
    //        //    //        break;
    //        //    //        // сопротивление 2xW (считается также как 4xW, минус сопротивление проводов)

    //        //    //        case vtR2W:
    //        //    //            if ((M[1] - M[3]) != 0) {
    //        //    //                V = ((M[0] - M[2]) / (M[1] - M[3])) * P;
    //        //    //                V = V - Cable; // учитываем сопротивление кабеля!
    //        //    //                Err = DirectCalculation(NSH, V, 0, 1, ARRAYOFCONST(()), T);
    //        //    //                if (Err != ASPT_OK)
    //        //    //                    T = ErrValue;
    //        //    //            }
    //        //    //            else
    //        //    //                Err = ASPT_CALCULATION_ERROR;
    //        //    //        break;
    //        //    //        // сопротивление 3xW

    //        //    //        case vtR3W:
    //        //    //            if ((M[2] - M[5]) != 0) {
    //        //    //                V = (double((-2 * (M[1] - M[4]) + (M[0] - M[3]))) / (M[2] - M[5])) * P;
    //        //    //                Err = DirectCalculation(NSH, V, 0, 1, ARRAYOFCONST(()), T);
    //        //    //                if (Err != ASPT_OK)
    //        //    //                    T = ErrValue;
    //        //    //            }
    //        //    //            else
    //        //    //                Err = ASPT_CALCULATION_ERROR;
    //        //    //        break;

    //        //    case vtR4W: // сопротивление 4xW
    //        //        if ((M[1] - M[3]) != 0) {
    //        //            V = ((M[0] - M[2]) / (M[1] - M[3])) * P;
    //        //            //                Err = DirectCalculation(NSH, V, 0, 1, ARRAYOFCONST(()), T);
    //        //            //                if (Err != ASPT_OK)
    //        //            //                    T = ErrValue;
    //        //        }
    //        //        else
    //        //            Err = ASPT_CALCULATION_ERROR;
    //        //        break;
    //        //        //        // термопара
    //        //        //        case vtTC:
    //        //        //            if (((M[1] - M[2]) != 0) && ((M[4] - M[5]) != 0)) {
    //        //        //                V = ((M[0] - M[2]) / (M[1] - M[2])) * P;
    //        //        //                Err = DirectCalculation(rPt100, V, 0, 1, ARRAYOFCONST(()), T);
    //        //        //                if (Err != ASPT_OK)
    //        //        //                    goto EX1;
    //        //        //                Err = ReverseCalculation(NSH, T, 0, 1, ARRAYOFCONST(()), V);
    //        //        //                if (Err != ASPT_OK)
    //        //        //                    goto EX1;
    //        //        //                V = ((M[3] - M[5]) / (M[4] - M[5])) * bEEPROMParameters.KU1Correct + V;
    //        //        //                Err = DirectCalculation(NSH, V, 0, 1, ARRAYOFCONST((0)), T);
    //        //        //                if (Err != ASPT_OK)
    //        //        //                    T = ErrValue;
    //        //        //            }
    //        //        //            else
    //        //        //                Err = ASPT_CALCULATION_ERROR;
    //        //        //        break;
    //        //        //        // термопара

    //        //        //        case vtTC0:
    //        //        //            if ((M[4] - M[5]) != 0) {
    //        //        //                V = ((M[3] - M[5]) / (M[4] - M[5])) * bEEPROMParameters.KU1Correct;
    //        //        //                Err = DirectCalculation(NSH, V, 0, 1, ARRAYOFCONST((0)), T);
    //        //        //                if (Err != ASPT_OK)
    //        //        //                    T = ErrValue;
    //        //        //            }
    //        //        //            else
    //        //        //                Err = ASPT_CALCULATION_ERROR;
    //        //        //        break;
    //        //        //        // ток

    //        //        //        case vtCurr:
    //        //        //            if ((M[1] - M[2] != 0) && (Cable != 0)) {
    //        //        //                V = (((M[0] - M[2]) / (M[1] - M[2])) * bEEPROMParameters.KU1Correct) / Cable;
    //        //        //                Err = DirectCalculation(NSH, V, Tmin, Tmax, ARRAYOFCONST(()), T);
    //        //        //                if (Err != ASPT_OK)
    //        //        //                    T = ErrValue;
    //        //        //            }
    //        //        //            else
    //        //        //                Err = ASPT_CALCULATION_ERROR;
    //        //        //        break;
    //        //        //        // напряжение

    //        //        //        case vtVolt:
    //        //        //            if ((M[1] - M[2]) != 0) {
    //        //        //                V = ((M[0] - M[2]) / (M[1] - M[2])) * bEEPROMParameters.KU1Correct;
    //        //        //                T = ErrValue;
    //        //        //            }
    //        //        //            else
    //        //        //                Err = ASPT_CALCULATION_ERROR;
    //        //        //        break;
    //        //        //        // эталонный термометр - сопротивление ТС (1)

    //        //        //        case etR4W:
    //        //        //            if ((M[1] - M[3]) != 0) {
    //        //        //                V = ((M[0] - M[2]) / (M[1] - M[3])) * P;
    //        //        //                if (device_u__0.Contains(NSH)) {
    //        //        //                    Err = DirectCalculation(NSH, V, 0, 1, Coefs, T);
    //        //        //                    if (Err != ASPT_OK)
    //        //        //                        T = ErrValue;
    //        //        //                }
    //        //        //            }
    //        //        //            else
    //        //        //                Err = ASPT_CALCULATION_ERROR;
    //        //        //        break;
    //        //        //        // эталонный термометр - термопара ТП (2)

    //        //        //        case etTC:
    //        //        //            switch (NSH) {
    //        //        //            case mVolt10:
    //        //        //            case mVolt20:
    //        //        //            case mVolt30: // без комп. ХС, тип задания: 1,2,3
    //        //        //                if ((M[4] - M[5]) != 0) {
    //        //        //                    V = ((M[3] - M[5]) / (M[4] - M[5])) * bEEPROMParameters.KU1Correct;
    //        //        //                    Err = DirectCalculation(NSH, V, 0, 1, Coefs, T);
    //        //        //                    if (Err != ASPT_OK)
    //        //        //                        T = ErrValue;
    //        //        //                }
    //        //        //                else
    //        //        //                    Err = ASPT_CALCULATION_ERROR;
    //        //        //                break;
    //        //        //            case mVolt1R:
    //        //        //            case mVolt1S:
    //        //        //            case mVolt1B:
    //        //        //            case mVolt2S:
    //        //        //            case mVolt3R:
    //        //        //            case mVolt3S: // c комп. ХС, тип задания: 1,2,3
    //        //        //            {
    //        //        //                switch (NSH) {
    //        //        //                case mVolt1R:
    //        //        //                case mVolt3R:
    //        //        //                    tcNSH = uR;
    //        //        //                    break;
    //        //        //                case mVolt1S:
    //        //        //                case mVolt2S:
    //        //        //                case mVolt3S:
    //        //        //                    tcNSH = uS;
    //        //        //                    break;
    //        //        //                case mVolt1B:
    //        //        //                    tcNSH = uB;
    //        //        //                    break;
    //        //        //                }
    //        //        //                if (((M[1] - M[2]) != 0) && ((M[4] - M[5]) != 0)) {
    //        //        //                    V = ((M[0] - M[2]) / (M[1] - M[2])) * P;
    //        //        //                    Err = DirectCalculation(rPt100, V, 0, 1, ARRAYOFCONST(()), T);
    //        //        //                    if (Err != ASPT_OK)
    //        //        //                        goto EX1;
    //        //        //                    Err = ReverseCalculation(tcNSH, T, 0, 1, ARRAYOFCONST(()), V);
    //        //        //                    if (Err != ASPT_OK)
    //        //        //                        goto EX1;
    //        //        //                    V = ((M[3] - M[5]) / (M[4] - M[5])) * bEEPROMParameters.KU1Correct + V;
    //        //        //                    Err = DirectCalculation(NSH, V, 0, 1, Coefs, T);
    //        //        //                    if (Err != ASPT_OK)
    //        //        //                        T = ErrValue;
    //        //        //                }
    //        //        //                else
    //        //        //                    Err = ASPT_CALCULATION_ERROR;
    //        //        //            break;
    //        //        //            }
    //        //        //            break;
    //        //        //            // образц. термометр - токовый выход ИП (3)

    //        //        //            //        case etCurr:
    //        //        //            //            if (((M[1] - M[2]) != 0) && (Cable != 0)) {
    //        //        //            //                V = (((M[0] - M[2]) / (M[1] - M[2])) * bEEPROMParameters.KU1Correct) / Cable;
    //        //        //            //                Err = DirectCalculation(mCurr, V, Tmin, Tmax, Coefs, T);
    //        //        //            //                if (Err != ASPT_OK)
    //        //        //            //                    T = ErrValue;
    //        //        //            //            }
    //        //        //            //            else
    //        //        //            Err = ASPT_CALCULATION_ERROR;
    //        //        //        //        break;
    //        //        //        // внутренний термометр АСПТ (не используется)

    //        //        //        case dvTrm:
    //        //        //        break;
    //        //    }
    //        //    goto EX0;
    //        //EX1:
    //        //    V = ErrValue;
    //        //    T = ErrValue;
    //        //EX0:
    //        ////    }
    //        ////    catch (...) {
    //        ////    }
    //        //EX2:
    //        //    ///*
    //        //    //  // отладка
    //        //    //  if Err <> ASPT_OK then
    //        //    //  begin
    //        //    //    ShowMessage(/*IntToStr*/QString().setNum(Err)+' : '+FloatToStr(M[0])+';'+FloatToStr(M[1])+';'+FloatToStr(M[2])+';'+FloatToStr(M[3])+';'+FloatToStr(M[4])+';'+FloatToStr(M[5]));
    //        //    ////end;
    //        //    //*/
    //        //    Value = V;
    //        //    Temperature = T;
    //        //    result = Err;
    //        return result;
    //    }
    int GetMeasureValue(AdcCfg& ADCCfg, TSensorType SensorType, double Cable, double& Value);
    // инициализация прибора
    int Initialize();
    // получение температуры прибора
    int GetTemperature(double& Temperature);
    // температурная коррекция прибора
    int Correction();

    uint SerialNumber() const;

signals:
    void Close();
    void Open(int mode);
    void Write(const QByteArray& data);
    void Read(int size, int timeout);

private:
    QSemaphore m_semaphore;
    QMutex m_mutex;
    AsptPort* m_port;
    QThread m_portThread;
    EepromParameters_t m_eepromParameters;
    //    int Err = 0; // код ошибки
    //--- коэффициенты термометра Pt100, используемого для измерения внутренней температуры прибора
    const double A0 = -2.4205036234E+02;
    const double A1 = +2.2201129783E+02;
    const double A2 = +2.9636114652E+01;
    const double A3 = -1.3926676547E+01;
    const double A4 = +5.1365329528E+00;
    const double A5 = -9.0943345215E-01;
    const double A6 = +6.6194888606E-02;
    uint m_serialNumber = 0;

    int TransmitBlock(const QVector<quint8>& B /*, quint16 BS*/);
    int ReceiveBlock(QVector<quint8>& B, quint16 BlockSize, quint16 Timeout);

    class PortControl {
    public:
        PortControl(Aspt* aspt)
            : m_aspt(aspt)
            , m_semaphore(&aspt->m_semaphore)
        {
            emit m_aspt->Open(QSerialPort::ReadWrite);
            if (m_semaphore->tryAcquire(1, 1000))
                m_aspt->m_err = ASPT_OK;
            else
                m_aspt->m_err = ASPT_ERROR; //m_aspt->m_port->error();
        }
        ~PortControl()
        {
            emit m_aspt->Close();
            m_semaphore->tryAcquire(1, 1000);
        }

    private:
        Aspt* m_aspt;
        QSemaphore* m_semaphore;
    };
    QVector<quint8> m_data;
    void RxData(const QByteArray& data);
};

class AsptPort : public QSerialPort, protected AsptProtocol {
    Q_OBJECT

public:
    AsptPort(Aspt* aspt);
    void Close();
    void Open(int mode);
    void Write(const QByteArray& data);
    void Read(int size, int timeout);
    QByteArray m_data;

private:
    QElapsedTimer m_timer;
    int m_size = 0;
    int m_timeout = 0;
    QMutex m_mutex;
    Aspt* m_aspt;
    void ReadyRead();
};

#endif //  device_uH
