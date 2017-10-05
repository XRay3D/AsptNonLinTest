#include "aspt.h"
#include <QApplication>
#include <QDebug>
#include <QMessageBox>

#define MAXIDX(x) (sizeof(x) / sizeof(x[0])) - 1

//QString GetLastErrorAsString()
//{
//#ifndef linux
//    //Get the error message, if any.
//    DWORD errorMessageID = ::GetLastError();
//    if (errorMessageID == 0)
//        return QString(); //No error message has been recorded
//    LPSTR messageBuffer = nullptr;
//    size_t size = ::FormatMessageA(
//        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
//        NULL,
//        errorMessageID,
//        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//        (LPSTR)&messageBuffer,
//        0,
//        NULL);
//    QString message(QString::fromLocal8Bit(messageBuffer, size));
//    //Free the buffer.
//    ::LocalFree(messageBuffer);
//    return message;
//#else
//    return "";
//#endif
//}

AsptClass::AsptClass()
    : hDev(0)
    , hAdc(0)
    , retCode(0)
    , asptLib("aspt5")
{
    if (asptLib.load()) {
        ASPT_Device_Create = pASPT_Device_Create(asptLib.resolve("ASPT_Device_Create"));
        ASPT_Device_Free = pASPT_Device_Free(asptLib.resolve("ASPT_Device_Free"));
        ASPT_Device_SetCOMN = pASPT_Device_SetCOMN(asptLib.resolve("ASPT_Device_SetCOMN"));
        ASPT_Device_GetCOMN = pASPT_Device_GetCOMN(asptLib.resolve("ASPT_Device_GetCOMN"));
        ASPT_Device_SetBaudRate = pASPT_Device_SetBaudRate(asptLib.resolve("ASPT_Device_SetBaudRate"));
        ASPT_Device_GetBaudRate = pASPT_Device_GetBaudRate(asptLib.resolve("ASPT_Device_GetBaudRate"));
        ASPT_Device_SetNumber = pASPT_Device_SetNumber(asptLib.resolve("ASPT_Device_SetNumber"));
        ASPT_Device_GetNumber = pASPT_Device_GetNumber(asptLib.resolve("ASPT_Device_GetNumber"));
        ASPT_Device_ChangeNumber = pASPT_Device_ChangeNumber(asptLib.resolve("ASPT_Device_ChangeNumber"));
        ASPT_Device_ReadEEPROM = pASPT_Device_ReadEEPROM(asptLib.resolve("ASPT_Device_ReadEEPROM"));
        ASPT_Device_WriteEEPROM = pASPT_Device_WriteEEPROM(asptLib.resolve("ASPT_Device_WriteEEPROM"));
        ASPT_Device_ReadParameter = pASPT_Device_ReadParameter(asptLib.resolve("ASPT_Device_ReadParameter"));
        ASPT_Device_WriteParameter = pASPT_Device_WriteParameter(asptLib.resolve("ASPT_Device_WriteParameter"));
        ASPT_Device_Measure = pASPT_Device_Measure(asptLib.resolve("ASPT_Device_Measure"));
        ASPT_Device_GetMeasureValue = pASPT_Device_GetMeasureValue(asptLib.resolve("ASPT_Device_GetMeasureValue"));
        ASPT_Device_Initialize = pASPT_Device_Initialize(asptLib.resolve("ASPT_Device_Initialize"));
        ASPT_Device_GetTemperature = pASPT_Device_GetTemperature(asptLib.resolve("ASPT_Device_GetTemperature"));
        ASPT_Device_Correction = pASPT_Device_Correction(asptLib.resolve("ASPT_Device_Correction"));
        ASPT_Device_SetTieParameters = pASPT_Device_SetTieParameters(asptLib.resolve("ASPT_Device_SetTieParameters"));
        ASPT_Device_GetCorrectParamValue = pASPT_Device_GetCorrectParamValue(asptLib.resolve("ASPT_Device_GetCorrectParamValue"));
        ASPT_Device_GetParamValue = pASPT_Device_GetParamValue(asptLib.resolve("ASPT_Device_GetParamValue"));

        ASPT_ADCCfg_Create = pASPT_ADCCfg_Create(asptLib.resolve("ASPT_ADCCfg_Create"));
        ASPT_ADCCfg_Free = pASPT_ADCCfg_Free(asptLib.resolve("ASPT_ADCCfg_Free"));
        ASPT_ADCCfg_SetUseMeasureStep = pASPT_ADCCfg_SetUseMeasureStep(asptLib.resolve("ASPT_ADCCfg_SetUseMeasureStep"));
        ASPT_ADCCfg_GetUseMeasureStep = pASPT_ADCCfg_GetUseMeasureStep(asptLib.resolve("ASPT_ADCCfg_GetUseMeasureStep"));
        ASPT_ADCCfg_SetUnsetADC = pASPT_ADCCfg_SetUnsetADC(asptLib.resolve("ASPT_ADCCfg_SetUnsetADC"));
        ASPT_ADCCfg_GetUnsetADC = pASPT_ADCCfg_GetUnsetADC(asptLib.resolve("ASPT_ADCCfg_GetUnsetADC"));
        ASPT_ADCCfg_SetAutoCalibrationADC = pASPT_ADCCfg_SetAutoCalibrationADC(asptLib.resolve("ASPT_ADCCfg_SetAutoCalibrationADC"));
        ASPT_ADCCfg_GetAutoCalibrationADC = pASPT_ADCCfg_GetAutoCalibrationADC(asptLib.resolve("ASPT_ADCCfg_GetAutoCalibrationADC"));
        ASPT_ADCCfg_SetUnsetFilter = pASPT_ADCCfg_SetUnsetFilter(asptLib.resolve("ASPT_ADCCfg_SetUnsetFilter"));
        ASPT_ADCCfg_GetUnsetFilter = pASPT_ADCCfg_GetUnsetFilter(asptLib.resolve("ASPT_ADCCfg_GetUnsetFilter"));
        ASPT_ADCCfg_SetInputBuffer = pASPT_ADCCfg_SetInputBuffer(asptLib.resolve("ASPT_ADCCfg_SetInputBuffer"));
        ASPT_ADCCfg_GetInputBuffer = pASPT_ADCCfg_GetInputBuffer(asptLib.resolve("ASPT_ADCCfg_GetInputBuffer"));
        ASPT_ADCCfg_SetStateCurrent = pASPT_ADCCfg_SetStateCurrent(asptLib.resolve("ASPT_ADCCfg_SetStateCurrent"));
        ASPT_ADCCfg_GetStateCurrent = pASPT_ADCCfg_GetStateCurrent(asptLib.resolve("ASPT_ADCCfg_GetStateCurrent"));
        ASPT_ADCCfg_SetGainFactor = pASPT_ADCCfg_SetGainFactor(asptLib.resolve("ASPT_ADCCfg_SetGainFactor"));
        ASPT_ADCCfg_GetGainFactor = pASPT_ADCCfg_GetGainFactor(asptLib.resolve("ASPT_ADCCfg_GetGainFactor"));
        ASPT_ADCCfg_SetFilterFrequency = pASPT_ADCCfg_SetFilterFrequency(asptLib.resolve("ASPT_ADCCfg_SetFilterFrequency"));
        ASPT_ADCCfg_GetFilterFrequency = pASPT_ADCCfg_GetFilterFrequency(asptLib.resolve("ASPT_ADCCfg_GetFilterFrequency"));
        ASPT_ADCCfg_SetMeasureChannel = pASPT_ADCCfg_SetMeasureChannel(asptLib.resolve("ASPT_ADCCfg_SetMeasureChannel"));
        ASPT_ADCCfg_GetMeasureChannel = pASPT_ADCCfg_GetMeasureChannel(asptLib.resolve("ASPT_ADCCfg_GetMeasureChannel"));
        ASPT_ADCCfg_SetSupportingResistor = pASPT_ADCCfg_SetSupportingResistor(asptLib.resolve("ASPT_ADCCfg_SetSupportingResistor"));
        ASPT_ADCCfg_GetSupportingResistor = pASPT_ADCCfg_GetSupportingResistor(asptLib.resolve("ASPT_ADCCfg_GetSupportingResistor"));
        ASPT_ADCCfg_SetValuePolarityCurrent = pASPT_ADCCfg_SetValuePolarityCurrent(asptLib.resolve("ASPT_ADCCfg_SetValuePolarityCurrent"));
        ASPT_ADCCfg_GetValuePolarityCurrent = pASPT_ADCCfg_GetValuePolarityCurrent(asptLib.resolve("ASPT_ADCCfg_GetValuePolarityCurrent"));
        ASPT_ADCCfg_SetVoltageChannel = pASPT_ADCCfg_SetVoltageChannel(asptLib.resolve("ASPT_ADCCfg_SetVoltageChannel"));
        ASPT_ADCCfg_GetVoltageChannel = pASPT_ADCCfg_GetVoltageChannel(asptLib.resolve("ASPT_ADCCfg_GetVoltageChannel"));
        ASPT_ADCCfg_SetPauseBeforeMeasure = pASPT_ADCCfg_SetPauseBeforeMeasure(asptLib.resolve("ASPT_ADCCfg_SetPauseBeforeMeasure"));
        ASPT_ADCCfg_GetPauseBeforeMeasure = pASPT_ADCCfg_GetPauseBeforeMeasure(asptLib.resolve("ASPT_ADCCfg_GetPauseBeforeMeasure"));
        ASPT_ADCCfg_SetAmountMeasurements = pASPT_ADCCfg_SetAmountMeasurements(asptLib.resolve("ASPT_ADCCfg_SetAmountMeasurements"));
        ASPT_ADCCfg_GetAmountMeasurements = pASPT_ADCCfg_GetAmountMeasurements(asptLib.resolve("ASPT_ADCCfg_GetAmountMeasurements"));
        ASPT_ADCCfg_SetAmountAverage = pASPT_ADCCfg_SetAmountAverage(asptLib.resolve("ASPT_ADCCfg_SetAmountAverage"));
        ASPT_ADCCfg_GetAmountAverage = pASPT_ADCCfg_GetAmountAverage(asptLib.resolve("ASPT_ADCCfg_GetAmountAverage"));
        ASPT_ADCCfg_SetPack = pASPT_ADCCfg_SetPack(asptLib.resolve("ASPT_ADCCfg_SetPack"));
        ASPT_ADCCfg_GetPack = pASPT_ADCCfg_GetPack(asptLib.resolve("ASPT_ADCCfg_GetPack"));
        ASPT_ADCCfg_ShowModal = pASPT_ADCCfg_ShowModal(asptLib.resolve("ASPT_ADCCfg_ShowModal"));

        //ASPT_DirectCalculation = pASPT_DirectCalculation(saptLib.resolve("ASPT_DirectCalculation"));
        //ASPT_ReverseCalculation = pASPT_ReverseCalculation(saptLib.resolve("ASPT_ReverseCalculation"));
        //ASPT_ProtocolByPoints = pASPT_ProtocolByPoints(saptLib.resolve("ASPT_ProtocolByPoints"));
        //ASPT_ProtocolByPointsKTPR = pASPT_ProtocolByPointsKTPR(saptLib.resolve("ASPT_ProtocolByPointsKTPR"));

        hDev = ASPT_Device_Create();
        if (DBG)
            qDebug() << "ASPT_Device_Create" << hDev;
        hAdc = ASPT_ADCCfg_Create();
        if (DBG)
            qDebug() << "ASPT_ADCCfg_Create" << hAdc;
    }
    else {
        QMessageBox::critical(0, "", "Не айдена aspt5.dll!");
        if (DBG)
            qDebug() << asptLib.errorString();
    }
}

AsptClass::~AsptClass()
{
    if (asptLib.isLoaded()) {
        retCode = ASPT_ADCCfg_Free(hAdc);
        if (DBG)
            qDebug() << "ASPT_ADCCfg_Free" << retCode;
        retCode = ASPT_Device_Free(hDev);
        if (DBG)
            qDebug() << "ASPT_Device_Free" << retCode;
        asptLib.unload();
    }
}

int AsptClass::DevSetCOMN(const QString& CPS)
{
    retCode = -1;
    if (ASPT_Device_SetCOMN != nullptr)
        retCode = ASPT_Device_SetCOMN(hDev, CPS.toStdWString().data());
    if (DBG)
        qDebug() << "ASPT_Device_SetCOMN" << retCode;
    return retCode;
}

int AsptClass::DevGetCOMN(QString& CPS)
{
    retCode = -1;
    if (ASPT_Device_GetCOMN != nullptr) {
        retCode = ASPT_Device_GetCOMN(hDev, CharArray);
        CPS = QString::fromWCharArray(CharArray);
    }
    if (DBG)
        qDebug() << "ASPT_Device_GetCOMN" << retCode;
    return retCode;
}

int AsptClass::DevSetBaudRate(uint16_t BaudRate)
{
    retCode = -1;
    if (ASPT_Device_SetBaudRate != nullptr)
        retCode = ASPT_Device_SetBaudRate(hDev, BaudRate);
    if (DBG)
        qDebug() << "ASPT_Device_SetBaudRate" << retCode;
    return retCode;
}

int AsptClass::DevGetBaudRate(uint16_t& BaudRate)
{
    retCode = -1;
    if (ASPT_Device_GetBaudRate != nullptr)
        retCode = ASPT_Device_GetBaudRate(hDev, BaudRate);
    if (DBG)
        qDebug() << "ASPT_Device_GetBaudRate" << retCode;
    return retCode;
}

int AsptClass::DevSetNumber(uint8_t DeviceNumber)
{
    retCode = -1;
    if (ASPT_Device_SetNumber != nullptr)
        retCode = ASPT_Device_SetNumber(hDev, DeviceNumber);
    if (DBG)
        qDebug() << "ASPT_Device_SetNumber" << retCode;
    return retCode;
}

int AsptClass::DevGetNumber(uint8_t& DeviceNumber)
{
    retCode = -1;
    if (ASPT_Device_GetNumber != nullptr)
        retCode = ASPT_Device_GetNumber(hDev, DeviceNumber);
    if (DBG)
        qDebug() << "ASPT_Device_GetNumber" << retCode;
    return retCode;
}

int AsptClass::DevChangeNumber(uint8_t NewDevNum)
{
    retCode = -1;
    if (ASPT_Device_ChangeNumber != nullptr)
        retCode = ASPT_Device_ChangeNumber(hDev, NewDevNum);
    if (DBG)
        qDebug() << "ASPT_Device_ChangeNumber" << retCode;
    return retCode;
}

int AsptClass::DevReadEEPROM(uint16_t Address, TParcelMas& B, uint16_t& BS)
{
    retCode = -1;
    if (ASPT_Device_ReadEEPROM != nullptr)
        retCode = ASPT_Device_ReadEEPROM(hDev, Address, B, BS);
    if (DBG)
        qDebug() << "ASPT_Device_ReadEEPROM" << retCode;
    return retCode;
}

int AsptClass::DevWriteEEPROM(uint16_t Address, TParcelMas B, uint16_t BS)
{
    retCode = -1;
    if (ASPT_Device_WriteEEPROM != nullptr)
        retCode = ASPT_Device_WriteEEPROM(hDev, Address, B, BS);
    if (DBG)
        qDebug() << "ASPT_Device_WriteEEPROM" << retCode;
    return retCode;
}

int AsptClass::DevReadParameter(TDeviceParameter DevParam, TParameter& P)
{
    retCode = -1;
    if (ASPT_Device_ReadParameter != nullptr) {
        retCode = ASPT_Device_ReadParameter(hDev, DevParam, P);
        P.Date = R48toD(P.Date);
        P.Value = R48toD(P.Value);
    }
    if (DBG)
        qDebug() << "ASPT_Device_ReadParameter" << retCode;
    return retCode;
}

int AsptClass::DevWriteParameter(TDeviceParameter DevParam, TParameter P)
{
    retCode = -1;
    if (ASPT_Device_WriteParameter != nullptr)
        retCode = ASPT_Device_WriteParameter(hDev, DevParam, P);
    if (DBG)
        qDebug() << "ASPT_Device_WriteParameter" << retCode;
    return retCode;
}

int AsptClass::DevMeasure(double* M, int M_maxidx)
{
    retCode = -1;
    if (ASPT_Device_Measure != nullptr)
        retCode = ASPT_Device_Measure(hDev, hAdc, M, M_maxidx);
    if (DBG)
        qDebug() << "ASPT_Device_Measure" << retCode;
    return retCode;
}

int AsptClass::DevGetMeasureValue(TSensorType SensorType, TNSH NSH, double VTmin, double VTmax, double VCable, const long double* Coefs, int Coefs_maxidx, double& Value, double& Temperature)
{
    retCode = -1;
    if (ASPT_Device_GetMeasureValue != nullptr) {
        VTmin = DtoR48(VTmin);
        VTmax = DtoR48(VTmax);
        VCable = DtoR48(VCable);
        retCode = ASPT_Device_GetMeasureValue(hDev, hAdc, SensorType, NSH, VTmin, VTmax, VCable, Coefs, Coefs_maxidx, Value, Temperature);
        Value = R48toD(Value);
        Temperature = R48toD(Temperature);
    }
    if (DBG)
        qDebug() << "ASPT_Device_GetMeasureValue" << retCode;
    return retCode;
}

int AsptClass::DevInitialize()
{
    retCode = -1;
    if (ASPT_Device_Initialize != nullptr)
        retCode = ASPT_Device_Initialize(hDev);
    if (DBG)
        qDebug() << "ASPT_Device_Initialize" << retCode;
    return retCode;
}

int AsptClass::DevGetTemperature(double& Temperature)
{
    retCode = -1;
    if (ASPT_Device_GetTemperature != nullptr)
        retCode = ASPT_Device_GetTemperature(hDev, Temperature);
    if (DBG)
        qDebug() << "ASPT_Device_GetTemperature" << retCode;
    return retCode;
}

int AsptClass::DevCorrection()
{
    retCode = -1;
    if (ASPT_Device_Correction != nullptr)
        retCode = ASPT_Device_Correction(hDev);
    if (DBG)
        qDebug() << "ASPT_Device_Correction" << retCode;
    return retCode;
}

int AsptClass::DevSetTieParameters(const QString& CPS, uint16_t BaudRate, uint8_t DeviceNumber)
{
    retCode = -1;
    if (ASPT_Device_SetTieParameters != nullptr)
        retCode = ASPT_Device_SetTieParameters(hDev, CPS.toStdWString().data(), BaudRate, DeviceNumber);
    if (DBG)
        qDebug() << "ASPT_Device_SetTieParameters" << retCode;
    return retCode;
}

int AsptClass::DevGetCorrectParamValue(TDeviceParameter DevParam, double& CorrectValue)
{
    retCode = -1;
    if (ASPT_Device_GetCorrectParamValue != nullptr)
        retCode = ASPT_Device_GetCorrectParamValue(hDev, DevParam, CorrectValue);
    if (DBG)
        qDebug() << "ASPT_Device_GetCorrectParamValue" << retCode;
    return retCode;
}

int AsptClass::DevGetParamValue(TDeviceParameter DevParam, double& ParamValue)
{
    retCode = -1;
    if (ASPT_Device_GetParamValue != nullptr)
        retCode = ASPT_Device_GetParamValue(hDev, DevParam, ParamValue);
    if (DBG)
        qDebug() << "ASPT_Device_GetParamValue" << retCode;
    return retCode;
}

int AsptClass::AdcSetUseMeasureStep(const uint8_t* P, int P_maxidx)
{
    retCode = -1;
    if (ASPT_ADCCfg_SetUseMeasureStep != nullptr)
        retCode = ASPT_ADCCfg_SetUseMeasureStep(hAdc, P, P_maxidx);
    if (DBG)
        qDebug() << "ASPT_ADCCfg_SetUseMeasureStep" << retCode;
    return retCode;
}

int AsptClass::AdcGetUseMeasureStep(uint8_t* P, int P_maxidx)
{
    retCode = -1;
    if (ASPT_ADCCfg_GetUseMeasureStep != nullptr)
        retCode = ASPT_ADCCfg_GetUseMeasureStep(hAdc, P, P_maxidx);
    if (DBG)
        qDebug() << "ASPT_ADCCfg_GetUseMeasureStep" << retCode;
    return retCode;
}

int AsptClass::AdcSetUnsetADC(const uint8_t* P, int P_maxidx)
{
    retCode = -1;
    if (ASPT_ADCCfg_SetUnsetADC != nullptr)
        retCode = ASPT_ADCCfg_SetUnsetADC(hAdc, P, P_maxidx);
    if (DBG)
        qDebug() << "ASPT_ADCCfg_SetUnsetADC" << retCode;
    return retCode;
}

int AsptClass::AdcGetUnsetADC(uint8_t* P, int P_maxidx)
{
    retCode = -1;
    if (ASPT_ADCCfg_GetUnsetADC != nullptr)
        retCode = ASPT_ADCCfg_GetUnsetADC(hAdc, P, P_maxidx);
    if (DBG)
        qDebug() << "ASPT_ADCCfg_GetUnsetADC" << retCode;
    return retCode;
}

int AsptClass::AdcSetAutoCalibrationADC(const uint8_t* P, int P_maxidx)
{
    retCode = -1;
    if (ASPT_ADCCfg_SetAutoCalibrationADC != nullptr)
        retCode = ASPT_ADCCfg_SetAutoCalibrationADC(hAdc, P, P_maxidx);
    if (DBG)
        qDebug() << "ASPT_ADCCfg_SetAutoCalibrationADC" << retCode;
    return retCode;
}

int AsptClass::AdcGetAutoCalibrationADC(uint8_t* P, int P_maxidx)
{
    retCode = -1;
    if (ASPT_ADCCfg_GetAutoCalibrationADC != nullptr)
        retCode = ASPT_ADCCfg_GetAutoCalibrationADC(hAdc, P, P_maxidx);
    if (DBG)
        qDebug() << "ASPT_ADCCfg_GetAutoCalibrationADC" << retCode;
    return retCode;
}

int AsptClass::AdcSetUnsetFilter(const uint8_t* P, int P_maxidx)
{
    retCode = -1;
    if (ASPT_ADCCfg_SetUnsetFilter != nullptr)
        retCode = ASPT_ADCCfg_SetUnsetFilter(hAdc, P, P_maxidx);
    if (DBG)
        qDebug() << "ASPT_ADCCfg_SetUnsetFilter" << retCode;
    return retCode;
}

int AsptClass::AdcGetUnsetFilter(uint8_t* P, int P_maxidx)
{
    retCode = -1;
    if (ASPT_ADCCfg_GetUnsetFilter != nullptr)
        retCode = ASPT_ADCCfg_GetUnsetFilter(hAdc, P, P_maxidx);
    if (DBG)
        qDebug() << "ASPT_ADCCfg_GetUnsetFilter" << retCode;
    return retCode;
}

int AsptClass::AdcSetInputBuffer(const uint8_t* P, int P_maxidx)
{
    retCode = -1;
    if (ASPT_ADCCfg_SetInputBuffer != nullptr)
        retCode = ASPT_ADCCfg_SetInputBuffer(hAdc, P, P_maxidx);
    if (DBG)
        qDebug() << "ASPT_ADCCfg_SetInputBuffer" << retCode;
    return retCode;
}

int AsptClass::AdcGetInputBuffer(uint8_t* P, int P_maxidx)
{
    retCode = -1;
    if (ASPT_ADCCfg_GetInputBuffer != nullptr)
        retCode = ASPT_ADCCfg_GetInputBuffer(hAdc, P, P_maxidx);
    if (DBG)
        qDebug() << "ASPT_ADCCfg_GetInputBuffer" << retCode;
    return retCode;
}

int AsptClass::AdcSetStateCurrent(const uint8_t* P, int P_maxidx)
{
    retCode = -1;
    if (ASPT_ADCCfg_SetStateCurrent != nullptr)
        retCode = ASPT_ADCCfg_SetStateCurrent(hAdc, P, P_maxidx);
    if (DBG)
        qDebug() << "ASPT_ADCCfg_SetStateCurrent" << retCode;
    return retCode;
}

int AsptClass::AdcGetStateCurrent(uint8_t* P, int P_maxidx)
{
    retCode = -1;
    if (ASPT_ADCCfg_GetStateCurrent != nullptr)
        retCode = ASPT_ADCCfg_GetStateCurrent(hAdc, P, P_maxidx);
    if (DBG)
        qDebug() << "ASPT_ADCCfg_GetStateCurrent" << retCode;
    return retCode;
}

int AsptClass::AdcSetGainFactor(const uint8_t* P, int P_maxidx)
{
    retCode = -1;
    if (ASPT_ADCCfg_SetGainFactor != nullptr)
        retCode = ASPT_ADCCfg_SetGainFactor(hAdc, P, P_maxidx);
    if (DBG)
        qDebug() << "ASPT_ADCCfg_SetGainFactor" << retCode;
    return retCode;
}

int AsptClass::AdcGetGainFactor(uint8_t* P, int P_maxidx)
{
    retCode = -1;
    if (ASPT_ADCCfg_GetGainFactor != nullptr)
        retCode = ASPT_ADCCfg_GetGainFactor(hAdc, P, P_maxidx);
    if (DBG)
        qDebug() << "ASPT_ADCCfg_GetGainFactor" << retCode;
    return retCode;
}

int AsptClass::AdcSetFilterFrequency(const uint8_t* P, int P_maxidx)
{
    retCode = -1;
    if (ASPT_ADCCfg_SetFilterFrequency != nullptr)
        retCode = ASPT_ADCCfg_SetFilterFrequency(hAdc, P, P_maxidx);
    if (DBG)
        qDebug() << "ASPT_ADCCfg_SetFilterFrequency" << retCode;
    return retCode;
}

int AsptClass::AdcGetFilterFrequency(uint8_t* P, int P_maxidx)
{
    retCode = -1;
    if (ASPT_ADCCfg_GetFilterFrequency != nullptr)
        retCode = ASPT_ADCCfg_GetFilterFrequency(hAdc, P, P_maxidx);
    if (DBG)
        qDebug() << "ASPT_ADCCfg_GetFilterFrequency" << retCode;
    return retCode;
}

int AsptClass::AdcSetMeasureChannel(const uint8_t* P, int P_maxidx)
{
    retCode = -1;
    if (ASPT_ADCCfg_SetMeasureChannel != nullptr)
        retCode = ASPT_ADCCfg_SetMeasureChannel(hAdc, P, P_maxidx);
    if (DBG)
        qDebug() << "ASPT_ADCCfg_SetMeasureChannel" << retCode;
    return retCode;
}

int AsptClass::AdcGetMeasureChannel(uint8_t* P, int P_maxidx)
{
    retCode = -1;
    if (ASPT_ADCCfg_GetMeasureChannel != nullptr)
        retCode = ASPT_ADCCfg_GetMeasureChannel(hAdc, P, P_maxidx);
    if (DBG)
        qDebug() << "ASPT_ADCCfg_GetMeasureChannel" << retCode;
    return retCode;
}

int AsptClass::AdcSetSupportingResistor(const uint8_t* P, int P_maxidx)
{
    retCode = -1;
    if (ASPT_ADCCfg_SetSupportingResistor != nullptr)
        retCode = ASPT_ADCCfg_SetSupportingResistor(hAdc, P, P_maxidx);
    if (DBG)
        qDebug() << "ASPT_ADCCfg_SetSupportingResistor" << retCode;
    return retCode;
}

int AsptClass::AdcGetSupportingResistor(uint8_t* P, int P_maxidx)
{
    retCode = -1;
    if (ASPT_ADCCfg_GetSupportingResistor != nullptr)
        retCode = ASPT_ADCCfg_GetSupportingResistor(hAdc, P, P_maxidx);
    if (DBG)
        qDebug() << "ASPT_ADCCfg_GetSupportingResistor" << retCode;
    return retCode;
}

int AsptClass::AdcSetValuePolarityCurrent(const uint8_t* P, int P_maxidx)
{
    retCode = -1;
    if (ASPT_ADCCfg_SetValuePolarityCurrent != nullptr)
        retCode = ASPT_ADCCfg_SetValuePolarityCurrent(hAdc, P, P_maxidx);
    if (DBG)
        qDebug() << "ASPT_ADCCfg_SetValuePolarityCurrent" << retCode;
    return retCode;
}

int AsptClass::AdcGetValuePolarityCurrent(uint8_t* P, int P_maxidx)
{
    retCode = -1;
    if (ASPT_ADCCfg_GetValuePolarityCurrent != nullptr)
        retCode = ASPT_ADCCfg_GetValuePolarityCurrent(hAdc, P, P_maxidx);
    if (DBG)
        qDebug() << "ASPT_ADCCfg_GetValuePolarityCurrent" << retCode;
    return retCode;
}

int AsptClass::AdcSetVoltageChannel(const uint8_t* P, int P_maxidx)
{
    retCode = -1;
    if (ASPT_ADCCfg_SetVoltageChannel != nullptr)
        retCode = ASPT_ADCCfg_SetVoltageChannel(hAdc, P, P_maxidx);
    if (DBG)
        qDebug() << "ASPT_ADCCfg_SetVoltageChannel" << retCode;
    return retCode;
}

int AsptClass::AdcGetVoltageChannel(uint8_t* P, int P_maxidx)
{
    retCode = -1;
    if (ASPT_ADCCfg_GetVoltageChannel != nullptr)
        retCode = ASPT_ADCCfg_GetVoltageChannel(hAdc, P, P_maxidx);
    if (DBG)
        qDebug() << "ASPT_ADCCfg_GetVoltageChannel" << retCode;
    return retCode;
}

int AsptClass::AdcSetPauseBeforeMeasure(const uint8_t* P, int P_maxidx)
{
    retCode = -1;
    if (ASPT_ADCCfg_SetPauseBeforeMeasure != nullptr)
        retCode = ASPT_ADCCfg_SetPauseBeforeMeasure(hAdc, P, P_maxidx);
    if (DBG)
        qDebug() << "ASPT_ADCCfg_SetPauseBeforeMeasure" << retCode;
    return retCode;
}

int AsptClass::AdcGetPauseBeforeMeasure(uint8_t* P, int P_maxidx)
{
    retCode = -1;
    if (ASPT_ADCCfg_GetPauseBeforeMeasure != nullptr)
        retCode = ASPT_ADCCfg_GetPauseBeforeMeasure(hAdc, P, P_maxidx);
    if (DBG)
        qDebug() << "ASPT_ADCCfg_GetPauseBeforeMeasure" << retCode;
    return retCode;
}

int AsptClass::AdcSetAmountMeasurements(const uint8_t* P, int P_maxidx)
{
    retCode = -1;
    if (ASPT_ADCCfg_SetAmountMeasurements != nullptr)
        retCode = ASPT_ADCCfg_SetAmountMeasurements(hAdc, P, P_maxidx);
    if (DBG)
        qDebug() << "ASPT_ADCCfg_SetAmountMeasurements" << retCode;
    return retCode;
}

int AsptClass::AdcGetAmountMeasurements(uint8_t* P, int P_maxidx)
{
    retCode = -1;
    if (ASPT_ADCCfg_GetAmountMeasurements != nullptr)
        retCode = ASPT_ADCCfg_GetAmountMeasurements(hAdc, P, P_maxidx);
    if (DBG)
        qDebug() << "ASPT_ADCCfg_GetAmountMeasurements" << retCode;
    return retCode;
}

int AsptClass::AdcSetAmountAverage(const uint8_t* P, int P_maxidx)
{
    retCode = -1;
    if (ASPT_ADCCfg_SetAmountAverage != nullptr)
        retCode = ASPT_ADCCfg_SetAmountAverage(hAdc, P, P_maxidx);
    if (DBG)
        qDebug() << "ASPT_ADCCfg_SetAmountAverage" << retCode;
    return retCode;
}

int AsptClass::AdcGetAmountAverage(uint8_t* P, int P_maxidx)
{
    retCode = -1;
    if (ASPT_ADCCfg_GetAmountAverage != nullptr)
        retCode = ASPT_ADCCfg_GetAmountAverage(hAdc, P, P_maxidx);
    if (DBG)
        qDebug() << "ASPT_ADCCfg_GetAmountAverage" << retCode;
    return retCode;
}

int AsptClass::AdcSetPack(const QString& P)
{
    retCode = -1;
    if (ASPT_ADCCfg_SetPack != nullptr)
        retCode = ASPT_ADCCfg_SetPack(hAdc, P.toStdWString().data());
    if (DBG)
        qDebug() << "ASPT_ADCCfg_SetPack" << retCode;
    return retCode;
}

int AsptClass::AdcGetPack(QString& P)
{
    retCode = -1;
    if (ASPT_ADCCfg_GetPack != nullptr) {
        retCode = ASPT_ADCCfg_GetPack(hAdc, CharArray);
        P = QString::fromWCharArray(CharArray);
    }
    if (DBG)
        qDebug() << "ASPT_ADCCfg_GetPack" << retCode;
    return retCode;
}

int AsptClass::AdcShowModal(const QString& Caption, TAccessType Access, bool& M)
{
    retCode = -1;
    if (ASPT_ADCCfg_ShowModal != nullptr)
        retCode = ASPT_ADCCfg_ShowModal(hAdc, Caption.toStdWString().data(), Access, M);
    if (DBG)
        qDebug() << "ASPT_ADCCfg_ShowModal" << retCode;
    return retCode;
}

int AsptClass::AdcSetMeasureChannel(uint8_t measureChannel)
{
    retCode = -1;
    if (ASPT_ADCCfg_SetMeasureChannel != nullptr) {
        uint8_t array[MaxStepCount]{ measureChannel, measureChannel, measureChannel, measureChannel, measureChannel, measureChannel };
        retCode = ASPT_ADCCfg_SetMeasureChannel(hAdc, array, MAXIDX(array));
    }
    if (DBG)
        qDebug() << "ASPT_ADCCfg_SetMeasureChannel" << retCode;
    return retCode;
}

int AsptClass::AdcSetSupportingResistor(uint8_t sr)
{
    retCode = -1;
    if (ASPT_ADCCfg_SetSupportingResistor != nullptr) {
        uint8_t array[MaxStepCount]{ sr, sr, sr, sr, sr, sr };
        retCode = ASPT_ADCCfg_SetSupportingResistor(hAdc, array, MAXIDX(array));
    }
    if (DBG)
        qDebug() << "ASPT_ADCCfg_SetSupportingResistor" << retCode;
    return retCode;
}

int AsptClass::AdcSetValuePolarityCurrent(uint8_t current)
{
    retCode = 0;
    if (ASPT_ADCCfg_GetValuePolarityCurrent != nullptr && ASPT_ADCCfg_SetValuePolarityCurrent != nullptr) {
        uint8_t array[MaxStepCount];
        retCode |= ASPT_ADCCfg_GetValuePolarityCurrent(hAdc, array, MAXIDX(array));
        for (uint8_t& ch : array) {
            ch = (ch & 0xF8) | current;
        }
        retCode |= ASPT_ADCCfg_SetValuePolarityCurrent(hAdc, array, MAXIDX(array));
    }
    if (DBG)
        qDebug() << "ASPT_ADCCfg_GetValuePolarityCurrent|ASPT_ADCCfg_SetValuePolarityCurrent" << retCode;
    return retCode;
}

//int ASPT_CLASS::ASPT_DirectCalculation(TNSH NSH, long double Value, long double Tmin, long double Tmax, const long double* Coefs, int Coefs_maxidx, long double& Temper) { /*ASPT_DirectCalculation*/   retCode =-1;    if (hModule != nullptr)retCode = ();qDebug() << "" << retCode;return retCode;}
//int ASPT_CLASS::ASPT_ReverseCalculation(TNSH NSH, long double Temper, long double Tmin, long double Tmax, const long double* Coefs, int Coefs_maxidx, long double& Value) { /*ASPT_ReverseCalculation*/   retCode =-1;    if (hModule != nullptr)retCode = ();qDebug() << "" << retCode;return retCode;}
//int ASPT_CLASS::ASPT_ProtocolByPoints(uint8_t PointCount, uint8_t DeflectionClass, TNSH NSH, long double Tmin, long double Tmax, long double* MIn, int MIn_maxidx, long double* MOut, int MOut_maxidx, Char*& ResultMessage) { /*ASPT_ProtocolByPoints*/   retCode =-1;    if (hModule != nullptr)retCode = ();qDebug() << "" << retCode;return retCode;}
//int ASPT_CLASS::ASPT_ProtocolByPointsKTPR(uint8_t PointCount, uint8_t DeflectionClass1, uint8_t DeflectionClass2, TNSH NSH, bool Percent, long double R, long double X0, long double X1, long double Tmin, long double Tmax, long double dTmin, long double* MIn, int MIn_maxidx, long double* MOut, int MOut_maxidx, long double* Dev, int Dev_maxidx, Char*& ResultMessage) { /*ASPT_ProtocolByPointsKTPR*/   retCode =-1;    if (hModule != nullptr)retCode = ();qDebug() << "" << retCode;return retCode;}

long AsptClass::RetCode() const
{
    return retCode;
}

double AsptClass::DtoR48(double d)
{
    uint64_t r = 0, tmp = 0;

    tmp = *(uint64_t*)&d; //e
    tmp >>= 52; // tmp /= 0x10000000000000;
    tmp -= 894;
    tmp &= 0xFF;
    if (tmp == 0)
        return 0.0;
    r |= tmp;

    tmp = *(uint64_t*)&d; //m
    tmp >>= 5; // tmp /= 0x20;
    tmp &= 0x7FFFFFFFFF00;
    r |= tmp;

    tmp = *(uint64_t*)&d; //s
    tmp >>= 16; // tmp /= 0x10000;
    tmp &= 0x800000000000;
    r |= tmp;

    return *(double*)&r;
}

double AsptClass::R48toD(double r)
{
    // Convert Turbo Pascal REAL (aka delphi double) data type to double
    // TurboPascal real data type (6 bytes, 2.9 x 10-39 to 1.7 x 1038)
    //
    //    1                   39                       8
    //   +-+---------------------------------------+--------+
    //   |s|                  f                    |   e    |
    //   +-+---------------------------------------+--------+
    //   e = 0 -> v = 0
    //   exponent bias = 129

    uint64_t d = 0, tmp = 0;

    tmp = *(uint64_t*)&r; //e
    tmp &= 0xFF;
    if (tmp == 0)
        return 0.0;

    tmp += 894;
    tmp <<= 52; // tmp *= 0x10000000000000;
    d |= tmp;

    tmp = *(uint64_t*)&r; //m
    tmp &= 0x7FFFFFFFFF00;
    tmp <<= 5; // tmp *= 0x20;
    d |= tmp;

    tmp = *(uint64_t*)&r; //s
    tmp &= 0x800000000000;
    tmp <<= 16; // tmp *= 0x10000;
    d |= tmp;

    return *(double*)&d;
}
