#ifndef MEASURINGINTERFACE_H
#define MEASURINGINTERFACE_H

#include "Aspt5/aspt.h"
#include "Upn/upn.h"
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
