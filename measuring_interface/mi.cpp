#include "mi.h"

MI::MI() {
    if(!semafore.available()) {
        m_aspt = new Aspt;
        m_upn = new Upn;
        m_measure = new Measure;

        m_aspt->moveToThread(&thread);
        m_measure->moveToThread(&thread);
        m_upn->moveToThread(&thread);

        thread.connect(&thread, &QThread::finished, m_aspt, &Measure::deleteLater);
        thread.connect(&thread, &QThread::finished, m_measure, &Measure::deleteLater);
        thread.connect(&thread, &QThread::finished, m_upn, &Upn::deleteLater);

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
