#include "adccfg.h"

#include <QSet>

const QString AdcCfg::PackClearADCCfgFull(QStringLiteral("|1|1|1|1|1|1|6|6|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|6|0|"));
const QString AdcCfg::PackClearADCCfgLow(QStringLiteral("|255|255|255|255|255|255|6|6|255|0|255|255|255|255|0|0|0|255|0|255|255|255|255|0|0|0|255|0|255|255|255|255|0|0|0|255|0|255|255|255|255|0|0|0|255|0|255|255|255|255|0|0|0|255|0|255|255|255|255|0|0|0|6|0|"));
const QString AdcCfg::PackClearADCCfgMedium(QStringLiteral("|255|255|255|255|255|255|6|6|255|0|255|255|255|255|0|0|0|255|0|255|255|255|255|0|0|0|255|0|255|255|255|255|0|0|0|255|0|255|255|255|255|0|0|0|255|0|255|255|255|255|0|0|0|255|0|255|255|255|255|0|0|0|6|0|"));
const QString AdcCfg::PackClearADCCfgSpecial1(QStringLiteral("|255|255|255|255|255|255|6|6|255|0|255|255|255|255|0|0|0|255|0|255|255|255|255|0|0|0|255|0|255|255|255|255|0|0|0|255|0|255|255|255|255|0|0|0|255|0|255|255|255|255|0|0|0|255|0|255|255|255|255|0|0|0|6|0|"));
const QString AdcCfg::PackCorrection(QStringLiteral("|1|1|1|0|0|0|3|6|31|27|0|1|0|2|8|4|8|31|27|0|1|0|3|8|4|8|31|27|0|1|0|5|8|4|8|31|27|0|1|0|0|8|4|8|31|27|0|1|0|0|8|4|8|31|27|0|1|0|0|8|4|8|234|1|"));
const QString AdcCfg::PackCurrent(QStringLiteral("|4|1|1|1|1|1|3|5|3|0|0|0|10|5|10|0|0|1|1|1|3|5|3|0|0|6|10|5|10|0|0|1|1|1|3|5|3|0|8|0|10|5|10|0|0|1|1|1|3|5|3|0|8|6|10|5|10|0|0|1|1|1|3|7|3|0|0|0|1|1|1|0|0|1|1|1|3|7|3|0|0|0|1|0|1|"));
const QString AdcCfg::PackResistor(QStringLiteral("|4|1|1|0|1|1|3|3|0|1|3|0|20|4|8|0|0|1|1|1|3|3|0|1|3|6|20|4|8|0|0|1|1|1|3|3|0|1|11|0|20|4|8|0|0|1|1|1|3|3|0|1|11|6|20|4|8|1|1|1|1|1|0|0|0|1|0|0|0|0|1|1|1|1|1|1|0|0|0|1|0|0|0|0|1|"));
const QString AdcCfg::PackVoltage(QStringLiteral("|3|1|1|1|1|0|3|5|2|0|3|0|10|5|10|0|0|1|1|0|3|5|2|0|3|4|10|5|10|0|0|1|1|0|3|5|2|0|11|5|10|5|10|0|0|1|1|0|3|5|2|0|11|6|10|5|10|0|0|1|1|0|3|7|2|0|0|0|1|1|1|0|0|1|1|0|3|7|2|0|0|0|1|1|0|"));

AdcCfg::AdcCfg(const QString& Pack)
{
    m_err = 0;
    //inherited Create;
    clear();
    init();
    setMeasureStepCount(MaxStepCount);
    if (!Pack.isEmpty())
        setPack(Pack);
}
/////////////////////////////////////////////////////////
AdcCfg::~AdcCfg()
{
    //todo check : inherited::Destroy;
}
/////////////////////////////////////////////////////////
int AdcCfg::getErr() const
{
    return m_err;
}

quint8 AdcCfg::measureStepCount()
{
    quint8 SC = 0;
    for (int stop = MaxStepCount, i = 0; i < stop; ++i)
        if (UseMeasureStep[i])
            SC++;
    return SC;
}
/////////////////////////////////////////////////////////
void AdcCfg::setMeasureStepCount(quint8 SC)
{
    for (int stop = MaxStepCount, i = 0; i < stop; ++i)
        UseMeasureStep[i] = false;
    const QSet<int> check({ 0, 1, 2, 3, 4, 5, 6 });
    if (check.contains(SC))
        for (int stop = SC - 1, i = 0; i <= stop; i++)
            UseMeasureStep[i] = true;
}
/////////////////////////////////////////////////////////
int AdcCfg::clear()
{
    for (int stop = MaxStepCount - 1, j = 0; j <= stop; j++) {
        memset(&MeasureStep[j], 0, sizeof(StepData));
    }
    return (m_err = ASPT_OK);
}
/////////////////////////////////////////////////////////
int AdcCfg::init()
{
    do {
        if (setUseMeasureStep({ 1, 1, 1, 1, 1, 1 }) != ASPT_OK)
            break;
        if (setUnsetADC({ 0, 0, 0, 0, 0, 0 }) != ASPT_OK)
            break;
        if (setAutoCalibrationADC({ 0, 0, 0, 0, 0, 0 }) != ASPT_OK)
            break;
        if (setUnsetFilter({ 0, 0, 0, 0, 0, 0 }) != ASPT_OK)
            break;
        if (setInputBuffer({ 0, 0, 0, 0, 0, 0 }) != ASPT_OK)
            break;
        if (setStateCurrent({ 0, 0, 0, 0, 0, 0 }) != ASPT_OK)
            break;
        if (setGainFactor({ 0, 0, 0, 0, 0, 0 }) != ASPT_OK)
            break;
        if (setFilterFrequency({ 0, 0, 0, 0, 0, 0 }) != ASPT_OK)
            break;
        if (setMeasureChannel({ 0, 0, 0, 0, 0, 0 }) != ASPT_OK)
            break;
        if (setSupportingResistor({ 0, 0, 0, 0, 0, 0 }) != ASPT_OK)
            break;
        if (setValuePolarityCurrent({ 0, 0, 0, 0, 0, 0 }) != ASPT_OK)
            break;
        if (setVoltageChannel({ 0, 0, 0, 0, 0, 0 }) != ASPT_OK)
            break;
        if (setPauseBeforeMeasure({ 1, 1, 1, 1, 1, 1 }) != ASPT_OK)
            break;
        if (setAmountMeasurements({ 1, 1, 1, 1, 1, 1 }) != ASPT_OK)
            break;
        if (setAmountAverage({ 1, 1, 1, 1, 1, 1 }) != ASPT_OK)
            break;
    } while (0);
    return m_err;
}
/////////////////////////////////////////////////////////
int AdcCfg::convertSectionToParcel(QVector<quint8>& B /*, quint16& BS*/)
{
    quint16 BS = 0;
    B.fill(0, MaxParcelLength);
    B[BS++] = measureStepCount();
    for (int stop = MaxStepCount, i = 0; i < stop; ++i) {
        if (UseMeasureStep[i]) {
            StepData with = MeasureStep[i];
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
/////////////////////////////////////////////////////////
int AdcCfg::convertSectionFromParcel(QVector<quint8>& B)
{
    quint8 SC = 0;
    quint16 BS = 0;

    if (B.size() < 3)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    m_err = CheckControlAmount(B);
    if (m_err != ASPT_OK)
        return m_err;

    SC = B[BS++];
    if ((SC > MaxStepCount) || (SC != measureStepCount()))
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = MaxStepCount, i = 0; i < stop; i++) {
        if (UseMeasureStep[i]) {
            StepData& with0 = MeasureStep[i];
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
/////////////////////////////////////////////////////////
/// \brief AdcCfg::setUseMeasureStep
/// \param P
/// \return
///
int AdcCfg::setUseMeasureStep(const QVector<quint8>& P)
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        UseMeasureStep[i] = static_cast<bool>(P[i]);
    return (m_err = ASPT_OK);
}
int AdcCfg::useMeasureStep(QVector<quint8>& P) const
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = UseMeasureStep[i];
    return (m_err = ASPT_OK);
}
QVector<quint8> AdcCfg::useMeasureStep() const
{
    QVector<quint8> P(6, 0);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = UseMeasureStep[i];
    return P;
}
/////////////////////////////////////////////////////////
/// \brief AdcCfg::setUnsetADC
/// \param P
/// \return
///
int AdcCfg::setUnsetADC(const QVector<quint8>& P)
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        if (P[i] != IgnoreValue)
            MeasureStep[i].UnsetADC = static_cast<bool>(P[i]);
    return (m_err = ASPT_OK);
}
//QVector<quint8> AdcCfg::unsetADC() const
//{
//    if (P.size() > MaxStepCount)
//        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
//    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
//        P[i] = MeasureStep[i].UnsetADC;
//    return (m_err = ASPT_OK);
//}
QVector<quint8> AdcCfg::unsetADC() const
{
    QVector<quint8> P(6, 0);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].UnsetADC;
    return P;
}

/////////////////////////////////////////////////////////
/// \brief AdcCfg::setAutoCalibrationADC
/// \param P
/// \return
///
int AdcCfg::setAutoCalibrationADC(const QVector<quint8>& P)
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        if (P[i] != IgnoreValue)
            MeasureStep[i].AutoCalibrationADC = static_cast<bool>(P[i]);
    return (m_err = ASPT_OK);
}
int AdcCfg::autoCalibrationADC(QVector<quint8>& P) const
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].AutoCalibrationADC;
    return (m_err = ASPT_OK);
}

QVector<quint8> AdcCfg::autoCalibrationADC() const
{
    QVector<quint8> P(6, 0);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].AutoCalibrationADC;
    return P;
}
/////////////////////////////////////////////////////////
/// \brief AdcCfg::setUnsetFilter
/// \param P
/// \return
///
int AdcCfg::setUnsetFilter(const QVector<quint8>& P)
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        if (P[i] != IgnoreValue)
            MeasureStep[i].UnsetFilter = static_cast<bool>(P[i]);
    return (m_err = ASPT_OK);
}
int AdcCfg::unsetFilter(QVector<quint8>& P) const
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].UnsetFilter;
    return (m_err = ASPT_OK);
}
QVector<quint8> AdcCfg::unsetFilter() const
{
    QVector<quint8> P(6, 0);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].UnsetFilter;
    return P;
}
/////////////////////////////////////////////////////////
/// \brief AdcCfg::setInputBuffer
/// \param P
/// \return
///
int AdcCfg::setInputBuffer(const QVector<quint8>& P)
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        if (P[i] != IgnoreValue)
            MeasureStep[i].InputBuffer = static_cast<bool>(P[i]);
    return (m_err = ASPT_OK);
}
int AdcCfg::inputBuffer(QVector<quint8>& P) const
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].InputBuffer;
    return (m_err = ASPT_OK);
}
QVector<quint8> AdcCfg::inputBuffer() const
{
    QVector<quint8> P(6, 0);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].InputBuffer;
    return P;
}
/////////////////////////////////////////////////////////
/// \brief AdcCfg::setStateCurrent
/// \param P
/// \return
///
int AdcCfg::setStateCurrent(const QVector<quint8>& P)
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        if (P[i] != IgnoreValue)
            MeasureStep[i].StateCurrent = static_cast<bool>(P[i]);
    return (m_err = ASPT_OK);
}
int AdcCfg::stateCurrent(QVector<quint8>& P)
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].StateCurrent;
    return (m_err = ASPT_OK);
}
QVector<quint8> AdcCfg::stateCurrent() const
{
    QVector<quint8> P(6, 0);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].StateCurrent;
    return P;
}
/////////////////////////////////////////////////////////
/// \brief AdcCfg::setGainFactor
/// \param P
/// \return
///
int AdcCfg::setGainFactor(const QVector<quint8>& P)
{
    const QSet<int> check({ 0, 1, 2, 3, 4, 5, 6, 7 });
    if (sizeof(P) > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = sizeof(P) - 1, i = 0; i <= stop; i++)
        if ((P[i] != IgnoreValue) && check.contains(P[i]))
            MeasureStep[i].GainFactor = P[i];
    return (m_err = ASPT_OK);
}
int AdcCfg::gainFactor(QVector<quint8>& P) const
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].GainFactor;
    return (m_err = ASPT_OK);
}
QVector<quint8> AdcCfg::gainFactor() const
{
    QVector<quint8> P(6, 0);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].GainFactor;
    return P;
}
/////////////////////////////////////////////////////////
/// \brief AdcCfg::setFilterFrequency
/// \param P
/// \return
///
int AdcCfg::setFilterFrequency(const QVector<quint8>& P)
{
    const QSet<int> check({ 0, 1, 2, 3, 4, 5, 6, 7 });
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        if ((P[i] != IgnoreValue) && check.contains(P[i]))
            MeasureStep[i].FilterFrequency = P[i];
    return (m_err = ASPT_OK);
}
int AdcCfg::filterFrequency(QVector<quint8>& P) const
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].FilterFrequency;
    return (m_err = ASPT_OK);
}
QVector<quint8> AdcCfg::filterFrequency() const
{
    QVector<quint8> P(6, 0);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].FilterFrequency;
    return P;
}
/////////////////////////////////////////////////////////
/// \brief AdcCfg::setMeasureChannel
/// \param P
/// \return
///
int AdcCfg::setMeasureChannel(const QVector<quint8>& P)
{
    const QSet<int> check({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);

    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        if ((P[i] != IgnoreValue) && check.contains(P[i]))
            MeasureStep[i].MeasureChannel = P[i];
    return (m_err = ASPT_OK);
}
int AdcCfg::measureChannel(QVector<quint8>& P) const
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].MeasureChannel;
    return (m_err = ASPT_OK);
}
QVector<quint8> AdcCfg::measureChannel() const
{
    QVector<quint8> P(6, 0);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].MeasureChannel;
    return P;
}
/////////////////////////////////////////////////////////
/// \brief AdcCfg::setSupportingResistor
/// \param P
/// \return
///
int AdcCfg::setSupportingResistor(const QVector<quint8>& P)
{
    const QSet<int> check({ 0, 1, 2, 3 });
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        if ((P[i] != IgnoreValue) && check.contains(P[i]))
            MeasureStep[i].SupportingResistorNumber = P[i];
    return (m_err = ASPT_OK);
}
int AdcCfg::supportingResistor(QVector<quint8>& P)
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].SupportingResistorNumber;
    return (m_err = ASPT_OK);
}
QVector<quint8> AdcCfg::supportingResistor() const
{
    QVector<quint8> P(6, 0);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].SupportingResistorNumber;
    return P;
}
/////////////////////////////////////////////////////////
/// \brief AdcCfg::setValuePolarityCurrent
/// \param P
/// \return
///
int AdcCfg::setValuePolarityCurrent(const QVector<quint8>& P)
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
int AdcCfg::valuePolarityCurrent(QVector<quint8>& P) const
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].ValuePolarityCurrent;
    return (m_err = ASPT_OK);
}
QVector<quint8> AdcCfg::valuePolarityCurrent() const
{
    QVector<quint8> P(6, 0);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].ValuePolarityCurrent;
    return P;
}
/////////////////////////////////////////////////////////
/// \brief AdcCfg::setVoltageChannel
/// \param P
/// \return
///
int AdcCfg::setVoltageChannel(const QVector<quint8>& P)
{
    const QSet<int> check({ 0, 1, 2, 3, 4, 5, 6 });
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        if ((P[i] != IgnoreValue) && check.contains(P[i]))
            MeasureStep[i].VoltageChannel = P[i];
    return (m_err = ASPT_OK);
}
int AdcCfg::voltageChannel(QVector<quint8>& P) const
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].VoltageChannel;
    return (m_err = ASPT_OK);
}
QVector<quint8> AdcCfg::voltageChannel() const
{
    QVector<quint8> P(6, 0);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].VoltageChannel;
    return P;
}
/////////////////////////////////////////////////////////
/// \brief AdcCfg::setPauseBeforeMeasure
/// \param P
/// \return
///
int AdcCfg::setPauseBeforeMeasure(const QVector<quint8>& P)
{
    const QSet<int> check({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254 });
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        if ((P[i] != IgnoreValue) && check.contains(P[i]))
            MeasureStep[i].PauseBeforeMeasure = P[i];
    return (m_err = ASPT_OK);
}
int AdcCfg::pauseBeforeMeasure(QVector<quint8>& P) const
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].PauseBeforeMeasure;
    return (m_err = ASPT_OK);
}
QVector<quint8> AdcCfg::pauseBeforeMeasure() const
{
    QVector<quint8> P(6, 0);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].PauseBeforeMeasure;
    return P;
}
/////////////////////////////////////////////////////////
/// \brief AdcCfg::setAmountMeasurements
/// \param P
/// \return
///
int AdcCfg::setAmountMeasurements(const QVector<quint8>& P)
{
    const QSet<int> check({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254 });
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        if ((P[i] != IgnoreValue) && check.contains(P[i]))
            MeasureStep[i].AmountMeasurements = P[i];
    return (m_err = ASPT_OK);
}
int AdcCfg::amountMeasurements(QVector<quint8>& P) const
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].AmountMeasurements;
    return (m_err = ASPT_OK);
}
QVector<quint8> AdcCfg::amountMeasurements() const
{
    QVector<quint8> P(6, 0);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].AmountMeasurements;
    return P;
}
/////////////////////////////////////////////////////////
/// \brief AdcCfg::setAmountAverage
/// \param P
/// \return
///
int AdcCfg::setAmountAverage(const QVector<quint8>& P)
{
    const QSet<int> check({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254 });
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        if ((P[i] != IgnoreValue) && check.contains(P[i]))
            MeasureStep[i].AmountAverage = P[i];
    return (m_err = ASPT_OK);
}
int AdcCfg::amountAverage(QVector<quint8>& P) const
{
    if (P.size() > MaxStepCount)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].AmountAverage;
    return (m_err = ASPT_OK);
}
QVector<quint8> AdcCfg::amountAverage() const
{
    QVector<quint8> P(6, 0);
    for (int stop = P.size() - 1, i = 0; i <= stop; i++)
        P[i] = MeasureStep[i].AmountAverage;
    return P;
}
/////////////////////////////////////////////////////////
/// \brief AdcCfg::pack
/// \return
///
int AdcCfg::setPack(const QString& P)
{
    bool ok = false;
    bool M[6]{ false, false, false, false, false, false }; /*# range 0..MaxStepCount-1*/
    bool MaskWas = false;
    int iNew = 0, iOld = 0;
    QString sNew(P), sOld(pack());
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
    m_err = convertSectionFromParcel(B);
    if (m_err != ASPT_OK)
        return m_err;
    for (int stop = MaxStepCount, i = 0; i < stop; ++i)
        UseMeasureStep[i] = M[i];
    return (m_err = ASPT_OK);
}
QString AdcCfg::pack()
{
    QVector<quint8> B;
    QString S('|');
    bool M[6]; /*# range 0..MaxStepCount-1*/
    for (int stop = MaxStepCount, i = 0; i < stop; i++) {
        M[i] = UseMeasureStep[i];
        UseMeasureStep[i] = true;
    }
    m_err = convertSectionToParcel(B);
    if (m_err != ASPT_OK)
        return "";
    for (int stop = MaxStepCount, i = 0; i < stop; ++i)
        UseMeasureStep[i] = M[i];

    for (int stop = MaxStepCount, i = 0; i < stop; ++i)
        S += QString().setNum(UseMeasureStep[i]) + '|';
    S += QString().setNum(measureStepCount()) + '|';
    for (int stop = B.size() - 1, i = 0; i <= stop; i++) {
        S += QString().setNum(B[i]) + '|';
    }
    return S;
}
