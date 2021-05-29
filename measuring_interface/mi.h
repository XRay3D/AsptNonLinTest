#pragma once

#include "aspt.h"
#include "upn/upn.h"

class MI {
    static inline Aspt* m_aspt;
    static inline Upn* m_upn;
    static inline QThread thread;
    static inline QSemaphore semafore;

public:
    MI();
    ~MI();
    static Aspt* aspt() { return m_aspt; }
    static Upn* upn() { return m_upn; }
};
