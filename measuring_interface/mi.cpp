#include "mi.h"

Aspt* MI::aspt = nullptr;
Measure* MI::measure = nullptr;
Upn* MI::upn = nullptr;

QThread thread;
QSemaphore semafore;

MI::MI()
{
    if (!semafore.available()) {
        aspt = new Aspt;
        upn = new Upn;
        measure = new Measure;

        aspt->moveToThread(&thread);
        measure->moveToThread(&thread);
        upn->moveToThread(&thread);

        thread.connect(&thread, &QThread::finished, aspt, &Measure::deleteLater);
        thread.connect(&thread, &QThread::finished, measure, &Measure::deleteLater);
        thread.connect(&thread, &QThread::finished, upn, &Upn::deleteLater);

        thread.start(QThread::NormalPriority);
    }
    semafore.release();
}

MI::~MI()
{
    semafore.acquire();
    if (!semafore.available()) {
        thread.quit();
        thread.wait();
    }
}
