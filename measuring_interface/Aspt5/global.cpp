#include "global.h"

int AsptProtocol::CheckControlAmount(QVector<quint8> B)
{
    quint16 j = 0;

    if (B.size() < 3)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);

    for (int stop = B.size() - 2, i = 0; i < stop; i++)
        j = j + B[i];

    if (B[B.size() - 2] == (j & 0xFF))
        if (B[B.size() - 1] == (j >> 8))
            return (m_err = ASPT_OK);
    return (m_err = ASPT_CONTROL_AMOUNT_ERROR);
}

int AsptProtocol::ControlAmount(QVector<quint8>& B)
{
    quint16 j = 0;
    if (B.size() < 3)
        return (m_err = ASPT_FUNCTION_INVALID_PARAMETER);

    for (int i = 0, stop = B.size() - 2; i < stop; i++)
        j = j + B[i];

    B[B.size() - 2] = (j & 0xFF);
    B[B.size() - 1] = (j >> 8);
    return (m_err = ASPT_OK);
}

double AsptProtocol::Byte5ToDouble(quint8* M)
{
    quint64 tmp1 = 0, tmp2 = 0;

    if (M[0] == 0)
        return 0.0;

    tmp2 = M[0] + 894; // exp
    tmp1 |= tmp2 << 52;

    if (M[1] & 0x80) // sign
        tmp1 |= 0x8000000000000000;

    qSwap(M[5], M[1]);
    qSwap(M[4], M[2]);

    tmp2 = *reinterpret_cast<quint64*>(M) & 0x7FFFFFFFFF00; // mantisa
    tmp1 |= tmp2 << 5;

    return *reinterpret_cast<double*>(&tmp1);
}

int AsptProtocol::ConvertSectionToParcel(BlockName_e BlockName, QVector<quint8>& B, Commands_e Command, quint16 Address, quint8 TrLength, quint8 RcLength)
{
    m_captionTransmit.CommandCode = Command;
    m_captionTransmit.EEPROMAddress = Address;
    m_captionTransmit.TransmitMasLength = TrLength;
    m_captionTransmit.ReceiveMasLength = RcLength;
    switch (BlockName) {
    case bnCaptionTransmit:
        B.fill(0, sizeof(Caption_t));
        *reinterpret_cast<Caption_t*>(B.data()) = m_captionTransmit;
        break;
    case bnCaptionReceive:
        B.fill(0, sizeof(Caption_t));
        *reinterpret_cast<Caption_t*>(B.data()) = m_captionReceive;
        break;
    case bnEEPROMParameters:
        //            B.fill(0, sizeof(EepromParameters_t));
        //            *reinterpret_cast<EepromParameters_t*>(B.data()) = bEEPROMParameters;
        break;
    }
    if (ControlAmount(B) != ASPT_OK)
        return m_err;
    return (m_err = ASPT_OK);
}

int AsptProtocol::ConvertSectionFromParcel(BlockName_e BN, const QVector<quint8>& B)
{
    switch (BN) {
    case bnCaptionTransmit:
        m_captionTransmit = *reinterpret_cast<const Caption_t*>(B.data());
        break;
    case bnCaptionReceive:
        m_captionReceive = *reinterpret_cast<const Caption_t*>(B.data());
        break;
    case bnEEPROMParameters:
        //            bEEPROMParameters = *reinterpret_cast<const EepromParameters_t*>(B.data());
        break;
    }
    return (m_err = ASPT_OK);
}

int AsptProtocol::AnalysisCaptionReceive(QVector<quint8>& B)
{
    // проверка КС
    if (CheckControlAmount(B) != ASPT_OK)
        return m_err;
    // извлекаем данные из посылки
    if (ConvertSectionFromParcel(bnCaptionReceive, B) != ASPT_OK)
        return m_err;
    // проверка номера прибора
    if (m_captionTransmit.DeviceNumber != m_captionReceive.DeviceNumber)
        return (m_err = ASPT_INVALID_DEVICE_NUMBER);
    // резалты
    return (m_err = ASPT_OK);
}

//------------------------------------------------------------------------------

//const QSet<QChar /*, 0, 255*/> global_u__0({ QChar('e'), QChar('E') });
//const QSet<QChar /*, 0, 255*/> global_u__1({ QChar('+'), QChar('-') });
//const QSet<QChar /*, 0, 255*/> global_u__2({ QChar(','), QChar('.') });
//const QSet<QChar /*, 0, 255*/> global_u__3({ QChar('e'), QChar('E') });
//const QSet<QChar /*, 0, 255*/> global_u__4({ QChar('+'), QChar('-') });
//typedef quint8 quint8;

//double Degree(double B, quint16 d)
//{
//    double result = 0.0;
//    int i = 0;
//    double V = 0.0;
//    V = 1;
//    if (d > 0)
//        for (int stop = d - 1, i = 0; i <= stop; i++)
//            V = V * B;
//    result = V;
//    return result;
//}
////------------------------------------------------------------------------------

//double DtoR48(double d)
//{
//    uint64_t r = 0, tmp = 0;

//    tmp = *(uint64_t*)&d; //e
//    tmp >>= 52; // tmp /= 0x10000000000000;
//    tmp -= 894;
//    tmp &= 0xFF;
//    if (tmp == 0)
//        return 0.0;
//    r |= tmp;

//    tmp = *(uint64_t*)&d; //m
//    tmp >>= 5; // tmp /= 0x20;
//    tmp &= 0x7FFFFFFFFF00;
//    r |= tmp;

//    tmp = *(uint64_t*)&d; //s
//    tmp >>= 16; // tmp /= 0x10000;
//    tmp &= 0x800000000000;
//    r |= tmp;

//    return *(double*)&r;
//}

//double R48toD(double r)
//{
//    // Convert Turbo Pascal REAL (aka delphi double) data type to double
//    // TurboPascal real data type (6 bytes, 2.9 x 10-39 to 1.7 x 1038)
//    //
//    // 1                   39                       8
//    // +-+---------------------------------------+--------+
//    // |s|                  f                    |   e    |
//    // +-+---------------------------------------+--------+
//    // e = 0 -> v = 0
//    // exponent bias = 129

//    uint64_t d = 0, tmp = 0;

//    tmp = *(uint64_t*)&r; //e
//    tmp &= 0xFF;
//    if (tmp == 0)
//        return 0.0;

//    tmp += 894;
//    tmp <<= 52; // tmp *= 0x10000000000000;
//    d |= tmp;

//    tmp = *(uint64_t*)&r; //m
//    tmp &= 0x7FFFFFFFFF00;
//    tmp <<= 5; // tmp *= 0x20;
//    d |= tmp;

//    tmp = *(uint64_t*)&r; //s
//    tmp &= 0x800000000000;
//    tmp <<= 16; // tmp *= 0x10000;
//    d |= tmp;

//    return *(double*)&d;
//}

//void RealToByte5(double& R, quint8* M, int M_maxidx)
//{
//    // __asm {
//    // push ESI;
//    // push EDI;
//    // MOV ESI,SS:[R]
//    // MOV EDI,SS:[M]
//    // MOV AL,[ESI]
//    // MOV DS:[EDI],AL
//    // inc EDI
//    // inc ESI
//    // inc ESI
//    // inc ESI
//    // inc ESI
//    // inc ESI
//    // MOV AL,[ESI]
//    // MOV DS:[EDI],AL
//    // MOV DL,AL
//    // and DL,128
//    // inc EDI
//    // DEC ESI
//    // MOV AL,[ESI]
//    // MOV DS:[EDI],AL
//    // inc EDI
//    // DEC ESI
//    // MOV AL,[ESI]
//    // MOV DS:[EDI],AL
//    // inc EDI
//    // DEC ESI
//    // MOV AL,[ESI]
//    // MOV DS:[EDI],AL
//    // DEC ESI
//    // MOV AL,[ESI]
//    // and AL,128
//    // JZ e1
//    // or byte PTR DS:[EDI-3],128
//    // Add byte PTR DS:[EDI],1
//    // DEC EDI
//    // ADC byte PTR DS:[EDI],0
//    // DEC EDI
//    // ADC byte PTR DS:[EDI],0
//    // DEC EDI
//    // ADC byte PTR DS:[EDI],0
//    // DEC EDI
//    // ADC byte PTR DS:[EDI],0
//    // inc EDI
//    // and byte PTR DS:[EDI],127
//    // or DS:[EDI],DL
//    //e1:
//    // POP EDI
//    // POP ESI
//    // }
//}
//------------------------------------------------------------------------------
//double Byte5ToDouble(quint8* M)
//{
//    quint64 tmp1 = 0, tmp2 = 0;

//    if (M[0] == 0)
//        return 0.0;

//    tmp2 = M[0] + 894; // exp
//    tmp1 |= tmp2 << 52;

//    if (M[1] & 0x80) // sign
//        tmp1 |= 0x8000000000000000;

//    qSwap(M[5], M[1]);
//    qSwap(M[4], M[2]);

//    tmp2 = *reinterpret_cast<quint64*>(M) & 0x7FFFFFFFFF00; // mantisa
//    tmp1 |= tmp2 << 5;

//    return *reinterpret_cast<double*>(&tmp1);
//}
////------------------------------------------------------------------------------

//int ControlAmount(QVector<quint8>& B)
//{
//    quint16 j = 0;
//    if (B.size() < 3)
//        return ASPT_FUNCTION_INVALID_PARAMETER;

//    for (int i = 0, stop = B.size() - 2; i < stop; i++)
//        j = j + B[i];

//    B[B.size() - 2] = (j & 0xFF);
//    B[B.size() - 1] = (j >> 8);
//    return ASPT_OK;
//}
////------------------------------------------------------------------------------

//int CheckControlAmount(QVector<quint8> B)
//{
//    quint16 j = 0;

//    if (B.size() < 3)
//        return ASPT_FUNCTION_INVALID_PARAMETER;

//    for (int stop = B.size() - 2, i = 0; i < stop; i++)
//        j = j + B[i];

//    if (B[B.size() - 2] == (j & 0xFF))
//        if (B[B.size() - 1] == (j >> 8))
//            return ASPT_OK;
//    return ASPT_CONTROL_AMOUNT_ERROR;
//}
////------------------------------------------------------------------------------

//bool IntIsValidChar(const QString& Text, QChar Key)
//{
// bool result = false;
// QSet<QChar /*, 0, 255*/> ValidChars;
// ValidChars = (QSet<QChar /*, 0, 255*/>()
// << QChar('+')
// << QChar('-')
// << QChar('0')
// << QChar('1')
// << QChar('2')
// << QChar('3')
// << QChar('4')
// << QChar('5')
// << QChar('6')
// << QChar('7')
// << QChar('8')
// << QChar('9'));
// if (Pos("E", AnsiUpperCase(Text)) == 0)
// ValidChars = ValidChars + global_u__0;
// if ((Pos("-", Text) != 0) || (Pos("+", Text) != 0))
// ValidChars = ValidChars - global_u__1;
// result = (ValidChars.Contains(Key)) || (Key < '\x20');
// return result;
//}
//------------------------------------------------------------------------------

//bool FloatIsValidChar(const QString& Text, QChar Key)
//{
// bool result = false;
// QSet<QChar /*, 0, 255*/> ValidChars; // GetEnvironmentVariable
// ValidChars = (QSet<QChar /*, 0, 255*/>()
// << QChar('+') << QChar('-') << QChar('0') << QChar('1') << QChar('2') << QChar('3') << QChar('4') << QChar('5') << QChar('6') << QChar('7')
// << QChar('8') << QChar('9'));
// if (Pos(DecimalSeparator, Text) == 0)
// ValidChars = ValidChars + global_u__2;
// if (Pos("E", AnsiUpperCase(Text)) == 0)
// ValidChars = ValidChars + global_u__3;
// if ((Pos("-", Text) != 0) || (Pos("+", Text) != 0))
// ValidChars = ValidChars - global_u__4;
// result = (ValidChars.Contains(Key)) || (Key < '\x20');
// return result;
//}
//------------------------------------------------------------------------------

//quint16 ShowProgramDlg( TForm Form, const QString& DlgMsg, TMsgDlgType DlgType, TMsgDlgButtons DlgButtons )
//{
// quint16 result = 0;
// TForm MsgDlg;
// MsgDlg = CreateMessageDialog( DlgMsg, DlgType, DlgButtons );
// MsgDlg.Left = Form.Left + ( Form.Width / 2 ) - ( MsgDlg.Width / 2 );
// MsgDlg.Top = Form.Top + ( Form.Height / 2 ) - ( MsgDlg.Height / 2 );
// result = MsgDlg.ShowModal;
// MsgDlg.Free;
// return result;
//}
//------------------------------------------------------------------------------
