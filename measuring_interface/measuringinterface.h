#ifndef MEASURINGINTERFACE_H
#define MEASURINGINTERFACE_H

#include <QObject>
#include "Upn/upn.h"
#include "measure.h"
#include "Aspt5/aspt.h"

class MI {
public:
    MI();
    ~MI();
    static Upn* upn();
    static Measure* measure();
    static Aspt* aspt();
};

#endif // MEASURINGINTERFACE_H
