#ifndef MYPROTOCOL_H
#define MYPROTOCOL_H

#include <QByteArray>
#include <QDebug>

class MyProtocol {
public:
    enum {
        TransmitStartPattern = 0xAA55,
        ReceiveStartPattern = 0x55AA,
        MinimumSize = 5
    };

#pragma pack(push, 1)
    typedef struct {
        quint16 Start;
        quint8 Size;
        quint8 Command;
        quint8 Data[0x100 - MinimumSize];
    } Parcel_tp;

    typedef struct {
        quint16 Start;
        quint8 Size;
        quint8 Command;
        quint8 CRC;
    } Parcel_tc;

    template <typename T>
    struct Parcel_tt {
        quint16 Start;
        quint8 Size;
        quint8 Command;
        T Data;
        quint8 CRC;
    };
#pragma pack(pop)

    template <typename T>
    QByteArray Parcel(quint8 cmd, const T& data)
    {
        Parcel_tt<T> p = { TransmitStartPattern, MinimumSize + sizeof(T), cmd, data };
        QByteArray parcel(reinterpret_cast<const char*>(&p), p.Size);
        parcel[p.Size - 1] = CalcCrc(parcel);
        //qDebug() << QString("Parcel<%1>").arg(typeid(data).name()) << parcel.toHex().toUpper();
        return parcel;
    }

    QByteArray Parcel(quint8 cmd);

    bool CheckData(const QByteArray& data);

    quint8 CalcCrc(const QByteArray& data);
};

#endif // MYPROTOCOL_H
