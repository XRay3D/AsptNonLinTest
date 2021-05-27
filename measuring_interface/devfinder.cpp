#include "devfinder.h"

#include "mi.h"
#include <QComboBox>
#include <QSerialPortInfo>
#include <chrono>

namespace chr = std::chrono;
struct Timer {

#ifdef __gnu_linux__
    chrono::time_point<chrono::system_clock, chrono::nanoseconds> t1;
#else
    chr::time_point<chr::steady_clock> t1;
#endif
    static inline std::map<const char*, double> avg {};
    static inline std::map<const char*, size_t> ctr {};
    std::string_view string_view;
    Timer(std::string_view span)
        : t1 { chr::high_resolution_clock::now() }
        , string_view { span }
    {
    }
    ~Timer()
    {
        using chr::duration;
        using chr::high_resolution_clock;
        using chr::milliseconds;

        duration<double, std::milli> ms_double { chr::high_resolution_clock::now() - t1 };
        avg[string_view.data()] += ms_double.count();
        qDebug() << "time (" << string_view.data() << ")" << ms_double.count() << "ms"; //<< "avg" << (avg[string_view.data()] / ++ctr[string_view.data()]) << "ms";
    }
};

DevFinder::DevFinder(QComboBox* cbxAspt, QComboBox* cbxUpn)
    : cbxAspt { cbxAspt }
    , cbxUpn { cbxUpn }
{
    cbxAspt->clear();
    cbxUpn->clear();
}

void DevFinder::run()
{
    auto ports(QSerialPortInfo::availablePorts().toVector());
    emit updateProgressMax(ports.size() * 2);
    std::ranges::sort(ports, {}, [](const auto& a) { return a.portName().mid(3).toInt(); });
    //                QElapsedTimer t;

    int ctr {};
    for (auto&& portInfo : ports) {
        auto portName { portInfo.portName() };
        {
            Timer T("aspt");
            if (MI::aspt()->ping(portName)) {
                cbxAspt->addItem(portName, MI::aspt()->serialNumber());
                MI::aspt()->close();
            }
        }
        emit updateProgressVal(++ctr);
        if (isInterruptionRequested())
            break;
        {
            Timer T("upn");
            if (MI::upn()->ping(portName) && MI::upn()->readResistorValue()) {
                cbxUpn->addItem(portName, QVariant::fromValue(MI::upn()->resistors()));
                emit MI::upn()->close();
            }
        }
        emit updateProgressVal(++ctr);
        if (isInterruptionRequested())
            break;
    }
}
