#include "myprotocol.h"

QByteArray MyProtocol::Parcel(quint8 cmd)
{
    Parcel_tc p = { TransmitStartPattern, MinimumSize, cmd, 0 };
    QByteArray parcel(reinterpret_cast<const char*>(&p), p.Size);
    reinterpret_cast<quint8*>(parcel.data())[p.Size - 1] = CalcCrc(parcel); //crc
    return parcel;
}

bool MyProtocol::CheckData(const QByteArray& data)
{
    const Parcel_tp* p = reinterpret_cast<const Parcel_tp*>(data.constData());
    if (p->Start == ReceiveStartPattern)
        if (data.size() >= MinimumSize && p->Size == data.size())
            if (p->Data[p->Size - MinimumSize] == CalcCrc(data))
                return true;
    return false;
}

quint8 MyProtocol::CalcCrc(const QByteArray& data)
{
    enum { POLYNOMIAL = 0x1D }; // x^8 + x^4 + x^3 + x^2 + 1
    quint8 crc8 = 0;
    for (quint8 i = 0, end = static_cast<quint8>(data.size() - 1); i < end; ++i) {
        crc8 ^= data[i];
        for (quint8 j = 0; j < 8; ++j)
            crc8 = static_cast<quint8>((crc8 & 0x80) ? (crc8 << 1) ^ POLYNOMIAL : crc8 << 1);
    }
    return crc8;
}
