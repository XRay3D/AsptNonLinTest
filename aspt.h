#ifndef ASPTCLASS_H
#define ASPTCLASS_H
#include <QLibrary>
#include <QObject>

enum { DBG = 0 };

enum { MaxStepCount = 6 }; //           максимальное число шагов
enum { MaxParcelLength = 0xFFFF }; //   длина посылки для массива

enum ErrRtVal {
    ErrValue = 9999, // значение, в случае невозможности подсчитать результат
    ErrValueIMKT = 11111 //значение, в случае невозможности подсчитать результат (для данных получаемых из ИМКТ)
};

enum AsptRetVal {
    ASPT_OK = 0, //Ok (все в порядке, функция завершила свою работу)
    ASPT_ERROR = -101, //Error (не описанная или неизвестная)
    ASPT_FUNCTION_INVALID_PARAMETER = -102, //Ошибка в параметрах вызываемой функции
    ASPT_CONTROL_AMOUNT_ERROR = -103, //Ошибка подсчета контрольной суммы
    ASPT_INVALID_DEVICE_NUMBER = -104, //Неверный адрес прибора
    ASPT_DIRECT_CALCULATION_ERROR = -196, //Ошибка при расчете температуры в функции DirectCalculation
    ASPT_REVERSE_CALCULATION_ERROR = -197, //Ошибка при расчете изм. значения в функции ReverseCalculation
    ASPT_CALCULATION_ERROR = -198, //Ошибка в вычислениях (например, деление на 0)
    ASPT_DLL_ERROR_HANDLE_FREE = -199, //Ошибка DLL - Handle неопределен
    ASPT_ADC_OVERFLOW = -200 //Переполнение АЦП (изм = 7FFFFh) "поданный сигнал превышает допустимый"
};

//тип доступа к настройкам АЦП
enum TAccessType {
    atFull,
    atMedium,
    atLow,
    atSpecial1,
    atNone
};

enum TSupportingResistor {
    sr15,
    sr150,
    sr1000
};

enum TValuePolarityCurrent {
    c01,
    c02,
    c05,
    c10,
    c15,
    c20,
    c30,
    c40
};

//посылка с данными
typedef wchar_t Char;
typedef Char* TParcelMas;
typedef double TDateTime;
typedef void* HANDLE;
typedef HANDLE THandle;

#pragma pack(push, 1)
typedef struct {
    TDateTime Date; //дата последнего изменения параметра
    double Value; //значение параметра
} TParameter;
#pragma pack(pop)

//тип калибратора
enum TCalibrType {
    ctNone,
    ctKT110,
    ctKT150K,
    ctKT200K,
    ctKT500,
    ctKT500K,
    ctKT650,
    ctKT650K,
    ctKT1100,
    ctURNT01,
    ctT150,
    ctT220,
    ctTTB,
    ctGa,
    ctNull,
    ctUser
};
//TCalibrType = (ctNone, ctKT110, ctKT500, ctKT650, ctKT1100, ctURNT01, ctT150, ctT220, ctTTB, ctGa, ctNull, ctUser);

//тип датчика
enum TSensorType {
    vtNil, //vt - Verified Thermoelement;
    vtR2W,
    vtR3W,
    vtR4W,
    vtTC,
    vtTC0,
    vtCurr,
    vtVolt,
    etR4W, //et - Exemplary Thermometer;
    etTC,
    etCurr,
    etUser,
    dvTrm, //dv - Device Buil-in Thermometer;
    inCurr, //in - Invert Verified Thermoelement.
    inVolt
};

//параметры прибора
enum TDeviceParameter {
    dpROP1,
    dpROP2,
    dpROP3,
    dpROP4,
    dpKU0,
    dpKU1,
    dpRT0,
    dpKOEF1,
    dpKOEF2,
    dpALFAROP1,
    dpALFAROP2,
    dpALFAROP3,
    dpALFAROP4,
    dpALFAKU0,
    dpALFAKU1,
    dpRATIO,
    dpTIP,
    dpNumber
};

enum TNSH {
    nNil,
    r10M1,
    r50M1,
    r53M1,
    r100M1,
    r10M2,
    r50M2,
    r53M2,
    r100M2,
    r10P,
    r46P,
    r50P,
    r100P,
    r500P,
    rPt10,
    rPt50,
    rPt100,
    rPt500,
    rPt1000,
    r100H,
    uK,
    uL,
    uJ,
    uT,
    uN,
    uR,
    uS,
    uA1,
    uA2,
    uA3,
    uB,
    uE,
    i05,
    i420,
    i020,
    e0300,
    mRes1,
    mRes2,
    mVolt10,
    mVolt1R,
    mVolt1S,
    mVolt1B,
    mVolt20,
    mVolt2S,
    mVolt30,
    mVolt3R,
    mVolt3S,
    mCurr,
    mUser,
    e0100,
    e0010
};

class AsptClass {

public:
    AsptClass();
    ~AsptClass();

    int DevSetCOMN(const QString& CPS);
    int DevGetCOMN(QString& CPS);
    int DevSetBaudRate(uint16_t BaudRate);
    int DevGetBaudRate(uint16_t& BaudRate);
    int DevSetNumber(uint8_t DeviceNumber);
    int DevGetNumber(uint8_t& DeviceNumber);
    int DevChangeNumber(uint8_t NewDevNum);
    int DevReadEEPROM(uint16_t Address, TParcelMas& B, uint16_t& BS);
    int DevWriteEEPROM(uint16_t Address, TParcelMas B, uint16_t BS);
    int DevReadParameter(TDeviceParameter DevParam, TParameter& P);
    int DevWriteParameter(TDeviceParameter DevParam, TParameter P);
    int DevMeasure(double* M, int M_maxidx);
    int DevGetMeasureValue(TSensorType SensorType, TNSH NSH, double VTmin, double VTmax, double VCable, const long double* Coefs, int Coefs_maxidx, double& Value, double& Temperature);
    int DevInitialize();
    int DevGetTemperature(double& Temperature);
    int DevCorrection();
    int DevSetTieParameters(const QString& CPS, uint16_t BaudRate, uint8_t DeviceNumber);
    int DevGetCorrectParamValue(TDeviceParameter DevParam, double& CorrectValue);
    int DevGetParamValue(TDeviceParameter DevParam, double& ParamValue);

    int AdcSetUseMeasureStep(const uint8_t* P, int P_maxidx = 5);
    int AdcGetUseMeasureStep(uint8_t* P, int P_maxidx = 5);
    int AdcSetUnsetADC(const uint8_t* P, int P_maxidx = 5);
    int AdcGetUnsetADC(uint8_t* P, int P_maxidx = 5);
    int AdcSetAutoCalibrationADC(const uint8_t* P, int P_maxidx = 5);
    int AdcGetAutoCalibrationADC(uint8_t* P, int P_maxidx = 5);
    int AdcSetUnsetFilter(const uint8_t* P, int P_maxidx = 5);
    int AdcGetUnsetFilter(uint8_t* P, int P_maxidx = 5);
    int AdcSetInputBuffer(const uint8_t* P, int P_maxidx = 5);
    int AdcGetInputBuffer(uint8_t* P, int P_maxidx = 5);
    int AdcSetStateCurrent(const uint8_t* P, int P_maxidx = 5);
    int AdcGetStateCurrent(uint8_t* P, int P_maxidx = 5);
    int AdcSetGainFactor(const uint8_t* P, int P_maxidx = 5);
    int AdcGetGainFactor(uint8_t* P, int P_maxidx = 5);
    int AdcSetFilterFrequency(const uint8_t* P, int P_maxidx = 5);
    int AdcGetFilterFrequency(uint8_t* P, int P_maxidx = 5);
    int AdcSetMeasureChannel(const uint8_t* P, int P_maxidx);
    int AdcGetMeasureChannel(uint8_t* P, int P_maxidx = 5);
    int AdcSetSupportingResistor(const uint8_t* P, int P_maxidx);
    int AdcGetSupportingResistor(uint8_t* P, int P_maxidx = 5);
    int AdcSetValuePolarityCurrent(const uint8_t* P, int P_maxidx);
    int AdcGetValuePolarityCurrent(uint8_t* P, int P_maxidx = 5);
    int AdcSetVoltageChannel(const uint8_t* P, int P_maxidx = 5);
    int AdcGetVoltageChannel(uint8_t* P, int P_maxidx = 5);
    int AdcSetPauseBeforeMeasure(const uint8_t* P, int P_maxidx = 5);
    int AdcGetPauseBeforeMeasure(uint8_t* P, int P_maxidx = 5);
    int AdcSetAmountMeasurements(const uint8_t* P, int P_maxidx = 5);
    int AdcGetAmountMeasurements(uint8_t* P, int P_maxidx = 5);
    int AdcSetAmountAverage(const uint8_t* P, int P_maxidx = 5);
    int AdcGetAmountAverage(uint8_t* P, int P_maxidx = 5);
    int AdcSetPack(const QString& P);
    int AdcGetPack(QString& P);
    int AdcShowModal(const QString& Caption, TAccessType Access, bool& M);

    int AdcSetMeasureChannel(uint8_t measureChannel);
    int AdcSetSupportingResistor(uint8_t sr);
    int AdcSetValuePolarityCurrent(uint8_t current);

    //int ASPT_DirectCalculation(TNSH NSH, long double Value, long double Tmin, long double Tmax, const long double* Coefs, int Coefs_maxidx, long double& Temper);
    //int ASPT_ReverseCalculation(TNSH NSH, long double Temper, long double Tmin, long double Tmax, const long double* Coefs, int Coefs_maxidx, long double& Value);
    //int ASPT_ProtocolByPoints(uint8_t PointCount, uint8_t DeflectionClass, TNSH NSH, long double Tmin, long double Tmax, long double* MIn, int MIn_maxidx, long double* MOut, int MOut_maxidx, Char*& ResultMessage);
    //int ASPT_ProtocolByPointsKTPR(uint8_t PointCount, uint8_t DeflectionClass1, uint8_t DeflectionClass2, TNSH NSH, bool Percent, long double R, long double X0, long double X1, long double Tmin, long double Tmax, long double dTmin, long double* MIn, int MIn_maxidx, long double* MOut, int MOut_maxidx, long double* Dev, int Dev_maxidx, Char*& ResultMessage);

    long RetCode() const;

private:
    QLibrary asptLib;
    Char CharArray[65536 /*# range 0..MaxParcelLength*/];

    double DtoR48(double d);
    double R48toD(double r);

    HANDLE hAdc;
    HANDLE hDev;
    long retCode;

    //--- Функции работы с АСПТ ----------------------------------------------------
    /*создание объекта TDevice*/
    typedef THandle(__stdcall* pASPT_Device_Create)();
    pASPT_Device_Create ASPT_Device_Create = nullptr;

    /*удаление объекта TDevice*/
    typedef int(__stdcall* pASPT_Device_Free)(THandle&);
    pASPT_Device_Free ASPT_Device_Free = nullptr;

    /*установка COM-порта*/
    typedef int(__stdcall* pASPT_Device_SetCOMN)(THandle, const Char*);
    pASPT_Device_SetCOMN ASPT_Device_SetCOMN = nullptr;

    /*определение COM-порта*/
    typedef int(__stdcall* pASPT_Device_GetCOMN)(THandle, Char*);
    pASPT_Device_GetCOMN ASPT_Device_GetCOMN = nullptr;

    /*установка скорости обмена*/
    typedef int(__stdcall* pASPT_Device_SetBaudRate)(THandle, uint16_t);
    pASPT_Device_SetBaudRate ASPT_Device_SetBaudRate = nullptr;

    /*определение скорости обмена*/
    typedef int(__stdcall* pASPT_Device_GetBaudRate)(THandle, uint16_t&);
    pASPT_Device_GetBaudRate ASPT_Device_GetBaudRate = nullptr;

    /*установка сетевого адреса*/
    typedef int(__stdcall* pASPT_Device_SetNumber)(THandle, uint8_t);
    pASPT_Device_SetNumber ASPT_Device_SetNumber = nullptr;

    /*определение сетевого адреса*/
    typedef int(__stdcall* pASPT_Device_GetNumber)(THandle, uint8_t&);
    pASPT_Device_GetNumber ASPT_Device_GetNumber = nullptr;

    /*изменение сетевого адреса*/
    typedef int(__stdcall* pASPT_Device_ChangeNumber)(THandle, uint8_t);
    pASPT_Device_ChangeNumber ASPT_Device_ChangeNumber = nullptr;

    /*чтение из EEPROM*/
    typedef int(__stdcall* pASPT_Device_ReadEEPROM)(THandle, uint16_t, TParcelMas&, uint16_t&);
    pASPT_Device_ReadEEPROM ASPT_Device_ReadEEPROM = nullptr;

    /*запись в EEPROM*/
    typedef int(__stdcall* pASPT_Device_WriteEEPROM)(THandle, uint16_t, TParcelMas, uint16_t);
    pASPT_Device_WriteEEPROM ASPT_Device_WriteEEPROM = nullptr;

    /*чтение параметра прибора*/
    typedef int(__stdcall* pASPT_Device_ReadParameter)(THandle, TDeviceParameter, TParameter&);
    pASPT_Device_ReadParameter ASPT_Device_ReadParameter = nullptr;

    /*запись параметра прибора*/
    typedef int(__stdcall* pASPT_Device_WriteParameter)(THandle, TDeviceParameter, TParameter);
    pASPT_Device_WriteParameter ASPT_Device_WriteParameter = nullptr;

    /*измерение*/
    typedef int(__stdcall* pASPT_Device_Measure)(THandle, THandle, double*, int);
    pASPT_Device_Measure ASPT_Device_Measure = nullptr;

    /*получение измеренной величины*/
    typedef int(__stdcall* pASPT_Device_GetMeasureValue)(THandle, THandle, TSensorType, TNSH, double, double, double, const long double*, int, double&, double&);
    pASPT_Device_GetMeasureValue ASPT_Device_GetMeasureValue = nullptr;

    /*инициализация прибора (необходимо производить перед началом измерений)*/
    typedef int(__stdcall* pASPT_Device_Initialize)(THandle);
    pASPT_Device_Initialize ASPT_Device_Initialize = nullptr;

    /*получение температуры прибора*/
    typedef int(__stdcall* pASPT_Device_GetTemperature)(THandle, double&);
    pASPT_Device_GetTemperature ASPT_Device_GetTemperature = nullptr;

    /*температурная коррекция коэффициентов*/
    typedef int(__stdcall* pASPT_Device_Correction)(THandle);
    pASPT_Device_Correction ASPT_Device_Correction = nullptr;

    /*установка параметров связи*/
    typedef int(__stdcall* pASPT_Device_SetTieParameters)(THandle, const Char*, uint16_t, uint8_t);
    pASPT_Device_SetTieParameters ASPT_Device_SetTieParameters = nullptr;

    /*чтение откорректированного значения параметра*/
    typedef int(__stdcall* pASPT_Device_GetCorrectParamValue)(THandle, TDeviceParameter, double&);
    pASPT_Device_GetCorrectParamValue ASPT_Device_GetCorrectParamValue = nullptr;

    /*чтение значения параметра*/
    typedef int(__stdcall* pASPT_Device_GetParamValue)(THandle, TDeviceParameter, double&);
    pASPT_Device_GetParamValue ASPT_Device_GetParamValue = nullptr;

    /* Функции работы с модулем настройки АЦП АСПТ */
    /*создание/удаление объекта TADCCfg*/
    typedef THandle(__stdcall* pASPT_ADCCfg_Create)();
    pASPT_ADCCfg_Create ASPT_ADCCfg_Create = nullptr;
    typedef int(__stdcall* pASPT_ADCCfg_Free)(THandle&);
    pASPT_ADCCfg_Free ASPT_ADCCfg_Free = nullptr;

    /*установка/получение шагов, используемых для измерения (1-участвует, 0-не_участвует)*/
    typedef int(__stdcall* pASPT_ADCCfg_SetUseMeasureStep)(THandle, const uint8_t*, int);
    pASPT_ADCCfg_SetUseMeasureStep ASPT_ADCCfg_SetUseMeasureStep = nullptr;
    typedef int(__stdcall* pASPT_ADCCfg_GetUseMeasureStep)(THandle, uint8_t*, int);
    pASPT_ADCCfg_GetUseMeasureStep ASPT_ADCCfg_GetUseMeasureStep = nullptr;

    /*установка/получение флага сброса АЦП*/
    typedef int(__stdcall* pASPT_ADCCfg_SetUnsetADC)(THandle, const uint8_t*, int);
    pASPT_ADCCfg_SetUnsetADC ASPT_ADCCfg_SetUnsetADC = nullptr;
    typedef int(__stdcall* pASPT_ADCCfg_GetUnsetADC)(THandle, uint8_t*, int);
    pASPT_ADCCfg_GetUnsetADC ASPT_ADCCfg_GetUnsetADC = nullptr;

    /*установка/получение флага автокалибровки*/
    typedef int(__stdcall* pASPT_ADCCfg_SetAutoCalibrationADC)(THandle, const uint8_t*, int);
    pASPT_ADCCfg_SetAutoCalibrationADC ASPT_ADCCfg_SetAutoCalibrationADC = nullptr;
    typedef int(__stdcall* pASPT_ADCCfg_GetAutoCalibrationADC)(THandle, uint8_t*, int);
    pASPT_ADCCfg_GetAutoCalibrationADC ASPT_ADCCfg_GetAutoCalibrationADC = nullptr;

    /*установка/получение флага сброса фильтра*/
    typedef int(__stdcall* pASPT_ADCCfg_SetUnsetFilter)(THandle, const uint8_t*, int);
    pASPT_ADCCfg_SetUnsetFilter ASPT_ADCCfg_SetUnsetFilter = nullptr;
    typedef int(__stdcall* pASPT_ADCCfg_GetUnsetFilter)(THandle, uint8_t*, int);
    pASPT_ADCCfg_GetUnsetFilter ASPT_ADCCfg_GetUnsetFilter = nullptr;

    /*установка/получение флага включения входного буфера*/
    typedef int(__stdcall* pASPT_ADCCfg_SetInputBuffer)(THandle, const uint8_t*, int);
    pASPT_ADCCfg_SetInputBuffer ASPT_ADCCfg_SetInputBuffer = nullptr;
    typedef int(__stdcall* pASPT_ADCCfg_GetInputBuffer)(THandle, uint8_t*, int);
    pASPT_ADCCfg_GetInputBuffer ASPT_ADCCfg_GetInputBuffer = nullptr;

    /*установка/получение флага включения тока*/
    typedef int(__stdcall* pASPT_ADCCfg_SetStateCurrent)(THandle, const uint8_t*, int);
    pASPT_ADCCfg_SetStateCurrent ASPT_ADCCfg_SetStateCurrent = nullptr;
    typedef int(__stdcall* pASPT_ADCCfg_GetStateCurrent)(THandle, uint8_t*, int);
    pASPT_ADCCfg_GetStateCurrent ASPT_ADCCfg_GetStateCurrent = nullptr;

    /*установка/получение коэф-та усиления*/
    typedef int(__stdcall* pASPT_ADCCfg_SetGainFactor)(THandle, const uint8_t*, int);
    pASPT_ADCCfg_SetGainFactor ASPT_ADCCfg_SetGainFactor = nullptr;
    typedef int(__stdcall* pASPT_ADCCfg_GetGainFactor)(THandle, uint8_t*, int);
    pASPT_ADCCfg_GetGainFactor ASPT_ADCCfg_GetGainFactor = nullptr;

    /*установка/получение частоты фильтра*/
    typedef int(__stdcall* pASPT_ADCCfg_SetFilterFrequency)(THandle, const uint8_t*, int);
    pASPT_ADCCfg_SetFilterFrequency ASPT_ADCCfg_SetFilterFrequency = nullptr;
    typedef int(__stdcall* pASPT_ADCCfg_GetFilterFrequency)(THandle, uint8_t*, int);
    pASPT_ADCCfg_GetFilterFrequency ASPT_ADCCfg_GetFilterFrequency = nullptr;

    /*установка/получение номера канала АСПТ*/
    typedef int(__stdcall* pASPT_ADCCfg_SetMeasureChannel)(THandle, const uint8_t*, int);
    pASPT_ADCCfg_SetMeasureChannel ASPT_ADCCfg_SetMeasureChannel = nullptr;
    typedef int(__stdcall* pASPT_ADCCfg_GetMeasureChannel)(THandle, uint8_t*, int);
    pASPT_ADCCfg_GetMeasureChannel ASPT_ADCCfg_GetMeasureChannel = nullptr;

    /*установка/получение номера опорного резистора*/
    typedef int(__stdcall* pASPT_ADCCfg_SetSupportingResistor)(THandle, const uint8_t*, int);
    pASPT_ADCCfg_SetSupportingResistor ASPT_ADCCfg_SetSupportingResistor = nullptr;
    typedef int(__stdcall* pASPT_ADCCfg_GetSupportingResistor)(THandle, uint8_t*, int);
    pASPT_ADCCfg_GetSupportingResistor ASPT_ADCCfg_GetSupportingResistor = nullptr;

    /*установка/получение значения тока*/
    typedef int(__stdcall* pASPT_ADCCfg_SetValuePolarityCurrent)(THandle, const uint8_t*, int);
    pASPT_ADCCfg_SetValuePolarityCurrent ASPT_ADCCfg_SetValuePolarityCurrent = nullptr;
    typedef int(__stdcall* pASPT_ADCCfg_GetValuePolarityCurrent)(THandle, uint8_t*, int);
    pASPT_ADCCfg_GetValuePolarityCurrent ASPT_ADCCfg_GetValuePolarityCurrent = nullptr;

    /*установка/получение номера канала измеряемого напряжения*/
    typedef int(__stdcall* pASPT_ADCCfg_SetVoltageChannel)(THandle, const uint8_t*, int);
    pASPT_ADCCfg_SetVoltageChannel ASPT_ADCCfg_SetVoltageChannel = nullptr;
    typedef int(__stdcall* pASPT_ADCCfg_GetVoltageChannel)(THandle, uint8_t*, int);
    pASPT_ADCCfg_GetVoltageChannel ASPT_ADCCfg_GetVoltageChannel = nullptr;

    /*установка/получение паузы перед измерением*/
    typedef int(__stdcall* pASPT_ADCCfg_SetPauseBeforeMeasure)(THandle, const uint8_t*, int);
    pASPT_ADCCfg_SetPauseBeforeMeasure ASPT_ADCCfg_SetPauseBeforeMeasure = nullptr;
    typedef int(__stdcall* pASPT_ADCCfg_GetPauseBeforeMeasure)(THandle, uint8_t*, int);
    pASPT_ADCCfg_GetPauseBeforeMeasure ASPT_ADCCfg_GetPauseBeforeMeasure = nullptr;

    /*установка/получение пропущенных измерений*/
    typedef int(__stdcall* pASPT_ADCCfg_SetAmountMeasurements)(THandle, const uint8_t*, int);
    pASPT_ADCCfg_SetAmountMeasurements ASPT_ADCCfg_SetAmountMeasurements = nullptr;
    typedef int(__stdcall* pASPT_ADCCfg_GetAmountMeasurements)(THandle, uint8_t*, int);
    pASPT_ADCCfg_GetAmountMeasurements ASPT_ADCCfg_GetAmountMeasurements = nullptr;

    /*установка/получение количества усреднений*/
    typedef int(__stdcall* pASPT_ADCCfg_SetAmountAverage)(THandle, const uint8_t*, int);
    pASPT_ADCCfg_SetAmountAverage ASPT_ADCCfg_SetAmountAverage = nullptr;
    typedef int(__stdcall* pASPT_ADCCfg_GetAmountAverage)(THandle, uint8_t*, int);
    pASPT_ADCCfg_GetAmountAverage ASPT_ADCCfg_GetAmountAverage = nullptr;

    /*установка/получение параметров АЦП по строке с параметрами (use in *.ini)*/
    typedef int(__stdcall* pASPT_ADCCfg_SetPack)(THandle, const Char* P);
    pASPT_ADCCfg_SetPack ASPT_ADCCfg_SetPack = nullptr;
    typedef int(__stdcall* pASPT_ADCCfg_GetPack)(THandle, Char* P);
    pASPT_ADCCfg_GetPack ASPT_ADCCfg_GetPack = nullptr;

    /*вывод формы конфигурации АЦП*/
    typedef int(__stdcall* pASPT_ADCCfg_ShowModal)(THandle, const Char*, TAccessType, bool&);
    pASPT_ADCCfg_ShowModal ASPT_ADCCfg_ShowModal = nullptr;

    ///* Функции обработки результатов измерения */
    ///*расчет температуры из измеренного значения*/
    //typedef int(__stdcall* pASPT_DirectCalculation)(TNSH NSH, long double Value, long double Tmin, long double Tmax, const long double* Coefs, int Coefs_maxidx, long double& Temper);
    //pASPT_DirectCalculation ASPT_DirectCalculation = nullptr;
    ///*расчет значения из температуры*/
    //typedef int(__stdcall* pASPT_ReverseCalculation)(TNSH NSH, long double Temper, long double Tmin, long double Tmax, const long double* Coefs, int Coefs_maxidx, long double& Value);
    //pASPT_ReverseCalculation ASPT_ReverseCalculation = nullptr;
    ///*расчет данных для протокола*/
    //typedef int(__stdcall* pASPT_ProtocolByPoints)(uint8_t PointCount, uint8_t DeflectionClass, TNSH NSH, long double Tmin, long double Tmax, long double* MIn, int MIn_maxidx, long double* MOut, int MOut_maxidx, Char*& ResultMessage);
    //pASPT_ProtocolByPoints ASPT_ProtocolByPoints = nullptr;
    ///*расчет данных для протокола (КТПР)*/
    //typedef int(__stdcall* pASPT_ProtocolByPointsKTPR)(uint8_t PointCount, uint8_t DeflectionClass1, uint8_t DeflectionClass2, TNSH NSH, bool Percent, long double R, long double X0, long double X1, long double Tmin, long double Tmax, long double dTmin, long double* MIn, int MIn_maxidx, long double* MOut, int MOut_maxidx, long double* Dev, int Dev_maxidx, Char*& ResultMessage);
    //pASPT_ProtocolByPointsKTPR ASPT_ProtocolByPointsKTPR = nullptr;
};

#endif //ASPTCLASS_H
