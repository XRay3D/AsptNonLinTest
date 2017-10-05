#ifndef MY_PROTOCOL_H
#define MY_PROTOCOL_H

#include <stdint.h>
#include <QByteArray>

enum { OLD_UPN = 1 }; // x^8 + x^4 + x^3 + x^2 + 1

class MyProtocol {
public:
    MyProtocol();
    char CalcCrc(const QByteArray& Data);
    QByteArray Parcel(QByteArray& Data);
    QByteArray Parcel(uint8_t* data_, uint8_t Len = 1);
    bool CheckData(const QByteArray& Data);
};

#endif // MY_PROTOCOL_H
