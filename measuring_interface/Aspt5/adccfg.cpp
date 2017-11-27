#include "adccfg.h"

#include <QSet>
//------------------------------------------------------------------------------
//QSet<int /*, 0, 255*/> adccfg_u__0({ 0 });
//QSet<int /*, 0, 255*/> adccfg_u__1({ 1, 1, 1, 1, 1, 1 });
//QSet<int /*, 0, 255*/> adccfg_u__2({ 0, 0, 0, 0, 0, 0 });
//QSet<int /*, 0, 255*/> adccfg_u__16({ 0, 1, 2, 3, 4, 5, 6, 7 });
//QSet<int /*, 0, 255*/> adccfg_u__18({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
//QSet<int /*, 0, 255*/> adccfg_u__19({ 0, 1, 2, 3 });
//QSet<int /*, 0, 255*/> adccfg_u__21({ 0, 1, 2, 3, 4, 5, 6 });
//QSet<int /*, 0, 255*/> adccfg_u__22({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254 });
//------------------------------------------------------------------------------

const QString AdcCfg::PackClearADCCfgFull(QStringLiteral("|1|1|1|1|1|1|6|6|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|6|0|"));
const QString AdcCfg::PackClearADCCfgLow(QStringLiteral("|255|255|255|255|255|255|6|6|255|0|255|255|255|255|0|0|0|255|0|255|255|255|255|0|0|0|255|0|255|255|255|255|0|0|0|255|0|255|255|255|255|0|0|0|255|0|255|255|255|255|0|0|0|255|0|255|255|255|255|0|0|0|6|0|"));
const QString AdcCfg::PackClearADCCfgMedium(QStringLiteral("|255|255|255|255|255|255|6|6|255|0|255|255|255|255|0|0|0|255|0|255|255|255|255|0|0|0|255|0|255|255|255|255|0|0|0|255|0|255|255|255|255|0|0|0|255|0|255|255|255|255|0|0|0|255|0|255|255|255|255|0|0|0|6|0|"));
const QString AdcCfg::PackClearADCCfgSpecial1(QStringLiteral("|255|255|255|255|255|255|6|6|255|0|255|255|255|255|0|0|0|255|0|255|255|255|255|0|0|0|255|0|255|255|255|255|0|0|0|255|0|255|255|255|255|0|0|0|255|0|255|255|255|255|0|0|0|255|0|255|255|255|255|0|0|0|6|0|"));
const QString AdcCfg::PackCorrection(QStringLiteral("|1|1|1|0|0|0|3|6|31|27|0|1|0|2|8|4|8|31|27|0|1|0|3|8|4|8|31|27|0|1|0|5|8|4|8|31|27|0|1|0|0|8|4|8|31|27|0|1|0|0|8|4|8|31|27|0|1|0|0|8|4|8|234|1|"));
const QString AdcCfg::PackCurrent(QStringLiteral("|4|1|1|1|1|1|3|5|3|0|0|0|10|5|10|0|0|1|1|1|3|5|3|0|0|6|10|5|10|0|0|1|1|1|3|5|3|0|8|0|10|5|10|0|0|1|1|1|3|5|3|0|8|6|10|5|10|0|0|1|1|1|3|7|3|0|0|0|1|1|1|0|0|1|1|1|3|7|3|0|0|0|1|0|1|"));
const QString AdcCfg::PackResistor(QStringLiteral("|4|1|1|0|1|1|3|3|0|1|3|0|20|4|8|0|0|1|1|1|3|3|0|1|3|6|20|4|8|0|0|1|1|1|3|3|0|1|11|0|20|4|8|0|0|1|1|1|3|3|0|1|11|6|20|4|8|1|1|1|1|1|0|0|0|1|0|0|0|0|1|1|1|1|1|1|0|0|0|1|0|0|0|0|1|"));
const QString AdcCfg::PackVoltage(QStringLiteral("|3|1|1|1|1|0|3|5|2|0|3|0|10|5|10|0|0|1|1|0|3|5|2|0|3|4|10|5|10|0|0|1|1|0|3|5|2|0|11|5|10|5|10|0|0|1|1|0|3|5|2|0|11|6|10|5|10|0|0|1|1|0|3|7|2|0|0|0|1|1|1|0|0|1|1|0|3|7|2|0|0|0|1|1|0|"));

AdcCfg::AdcCfg(const QString& Pack)
    : m_err(0)
    , _MeasureStepCount(MaxStepCount)
    , bDataSize(sizeof(MeasureStep[0]) - 1)
{
    //inherited Create;
    ClearSection();
    InitSection();
    SetMeasureStepCount(MaxStepCount);
    if (!Pack.isEmpty())
        SetPack(Pack);
}
//------------------------------------------------------------------------------
AdcCfg::~AdcCfg()
{
    //todo check : inherited::Destroy;
}
//------------------------------------------------------------------------------
int AdcCfg::getErr() const
{
    return m_err;
}

quint8 AdcCfg::GetMeasureStepCount()
{
    quint8 SC = 0;
    for (int stop = MaxStepCount, i = 0; i < stop; ++i)
        if (UseMeasureStep[i])
            SC++;
    return SC;
}
//------------------------------------------------------------------------------
void AdcCfg::SetMeasureStepCount(quint8 SC)
{
    for (int stop = MaxStepCount, i = 0; i < stop; ++i)
        UseMeasureStep[i] = false;
    const QSet<int> check({ 0, 1, 2, 3, 4, 5, 6 });
    if (check.contains(SC))
        for (int stop = SC - 1, i = 0; i <= stop; i++)
            UseMeasureStep[i] = true;
}
//------------------------------------------------------------------------------
int AdcCfg::ClearSection()
{
    for (int stop = MaxStepCount - 1, j = 0; j <= stop; j++) {
        memset(reinterpret_cast<quint8*>(&MeasureStep[j]), 0, bDataSize);
    }
    return (m_err = ASPT_OK);
}
//------------------------------------------------------------------------------
int AdcCfg::InitSection()
{
    do {
        if (SetUseMeasureStep({ 1, 1, 1, 1, 1, 1 }) != ASPT_OK)
            break;
        if (SetUnsetADC({ 0, 0, 0, 0, 0, 0 }) != ASPT_OK)
            break;
        if (SetAutoCalibrationADC({ 0, 0, 0, 0, 0, 0 }) != ASPT_OK)
            break;
        if (SetUnsetFilter({ 0, 0, 0, 0, 0, 0 }) != ASPT_OK)
            break;
        if (SetInputBuffer({ 0, 0, 0, 0, 0, 0 }) != ASPT_OK)
            break;
        if (SetStateCurrent({ 0, 0, 0, 0, 0, 0 }) != ASPT_OK)
            break;
        if (SetGainFactor({ 0, 0, 0, 0, 0, 0 }) != ASPT_OK)
            break;
        if (SetFilterFrequency({ 0, 0, 0, 0, 0, 0 }) != ASPT_OK)
            break;
        if (SetMeasureChannel({ 0, 0, 0, 0, 0, 0 }) != ASPT_OK)
            break;
        if (SetSupportingResistor({ 0, 0, 0, 0, 0, 0 }) != ASPT_OK)
            break;
        if (SetValuePolarityCurrent({ 0, 0, 0, 0, 0, 0 }) != ASPT_OK)
            break;
        if (SetVoltageChannel({ 0, 0, 0, 0, 0, 0 }) != ASPT_OK)
            break;
        if (SetPauseBeforeMeasure({ 1, 1, 1, 1, 1, 1 }) != ASPT_OK)
            break;
        if (SetAmountMeasurements({ 1, 1, 1, 1, 1, 1 }) != ASPT_OK)
            break;
        if (SetAmountAverage({ 1, 1, 1, 1, 1, 1 }) != ASPT_OK)
            break;
    } while (0);
    return m_err;
}
//------------------------------------------------------------------------------
int AdcCfg::ConvertSectionToParcel(QVector<quint8>& B /*, quint16& BS*/)
{
    quint16 BS = 0;
    B.fill(0, MaxParcelLength);
    B[BS++] = GetMeasureStepCount();
    for (int stop = MaxStepCount, i = 0; i < stop; ++i) {
        if (UseMeasureStep[i]) {
            TbData with = MeasureStep[i];
            {
                B[BS++] = static_cast<quint8>(with.UnsetADC | (with.AutoCalibrationADC << 1) | (with.UnsetFilter << 2) | (with.InputBuffer << 3) | (with.StateCurrent << 4));
                B[BS++] = static_cast<quint8>(with.GainFactor | with.FilterFrequency << 3);
                B[BS++] = with.MeasureChannel;
                B[BS++] = with.SupportingResistorNumber;
                B[BS++] = with.ValuePolarityCurrent;
                B[BS++] = with.VoltageChannel;
                B[BS++] = with.PauseBeforeMeasure;
                B[BS++] = with.AmountMeasurements;
                B[BS++] = with.AmountAverage;
            }
        }
    }
    BS += 2;
    B.resize(BS);
    m_err = ControlAmount(B);
    if (m_err != ASPT_OK)
        return m_err;
    return (m_err = ASPT_OK);
}
//------------------------------------------------------------------------------
int AdcCfg::ConvertSectionFromParcel(QVector<quint8>& B)
{
    quint8 SC = 0;
    quint16 BS = 0;

    if (B.size() < 3)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    m_err = CheckControlAmount(B);
    if (m_err != ASPT_OK)
        return m_err;

    SC = B[BS++];
    if ((SC > MaxStepCount) || (SC != GetMeasureStepCount()))
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = MaxStepCount, i = 0; i < stop; i++) {
        if (UseMeasureStep[i]) {
            TbData& with0 = MeasureStep[i];
            {
                with0.UnsetADC = (B[BS] & 0x01) >> 0;
                with0.AutoCalibrationADC = (B[BS] & 0x02) >> 1;
                with0.UnsetFilter = (B[BS] & 0x04) >> 2;
                with0.InputBuffer = (B[BS] & 0x08) >> 3;
                with0.StateCurrent = (B[BS++] & 0x10) >> 4;

                with0.GainFactor = (B[BS] & 0x07) >> 0;
                with0.FilterFrequency = (B[BS++] & 0x38) >> 3;

                with0.MeasureChannel = B[BS++];
                with0.SupportingResistorNumber = B[BS++];
                with0.ValuePolarityCurrent = B[BS++];
                with0.VoltageChannel = B[BS++];
                with0.PauseBeforeMeasure = B[BS++];
                with0.AmountMeasurements = B[BS++];
                with0.AmountAverage = B[BS++];
            }
        }
    }
    return (m_err = ASPT_OK);
}
//------------------------------------------------------------------------------
int AdcCfg::SetUseMeasureStep(const QVector<quint8>& P /*, int P_maxidx*/)
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        UseMeasureStep[i] = static_cast<bool>(P[i]);
    return (m_err = ASPT_OK);
}
//------------------------------------------------------------------------------
int AdcCfg::GetUseMeasureStep(QVector<quint8>& P /*, int P_maxidx*/)
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = UseMeasureStep[i];
    return (m_err = ASPT_OK);
}
//------------------------------------------------------------------------------
int AdcCfg::SetUnsetADC(const QVector<quint8>& P /*, int P_maxidx*/)
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        if (P[i] != IgnoreValue)
            MeasureStep[i].UnsetADC = static_cast<bool>(P[i]);
    return (m_err = ASPT_OK);
}
//------------------------------------------------------------------------------
int AdcCfg::GetUnsetADC(QVector<quint8>& P /*, int P_maxidx*/)
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].UnsetADC;
    return (m_err = ASPT_OK);
}
//------------------------------------------------------------------------------
int AdcCfg::SetAutoCalibrationADC(const QVector<quint8>& P /*, int P_maxidx*/)
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        if (P[i] != IgnoreValue)
            MeasureStep[i].AutoCalibrationADC = static_cast<bool>(P[i]);
    return (m_err = ASPT_OK);
}
//------------------------------------------------------------------------------
int AdcCfg::GetAutoCalibrationADC(QVector<quint8>& P /*, int P_maxidx*/)
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].AutoCalibrationADC;
    return (m_err = ASPT_OK);
}
//------------------------------------------------------------------------------
int AdcCfg::SetUnsetFilter(const QVector<quint8>& P /*, int P_maxidx*/)
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        if (P[i] != IgnoreValue)
            MeasureStep[i].UnsetFilter = static_cast<bool>(P[i]);
    return (m_err = ASPT_OK);
}
//------------------------------------------------------------------------------
int AdcCfg::GetUnsetFilter(QVector<quint8>& P /*, int P_maxidx*/)
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].UnsetFilter;
    return (m_err = ASPT_OK);
}
//------------------------------------------------------------------------------
int AdcCfg::SetInputBuffer(const QVector<quint8>& P /*, int P_maxidx*/)
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        if (P[i] != IgnoreValue)
            MeasureStep[i].InputBuffer = static_cast<bool>(P[i]);
    return (m_err = ASPT_OK);
}
//------------------------------------------------------------------------------
int AdcCfg::GetInputBuffer(QVector<quint8>& P /*, int P_maxidx*/)
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].InputBuffer;
    return (m_err = ASPT_OK);
}
//------------------------------------------------------------------------------
int AdcCfg::SetStateCurrent(const QVector<quint8>& P /*, int P_maxidx*/)
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        if (P[i] != IgnoreValue)
            MeasureStep[i].StateCurrent = static_cast<bool>(P[i]);
    return (m_err = ASPT_OK);
}
//------------------------------------------------------------------------------
int AdcCfg::GetStateCurrent(QVector<quint8>& P /*, int P_maxidx*/)
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].StateCurrent;
    return (m_err = ASPT_OK);
}
//------------------------------------------------------------------------------
int AdcCfg::SetGainFactor(const QVector<quint8>& P /*, int P_maxidx*/)
{
    const QSet<int> check({ 0, 1, 2, 3, 4, 5, 6, 7 });
    if (sizeof(P) > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = sizeof(P) - 1, i = 0; i <= stop; i++)
        if ((P[i] != IgnoreValue) && check.contains(P[i]))
            MeasureStep[i].GainFactor = P[i];
    return (m_err = ASPT_OK);
}
//------------------------------------------------------------------------------
int AdcCfg::GetGainFactor(QVector<quint8>& P /*, int P_maxidx*/)
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].GainFactor;
    return (m_err = ASPT_OK);
}
//------------------------------------------------------------------------------
int AdcCfg::SetFilterFrequency(const QVector<quint8>& P /*, int P_maxidx*/)
{
    const QSet<int> check({ 0, 1, 2, 3, 4, 5, 6, 7 });
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        if ((P[i] != IgnoreValue) && check.contains(P[i]))
            MeasureStep[i].FilterFrequency = P[i];
    return (m_err = ASPT_OK);
}
//------------------------------------------------------------------------------
int AdcCfg::GetFilterFrequency(QVector<quint8>& P /*, int P_maxidx*/)
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].FilterFrequency;
    return (m_err = ASPT_OK);
}
//------------------------------------------------------------------------------
int AdcCfg::SetMeasureChannel(const QVector<quint8>& P /*, int P_maxidx*/)
{
    const QSet<int> check({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);

    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        if ((P[i] != IgnoreValue) && check.contains(P[i]))
            MeasureStep[i].MeasureChannel = P[i];
    return (m_err = ASPT_OK);
}
//------------------------------------------------------------------------------
int AdcCfg::GetMeasureChannel(QVector<quint8>& P /*, int P_maxidx*/)
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].MeasureChannel;
    return (m_err = ASPT_OK);
}
//------------------------------------------------------------------------------
int AdcCfg::SetSupportingResistor(const QVector<quint8>& P /*, int P_maxidx*/)
{
    const QSet<int> check({ 0, 1, 2, 3 });
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        if ((P[i] != IgnoreValue) && check.contains(P[i]))
            MeasureStep[i].SupportingResistorNumber = P[i];
    return (m_err = ASPT_OK);
}
//------------------------------------------------------------------------------
int AdcCfg::GetSupportingResistor(QVector<quint8>& P /*, int P_maxidx*/)
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].SupportingResistorNumber;
    return (m_err = ASPT_OK);
}
//------------------------------------------------------------------------------
int AdcCfg::SetValuePolarityCurrent(const QVector<quint8>& P /*, int P_maxidx*/)
{
    const QSet<int> check({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        if ((P[i] != IgnoreValue) && check.contains(P[i])) {
            quint8& ch = MeasureStep[i].ValuePolarityCurrent;
            ch = (ch & 0xF8) | P[i];
        }
    return (m_err = ASPT_OK);
}
//------------------------------------------------------------------------------
int AdcCfg::GetValuePolarityCurrent(QVector<quint8>& P /*, int P_maxidx*/)
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].ValuePolarityCurrent;
    return (m_err = ASPT_OK);
}
//------------------------------------------------------------------------------
int AdcCfg::SetVoltageChannel(const QVector<quint8>& P /*, int P_maxidx*/)
{
    const QSet<int> check({ 0, 1, 2, 3, 4, 5, 6 });
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        if ((P[i] != IgnoreValue) && check.contains(P[i]))
            MeasureStep[i].VoltageChannel = P[i];
    return (m_err = ASPT_OK);
}
//------------------------------------------------------------------------------
int AdcCfg::GetVoltageChannel(QVector<quint8>& P /*, int P_maxidx*/)
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].VoltageChannel;
    return (m_err = ASPT_OK);
}
//------------------------------------------------------------------------------
int AdcCfg::SetPauseBeforeMeasure(const QVector<quint8>& P /*, int P_maxidx*/)
{
    const QSet<int> check({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254 });
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        if ((P[i] != IgnoreValue) && check.contains(P[i]))
            MeasureStep[i].PauseBeforeMeasure = P[i];
    return (m_err = ASPT_OK);
}
//------------------------------------------------------------------------------
int AdcCfg::GetPauseBeforeMeasure(QVector<quint8>& P /*, int P_maxidx*/)
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].PauseBeforeMeasure;
    return (m_err = ASPT_OK);
}
//------------------------------------------------------------------------------
int AdcCfg::SetAmountMeasurements(const QVector<quint8>& P /*, int P_maxidx*/)
{
    const QSet<int> check({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254 });
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        if ((P[i] != IgnoreValue) && check.contains(P[i]))
            MeasureStep[i].AmountMeasurements = P[i];
    return (m_err = ASPT_OK);
}
//------------------------------------------------------------------------------
int AdcCfg::GetAmountMeasurements(QVector<quint8>& P /*, int P_maxidx*/)
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].AmountMeasurements;
    return (m_err = ASPT_OK);
}
//------------------------------------------------------------------------------
int AdcCfg::SetAmountAverage(const QVector<quint8>& P /*, int P_maxidx*/)
{
    const QSet<int> check({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254 });
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        if ((P[i] != IgnoreValue) && check.contains(P[i]))
            MeasureStep[i].AmountAverage = P[i];
    return (m_err = ASPT_OK);
}
//------------------------------------------------------------------------------
int AdcCfg::GetAmountAverage(QVector<quint8>& P /*, int P_maxidx*/)
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);

    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].AmountAverage;
    return (m_err = ASPT_OK);
}
//------------------------------------------------------------------------------
QString AdcCfg::GetPack()
{
    QVector<quint8> B;
    QString S('|');
    bool M[6]; /*# range 0..MaxStepCount-1*/
    for (int stop = MaxStepCount, i = 0; i < stop; i++) {
        M[i] = UseMeasureStep[i];
        UseMeasureStep[i] = true;
    }
    m_err = ConvertSectionToParcel(B);
    if (m_err != ASPT_OK)
        return "";
    for (int stop = MaxStepCount, i = 0; i < stop; ++i)
        UseMeasureStep[i] = M[i];

    for (int stop = MaxStepCount, i = 0; i < stop; ++i)
        S += QString().setNum(UseMeasureStep[i]) + '|';
    S += QString().setNum(GetMeasureStepCount()) + '|';
    for (int stop = B.size() - 1, i = 0; i <= stop; i++) {
        S += QString().setNum(B[i]) + '|';
    }
    return S;
}
//------------------------------------------------------------------------------
int AdcCfg::SetPack(const QString& P)
{
    bool ok = false;
    bool M[6]{ false, false, false, false, false, false }; /*# range 0..MaxStepCount-1*/
    bool MaskWas = false;
    int iNew = 0, iOld = 0;
    QString sNew(P), sOld(GetPack());
    quint16 BS = 0;
    quint8 byte = 0;

    iNew = sNew.indexOf('|');
    iOld = sOld.indexOf('|');

    if (!sNew.startsWith('|') || !sOld.startsWith('|'))
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);

    sNew.remove(iNew, 1);
    sOld.remove(iOld, 1);

    QString mid;
    for (int stop = MaxStepCount - 1, k = 0; k <= stop; k++) {
        iNew = sNew.indexOf('|');
        iOld = sOld.indexOf('|');

        if (iNew < 0 || iOld < 0)
            return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);

        mid = sNew.mid(0, iNew);
        if (mid == QString().setNum(IgnoreValue)) {
            mid = sOld.mid(0, iOld);
            MaskWas = true;
        }

        byte = static_cast<quint8>(mid.toInt(&ok));
        if (!ok)
            return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
        UseMeasureStep[k] = static_cast<bool>(byte);

        sNew.remove(0, iNew + 1);
        sOld.remove(0, iOld + 1);
    }

    iNew = sNew.indexOf('|');
    iOld = sOld.indexOf('|');

    if (iNew < 0 || iOld < 0)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);

    mid = sNew.mid(0, iNew);
    byte = static_cast<quint8>(mid.toInt(&ok));
    if (!ok)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    //MeasureStepCount = byte;

    sNew.remove(0, iNew + 1);
    sOld.remove(0, iOld + 1);

    QVector<quint8> B(MaxParcelLength, 0);
    while ((sNew.length() > 1) && (sOld.length() > 1)) {
        iNew = sNew.indexOf('|');
        iOld = sOld.indexOf('|');

        if (iNew < 0 || iOld < 0)
            return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);

        mid = sNew.mid(0, iNew);
        if (mid == QString().setNum(IgnoreValue)) {
            mid = sOld.mid(0, iOld);
            MaskWas = true;
        }

        byte = static_cast<quint8>(mid.toInt(&ok));
        if (!ok)
            return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
        B[BS++] = byte;

        sNew.remove(0, iNew + 1);
        sOld.remove(0, iOld + 1);
    }
    B.resize(BS);
    for (int stop = MaxStepCount, i = 0; i < stop; i++) {
        M[i] = UseMeasureStep[i];
        UseMeasureStep[i] = true;
    }
    if (MaskWas == true) {
        m_err = ControlAmount(B);
        if (m_err != ASPT_OK)
            return m_err;
    }
    m_err = ConvertSectionFromParcel(B);
    if (m_err != ASPT_OK)
        return m_err;
    for (int stop = MaxStepCount, i = 0; i < stop; ++i)
        UseMeasureStep[i] = M[i];
    return (m_err = ASPT_OK);
}
//------------------------------------------------------------------------------
