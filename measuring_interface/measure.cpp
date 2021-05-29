#include "measure.h"
#include "measuremodel.h"
#include "mi.h"
#include <QCoreApplication>
#include <QMessageBox>
#include <QSerialPortInfo>
#include <random>

const int id[]{
    qRegisterMetaType<QVector<int>>("QVector<int>"),
};

// NOTE true on release
constexpr bool dbg = true;

Measure::Measure(MeasureModel* model, int measuresCount, QObject* parent)
    : QThread{parent}
    , model{model}
    , m_beep{"beep.wav", this}
    , m_AdcCfgList{
          "|1|1|1|0|0|0|3|6|28|3|0|0|5|0|20|6|1|28|3|0|0|5|6|20|6|1|28|3|0|0|5|5|20|6|1|0|0|0|0|5|0|1|1|1|0|0|0|0|0|0|1|1|1|0|0|0|0|0|0|1|1|1|220|0|\0",
          "|1|1|1|0|0|0|3|6|28|3|0|0|5|1|20|6|1|28|3|0|0|5|6|20|6|1|28|3|0|0|5|5|20|6|1|0|0|0|0|5|0|1|1|1|0|0|0|0|0|0|1|1|1|0|0|0|0|0|0|1|1|1|221|0|\0",
      }
    , measuresCount{measuresCount} //
{
}
template <class... Buttons>
int Measure::message(const QString& msg, Buttons... buttons) {
    qDebug(__FUNCTION__);
    int ret{};
    mutex.lock();
    emit showMessage(msg, {buttons...}, &ret);
    waiter.wait(&mutex);
    mutex.unlock();
    return ret;
}

void Measure::run() {
    std::random_device rd; // NOTE random_device for debuging without conected devices
    std::uniform_real_distribution<double> dist(-0.005, +0.005);

    if(!dbg && MI::aspt()->initialize() != Error::ASPT_OK) {
        message("Нет свяи с АСПТ!", QMessageBox::Ok);
        return;
    }

    int ctr{};

    for(int row = 0; row < RowCount; ++row) {
        if(model->rowEnabled()[row] == Qt::Unchecked)
            continue;

        int adcCh = 3;
        m_beep.play();

        if(!dbg && message(QString("Подключите УПН №%1 в канал %2 АСПТ").arg(MI::upn()->resistors()[6]).arg(row + 1), QMessageBox::Ok, QMessageBox::Abort) == QMessageBox::Abort)
            return;

        if(!dbg && MI::aspt()->correction() != Error::ASPT_OK) {
            message("Нет свяи с УПН!", QMessageBox::Ok);
            return;
        }

        if(isInterruptionRequested())
            return;

        int res;
        double v = 0.0;

        model->clearData(row);

        for(res = 0; res < 6; ++res) {
            if(!dbg && !MI::upn()->setResistor(res)) {
                message("Нет свяи с УПН!", QMessageBox::Ok);
                return;
            }
            AdcCfg ADCCfg;
            if(res < 3) {
                if(!(adcCh & 0x1)) {
                    res = 2;
                    continue;
                }
                ADCCfg.setPack(m_AdcCfgList[0]);
            } else {
                if(!(adcCh & 0x2)) {
                    res = 6;
                    continue;
                }
                ADCCfg.setPack(m_AdcCfgList[1]);
            }

            quint8 cfg = row;
            ADCCfg.setMeasureChannel({cfg, cfg, cfg, cfg, cfg, cfg});
            cfg = sr150;
            ADCCfg.setSupportingResistor({cfg, cfg, cfg, cfg, cfg, cfg});
            cfg = c10;
            ADCCfg.setValuePolarityCurrent({cfg, cfg, cfg, cfg, cfg, cfg});

            msleep(100);

            for(int i = 0; i < measuresCount; ++i) {
                if(!dbg && MI::aspt()->getMeasureValue(ADCCfg, vtR4W, 1.0, v) != 0) {
                    message("Нет свяи с АСПТ!", QMessageBox::Ok);
                    return;
                }
                if(dbg) { // NOTE fake measure
                    v = (1 + res % 3) * 100 + dist(rd);
                    msleep(100);
                }
                if(isInterruptionRequested())
                    return;

                model->addData(row, res, v);

                //        QMessageBox::warning(this, ui->leAsptSerNum->text(), tr("Проверь подключение УПН(%2) к каналу №%1 АСПТ!").arg(row + 1).arg(ui->leUpnSerNum->text()), tr("Хорошо"), "");
                //                    qDebug() << v;
                //                    switch (res) {
                //                    case 2:
                //                    case 5:
                //                        if (290 > v || 310 < v) {
                //                            fl = true;
                //                            // NOTE emit doMessage(CheckUptToAsptConnection, asptCh);
                //                            while (!checkStop())
                //                                continue;
                //                        } else
                //                            fl = false;
                //                        break;
                //                    default:
                //                        if (140 > v || 160 < v) {
                //                            fl = true;
                //                            // NOTE emit doMessage(CheckUptToAsptConnection, asptCh);
                //                            while (!checkStop())
                //                                continue;
                //                        } else
                //                            fl = false;
                //                        break;
                //                    }
                //                    msleep(200);
                //                } while (fl);
                //                emit measureReady(v, asptCh, res, i);
                emit updateProgressVal(++ctr);
            }
        }
    }
    m_beep.play();
}
