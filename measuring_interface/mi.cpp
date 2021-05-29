#include "mi.h"

MI::MI() {
    if(!semafore.available()) {
        m_aspt = new Aspt;
        m_upn = new Upn;

        m_aspt->moveToThread(&thread);
        m_upn->moveToThread(&thread);

        thread.connect(&thread, &QThread::finished, m_aspt, &QObject::deleteLater);
        thread.connect(&thread, &QThread::finished, m_upn, &QObject::deleteLater);

        thread.start(QThread::NormalPriority);
    }
    semafore.release();
}

MI::~MI() {
    semafore.acquire();
    if(!semafore.available()) {
        thread.quit();
        thread.wait();
    }
}
