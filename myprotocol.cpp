#include "myprotocol.h"
#include <QDebug>

enum { POLYNOMIAL = 0x1D }; // x^8 + x^4 + x^3 + x^2 + 1

enum {
    StartTx0 = +85,
    StartTx1 = -86,
    StartRx0 = -86,
    StartRx1 = +85
};

MyProtocol::MyProtocol()
{
}

bool MyProtocol::CheckData(const QByteArray& Data)
{
    //qDebug() << "CheckData" << Data.toHex().toUpper();
    if (!Data.isEmpty()) {
        const char start[3]{ StartRx0, StartRx1, 0 };
        if (Data.indexOf(start) == 0) {
            if (Data[2] == Data.size()) {
                if (Data[Data.size() - 1] == CalcCrc(Data)) {
                    //qDebug() << "CheckData" << true;
                    return true;
                }
            }
        }
    }
    //qDebug() << "CheckData" << false;
    return false;
}

QByteArray MyProtocol::Parcel(uint8_t* data_, uint8_t Len)
{
    QByteArray data;
    const char start[2]{ StartTx0, StartTx1 };
    data.append(start, 2);
    data.append(Len + 4); //размер посылки
    for (int i = 0; i < Len; ++i) {
        data.append(*data_++); //данные
    }
    data.append(CalcCrc(data)); //crc
    //qDebug() << "Parcel" << data.toHex().toUpper();
    return data;
}

QByteArray MyProtocol::Parcel(QByteArray& Data)
{
    return Parcel((uint8_t*)Data.data(), (uint8_t)Data.size());
}

char MyProtocol::CalcCrc(const QByteArray& Data)
{
    uint8_t crc8 = 0;
    uint8_t len = Data.size();
    const char start[3]{ StartRx0, StartRx1, 0 };
    if (Data.indexOf(start) == 0)
        --len;
    for (uint16_t i = 0; i < len; ++i) {
        crc8 ^= Data[i];
        for (uint16_t j = 0; j < 8; ++j) {
            if (crc8 & 0x80) {
                crc8 = (crc8 << 1) ^ POLYNOMIAL;
            }
            else {
                crc8 <<= 1;
            }
        }
    }
    return crc8;
}
