
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
    typedef struct StepData {
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
    } StepData;

    // настройки АЦП прибора для измерения напряжений канала по шагам
    StepData MeasureStep[6];
#pragma pack(pop)
    // используемые для измерения шаги
    bool UseMeasureStep[6];
    // размер передаваемых данных посылки
    // __property unsignedchar MeasureStepCount = { read = GetMeasureStepCount, write = SetMeasureStepCount };

    AdcCfg(const QString& Pack = QString());
    ~AdcCfg();

    quint8 measureStepCount();
    void setMeasureStepCount(quint8 SC);

    int clear();
    int init();

    int convertSectionToParcel(QVector<quint8>& B);
    int convertSectionFromParcel(QVector<quint8>& B);

    int setUseMeasureStep(const QVector<quint8>& P);
    int useMeasureStep(QVector<quint8>& P) const;
    QVector<quint8> useMeasureStep() const;

    int setUnsetADC(const QVector<quint8>& P);
    int unsetADC(QVector<quint8>& P) const;
    QVector<quint8> unsetADC() const;

    int setAutoCalibrationADC(const QVector<quint8>& P);
    int autoCalibrationADC(QVector<quint8>& P) const;
    QVector<quint8> autoCalibrationADC() const;

    int setUnsetFilter(const QVector<quint8>& P);
    int unsetFilter(QVector<quint8>& P) const;
    QVector<quint8> unsetFilter() const;

    int setInputBuffer(const QVector<quint8>& P);
    int inputBuffer(QVector<quint8>& P) const;
    QVector<quint8> inputBuffer() const;

    int setStateCurrent(const QVector<quint8>& P);
    int stateCurrent(QVector<quint8>& P);
    QVector<quint8> stateCurrent() const;

    int setGainFactor(const QVector<quint8>& P);
    int gainFactor(QVector<quint8>& P) const;
    QVector<quint8> gainFactor() const;

    int setFilterFrequency(const QVector<quint8>& P);
    int filterFrequency(QVector<quint8>& P) const;
    QVector<quint8> filterFrequency() const;

    int setMeasureChannel(const QVector<quint8>& P);
    int measureChannel(QVector<quint8>& P) const;
    QVector<quint8> measureChannel() const;

    int setSupportingResistor(const QVector<quint8>& P);
    int supportingResistor(QVector<quint8>& P);
    QVector<quint8> supportingResistor() const;

    int setValuePolarityCurrent(const QVector<quint8>& P);
    int valuePolarityCurrent(QVector<quint8>& P) const;
    QVector<quint8> valuePolarityCurrent() const;

    int setVoltageChannel(const QVector<quint8>& P);
    int voltageChannel(QVector<quint8>& P) const;
    QVector<quint8> voltageChannel() const;

    int setPauseBeforeMeasure(const QVector<quint8>& P);
    int pauseBeforeMeasure(QVector<quint8>& P) const;
    QVector<quint8> pauseBeforeMeasure() const;

    int setAmountMeasurements(const QVector<quint8>& P);
    int amountMeasurements(QVector<quint8>& P) const;
    QVector<quint8> amountMeasurements() const;

    int setAmountAverage(const QVector<quint8>& P);
    int amountAverage(QVector<quint8>& P) const;
    QVector<quint8> amountAverage() const;

    int setPack(const QString& P);
    QString pack();

    int getErr() const;
};

#endif //  adccfg_uH
