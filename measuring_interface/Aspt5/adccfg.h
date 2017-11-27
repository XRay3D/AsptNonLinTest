
/*$REALCOMPATIBILITY ON*/
#ifndef adccfg_uH
#define adccfg_uH

//#include "ex_com.h"
//#include "ExCOM_ASPT/com_u.h"
#include "global.h"

#include <QObject>

enum {
    MaxStepCount = 6, // максимальное число шагов измерения одного канала
    IgnoreValue = 255 // если встречается это значение - параметр не меняется
};
class AdcCfg : private AsptProtocol {

private:
    int m_err;
    quint8 _MeasureStepCount;

public:
    static const QString PackClearADCCfgFull;
    static const QString PackClearADCCfgLow;
    static const QString PackClearADCCfgMedium;
    static const QString PackClearADCCfgSpecial1;
    static const QString PackCorrection;
    static const QString PackCurrent;
    static const QString PackResistor;
    static const QString PackVoltage;

#pragma pack(push, 1)
    typedef struct TbData {
        quint8 UnsetADC; /*бит*/ // 1 - требуется сброс АЦП
        quint8 AutoCalibrationADC; /*бит*/ // 1 - требуется самокалибровка АЦП
        quint8 UnsetFilter; /*бит*/ // 1 - нужен сброс фильтра (SYNC)
        quint8 InputBuffer; /*бит*/ // входной буфер 0-выкл/1-вкл (BUFER)
        quint8 StateCurrent; /*бит*/ // 0-выключен ток / 1-включен ток
        quint8 GainFactor; /*3 бита*/ // коэф. усиления
        quint8 FilterFrequency; /*3 бита*/ // код(частота) фильтра
        quint8 MeasureChannel; // номер канала подключаемого тока - просто номер канала [0..15]
        quint8 SupportingResistorNumber; // номер опорного резистора
        quint8 ValuePolarityCurrent; // значение и полярность тока
        quint8 VoltageChannel; // номер канала измеряемого напряжения
        quint8 PauseBeforeMeasure; // пауза после переключения канала (5 мсек интервалы)
        quint8 AmountMeasurements; // количество пропущенных измерений перед первым
        quint8 AmountAverage; // количество усреднений
        //quint8 F;
    } TbData;

    // настройки АЦП прибора для измерения напряжений канала по шагам
    TbData MeasureStep[6 /*# range 0..MaxStepCount-1*/];
#pragma pack(pop)
    // используемые для измерения шаги
    bool UseMeasureStep[6 /*# range 0..MaxStepCount-1*/];
    // размер передаваемых данных посылки
    quint16 bDataSize;
    // __property unsignedchar MeasureStepCount = { read = GetMeasureStepCount, write = SetMeasureStepCount };
    //quint8 MeasureStepCount;

    AdcCfg(const QString& Pack = QString());
    ~AdcCfg();

    quint8 GetMeasureStepCount();
    void SetMeasureStepCount(quint8 SC);

    int ClearSection();
    int InitSection();
    int ConvertSectionToParcel(QVector<quint8>& B);
    int ConvertSectionFromParcel(QVector<quint8>& B);
    int SetUseMeasureStep(const QVector<quint8>& P /*, int P_maxidx*/);
    int GetUseMeasureStep(QVector<quint8>& P /*, int P_maxidx*/);
    int SetUnsetADC(const QVector<quint8>& P /*, int P_maxidx*/);
    int GetUnsetADC(QVector<quint8>& P /*, int P_maxidx*/);
    int SetAutoCalibrationADC(const QVector<quint8>& P /*, int P_maxidx*/);
    int GetAutoCalibrationADC(QVector<quint8>& P /*, int P_maxidx*/);
    int SetUnsetFilter(const QVector<quint8>& P /*, int P_maxidx*/);
    int GetUnsetFilter(QVector<quint8>& P /*, int P_maxidx*/);
    int SetInputBuffer(const QVector<quint8>& P /*, int P_maxidx*/);
    int GetInputBuffer(QVector<quint8>& P /*, int P_maxidx*/);
    int SetStateCurrent(const QVector<quint8>& P /*, int P_maxidx*/);
    int GetStateCurrent(QVector<quint8>& P /*, int P_maxidx*/);
    int SetGainFactor(const QVector<quint8>& P /*, int P_maxidx*/);
    int GetGainFactor(QVector<quint8>& P /*, int P_maxidx*/);
    int SetFilterFrequency(const QVector<quint8>& P /*, int P_maxidx*/);
    int GetFilterFrequency(QVector<quint8>& P /*, int P_maxidx*/);
    int SetMeasureChannel(const QVector<quint8>& P /*, int P_maxidx*/);
    int GetMeasureChannel(QVector<quint8>& P /*, int P_maxidx*/);
    int SetSupportingResistor(const QVector<quint8>& P /*, int P_maxidx*/);
    int GetSupportingResistor(QVector<quint8>& P /*, int P_maxidx*/);
    int SetValuePolarityCurrent(const QVector<quint8>& P /*, int P_maxidx*/);
    int GetValuePolarityCurrent(QVector<quint8>& P /*, int P_maxidx*/);
    int SetVoltageChannel(const QVector<quint8>& P /*, int P_maxidx*/);
    int GetVoltageChannel(QVector<quint8>& P /*, int P_maxidx*/);
    int SetPauseBeforeMeasure(const QVector<quint8>& P /*, int P_maxidx*/);
    int GetPauseBeforeMeasure(QVector<quint8>& P /*, int P_maxidx*/);
    int SetAmountMeasurements(const QVector<quint8>& P /*, int P_maxidx*/);
    int GetAmountMeasurements(QVector<quint8>& P /*, int P_maxidx*/);
    int SetAmountAverage(const QVector<quint8>& P /*, int P_maxidx*/);
    int GetAmountAverage(QVector<quint8>& P /*, int P_maxidx*/);
    int SetPack(const QString& P);
    QString GetPack();
    int getErr() const;
};

#endif //  adccfg_uH
