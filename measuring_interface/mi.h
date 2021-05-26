#pragma once

#include "Upn/upn.h"
#include "aspt.h"
#include "measure.h"
#include <QObject>

class MI {
    static inline Aspt* m_aspt;
    static inline Measure* m_measure;
    static inline Upn* m_upn;
    static inline QThread thread;
    static inline QSemaphore semafore;

public:
    MI();
    ~MI();
    static Aspt* aspt() { return m_aspt; }
    static Measure* measure() { return m_measure; }
    static Upn* upn() { return m_upn; }
};
