#ifndef MEASURINGINTERFACE_H
#define MEASURINGINTERFACE_H

#include "Upn/upn.h"
#include "aspt.h"
#include "measure.h"
#include <QObject>

class MI {
public:
    MI();
    ~MI();
    static Aspt* aspt;
    static Measure* measure;
    static Upn* upn;
};

#endif // MEASURINGINTERFACE_H
