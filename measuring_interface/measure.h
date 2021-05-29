#pragma once

#include <QMutex>
#include <QSemaphore>
#include <QSound>
#include <QThread>
#include <QWaitCondition>

class MeasureModel;

class Measure : public QThread {
    Q_OBJECT
    MeasureModel* const model;
    QMutex mutex;
    QSound m_beep;
    QString const m_AdcCfgList[2];
    QWaitCondition waiter;
    int const measuresCount;

public:
    explicit Measure(MeasureModel* model, int measuresCount, QObject* parent = nullptr);
    void resume() { waiter.wakeAll(); }

signals:
    void showMessage(const QString& msg, const QVector<int>& buttons, int* ret);
    void updateProgressVal(int);

private:
    template <class... Buttons>
    int message(const QString& msg, Buttons... buttons);

protected:
    void run() override;
};
