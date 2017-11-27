#include "measuringinterface.h"

static Upn* upn_ = nullptr;
static Measure* measure_ = nullptr;
static Aspt* aspt_ = nullptr;

static QThread thread;
static QSemaphore semafore;

MI::MI()
{
    if (!semafore.available()) {
        upn_ = new Upn;
        upn_->moveToThread(&thread);
        thread.connect(&thread, &QThread::finished, upn_, &Upn::deleteLater);

        measure_ = new Measure;
        measure_->moveToThread(&thread);
        thread.connect(&thread, &QThread::finished, measure_, &Measure::deleteLater);

        aspt_ = new Aspt;
        aspt_->moveToThread(&thread);
        thread.connect(&thread, &QThread::finished, aspt_, &Measure::deleteLater);

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

Upn* MI::upn() { return upn_; }

Measure* MI::measure() { return measure_; }

Aspt* MI::aspt() { return aspt_; }
