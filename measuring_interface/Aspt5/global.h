#ifndef global_uH
#define global_uH

//#include "ExCOM_ASPT/com_u.h"
#include <QObject>
#include <QSet>
#include <QVector>

enum {
    MaxParcelLength = 512,
    MaxBuffSize = 40, //максимальный размер передаваемых одновременно данных в EEPROM.
    DeviceType = 14, // тип прибора АСПТ
    ErrValue = 9999 // значение, в случае невозможности подсчитать результат
};

enum {
    ASPT_OK = 0, // Ok (все в порядке, функция завершила свою работу)
    ASPT_ERROR = -101, // Error (не описанная или неизвестная)
    ASPT_FUNCTION_INVALID_PARAMETER = -102, // Ошибка в параметрах вызываемой функции
    ASPT_CONTROL_AMOUNT_ERROR = -103, // Ошибка подсчета контрольной суммы
    ASPT_INVALID_DEVICE_NUMBER = -104, // Неверный адрес прибора
    ASPT_DIRECT_CALCULATION_ERROR = -196, // Ошибка при расчете температуры в функции DirectCalculation
    ASPT_REVERSE_CALCULATION_ERROR = -197, // Ошибка при расчете изм. значения в функции ReverseCalculation
    ASPT_CALCULATION_ERROR = -198, // Ошибка в вычислениях (например, деление на 0)
    ASPT_DLL_ERROR_HANDLE_FREE = -199, // Ошибка DLL - Handle неопределен
    ASPT_ADC_OVERFLOW = -200, // Переполнение АЦП (изм = 7FFFFh) "поданный сигнал превышает допустимый"
};

//  -1255..-1000                           // Внутренние ошибки прибора

enum TAccessType {
    atFull,
    atMedium,
    atLow,
    atSpecial1,
    atNone
};

enum TSensorType {
    vtNil, // vt - Verified Thermoelement;
    vtR2W,
    vtR3W,
    vtR4W,
    vtTC,
    vtTC0,
    vtCurr,
    vtVolt,
    etR4W, // et - Exemplary Thermometer;
    etPTS,
    etTC,
    etCurr,
    etUser,
    dvTrm, // dv - Device Buil-in Thermometer;
    inCurr, // in - Invert Verified Thermoelement.
    inVolt
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
    mUser
};

enum TSupportingResistor {
    sr15,
    sr150,
    sr1000
};

enum TPolarityCurrent {
    c01,
    c02,
    c05,
    c10,
    c15,
    c20,
    c30,
    c40,
    negativeCurrent = 8
};

/*
 * Протокол обмена с АСПТ
 * 1.  Идентификатор (0AAh)     (8)
 * 2.  Сетевой номер прибора    (8)
 * 3.  Команда                  (8)
 * 4.  Адрес                    (16)
 * 6.  Передаваемая длина (N)   (8)
 * 7.  Запрашиваемая длина      (8)
 * 8.  Контрольная сумма        (16)
 * 10. Блок с данными           (N байт)
 * 11. Контрольная сумма        (16)
 *
 * Команды:
 * 1 – Обработать шаги
 * 2 – Записать EEPROM
 * 3 – Прочитать EEPROM
 * 4 - ``
 * 5 – Ждите ответа
 * 6 – Чтение параметров из EEPROM
 * 7 – Запись параметров в EEPROM
 * 8 – Изменение сетевого номера
 * 9 – Ответ с кодом ошибки (в блоке данных первые три байта – код ошибки)
 * 10 – Считывание массива из 256 значений одного канала
 *
 * Коды ошибок
 *  DEEOk:  EQU  0 ;  Нет ошибки
 *  DEEKEY0: EQU  1 ;  Ошибка данных EEPROM
 *  DEEKEY1: EQU  2 ;  Ошибка чтения EEPROM
 *  DEEKEY2: EQU  3 ;  Ошибка записи EEPROM
 *  DEEADC:  EQU  4 ;  Ошибка DRDY  АЦП
 *  DEEMATH:  EQU  5 ;  Результат выходит за разрядную сетку
 *  DEECHAN:  EQU  6 ;  Не существующий номер канала
 *  DEENO:    EQU  7 ;  Нет результата
 *  DEEABT:   EQU  8 ;  Останов измерений по приему посылки
 *  DEEINT:   EQU  9 ;  Внутренняя ошибка
 *  DEESEND:  EQU 10 ;  Ошибка в посылке
 *  DEENoBYT: EQU 11 ;  Не хватает байтов в шагах
 *  DEEPARAM: EQU 12 ;  Ошибка в параметрах
 *  DEEFIR:   EQU 13 ;  Нет готовности FIR-фильтра
 *
*/

//--- Коды ошибок АСПТ
enum {
    //ASPT_OK = 0, // нет ошибки
    ASPT_EEPROMDATAERROR = 1, //    ошибка данных EEPROM
    ASPT_EEPROMREADERROR = 2, //    ошибка чтения EEPROM
    ASPT_EEPROMWRITEERROR = 3, //   ошибка записи EEPROM
    ASPT_ADCDRDYERROR = 4, //       ошибка DRDY АЦП
    ASPT_RESULTERROR = 5, //        результат выходит за разрядную сетку
    ASPT_CHANNELERROR = 6, //       не существующий номер канала
    ASPT_RESULTABSENT = 7, //       нет результата
    ASPT_STOPMEASURE = 8, //        останов измерений по приему посылки
    ASPT_INTERNALERROR = 9, //      внутренняя ошибка
    ASPT_PARCELERROR = 10, //       ошибка в передаваемой посылке
    ASPT_BYTEABSENT = 11, //        не хватает байтов в шагах измерения
    ASPT_PARAMETERSERROR = 12, //   ошибка в параметрах
    ASPT_FirNotReady = 13, //       Нет готовности FIR-фильтра
};

// Команды:
enum Commands_e {
    ProcessSteps = 1, //           * 1 – Обработать шаги
    RecordEEPROM_ = 2, //        * 2 – Записать EEPROM
    ReadEEPROM_ = 3, //          * 3 – Прочитать EEPROM
    WaitingAnswer = 5, //       * 5 – Ждите ответа
    ReadingFromEEPROM_ = 6, //   * 6 – Чтение параметров из EEPROM
    WriteToTheEEPROM_ = 7, //    * 7 – Запись параметров в EEPROM
    ChangeNetworkNumber = 8, // * 8 – Изменение сетевого номера
    AnswerWithErrorCode = 9, // * 9 – Ответ с кодом ошибки(в блоке данных первые три байта – код ошибки)
    ReadingArray = 10, //       * 10 – Считывание массива из 256 значений одного канала
};

// блоки данных
enum BlockName_e {
    bnCaptionTransmit,
    bnCaptionReceive,
    bnEEPROMParameters // данные EEPROM
};

// параметры прибора
enum DeviceParameter_e {
    dpROP1, // 0
    dpROP2, // 1
    dpROP3, // 2
    dpROP4, // 3
    dpKU0, // 4
    dpKU1, // 5
    dpRT0, // 6
    dpKOEF1, // 7
    dpKOEF2, // 8
    dpALFAROP1, // 9
    dpALFAROP2, // 10
    dpALFAROP3, // 11
    dpALFAROP4, // 12
    dpALFAKU0, // 13
    dpALFAKU1, // 14
    dpRATIO, // 15
    dpTIP, // 16
    dpNumber // 17
};

class AsptProtocol {
protected:
#pragma pack(push, 1)
    // заголовок посылки
    struct Caption_t {
        quint8 DeviceIdentifier = 0xAA; // идентификатор прибора
        quint8 DeviceNumber = 0x01; // cетевой номер прибора
        quint8 CommandCode = 0; // код команды
        quint16 EEPROMAddress = 0; // адрес EEPROM (2 байта)
        quint8 TransmitMasLength = 0; // передаваемая длина массива
        quint8 ReceiveMasLength = 0; // запрашиваемая длина массива
        quint16 ControlAmount = 0; // контрольная сумма (2 байта)
    };
#pragma pack(pop)
    // передаваемый заголовок посылки
    Caption_t m_captionTransmit;
    // принимаемый заголовок посылки
    Caption_t m_captionReceive;
    // код ошибки
    mutable int m_err = 0;

    int CheckControlAmount(QVector<quint8> B);
    int ControlAmount(QVector<quint8>& B);
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
    double Byte5ToDouble(quint8* M);

    //    // очистка блока данных
    //    int ClearSection(BlockName_e BN)
    //    {
    //        switch (BN) {
    //        case bnCaptionTransmit:
    //            memset(&bCaptionTransmit, 0, sizeof(Caption_t));
    //            break;
    //        case bnCaptionReceive:
    //            memset(&bCaptionReceive, 0, sizeof(Caption_t));
    //            break;
    //            //        case bnEEPROMParameters:
    //            //            memset(&bEEPROMParameters, 0, sizeof(EepromParameters_t));
    //            //            break;
    //        }
    //        //ASPT_ERROR;
    //        return ASPT_OK;
    //    }

    // преобразование блока данных в посылку
    //    int ConvertSectionToParcel(BlockName_e BN, QVector<quint8>& B)
    //    {
    //        switch (BN) {
    //        case bnCaptionTransmit:
    //            B.fill(0, sizeof(Caption_t));
    //            *reinterpret_cast<Caption_t*>(B.data()) = bCaptionTransmit;
    //            break;
    //        case bnCaptionReceive:
    //            B.fill(0, sizeof(Caption_t));
    //            *reinterpret_cast<Caption_t*>(B.data()) = bCaptionReceive;
    //            break;
    //            //        case bnEEPROMParameters:
    //            //            B.fill(0, sizeof(EepromParameters_t));
    //            //            *reinterpret_cast<EepromParameters_t*>(B.data()) = bEEPROMParameters;
    //            //            break;
    //        }
    //        if (ControlAmount(B) != ASPT_OK)
    //            return Err;
    //        return (Err = ASPT_OK);
    //    }
    int ConvertSectionToParcel(BlockName_e BlockName, QVector<quint8>& B, Commands_e Command, quint16 Address, quint8 TrLength, quint8 RcLength);
    // преобразование посылки в блок данных
    int ConvertSectionFromParcel(BlockName_e BN, const QVector<quint8>& B);
    // анализ заголовка полученной посылки
    int AnalysisCaptionReceive(QVector<quint8>& B);
};

//double Degree(double B, quint16 d);
//void RealToByte5(double& R, quint8* M, int M_maxidx);
//double Byte5ToDouble(quint8* M);
//int ControlAmount(QVector<quint8>& B);
//int CheckControlAmount(QVector<quint8> B);
//bool IntIsValidChar(const QString& Text, QChar Key);
//bool FloatIsValidChar(const QString& Text, QChar Key);
//quint16 ShowProgramDlg(TForm Form, const QString& DlgMsg, TMsgDlgType DlgType, TMsgDlgButtons DlgButtons);

#endif //  global_uH
