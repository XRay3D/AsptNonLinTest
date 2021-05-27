#pragma once

#include <QThread>
class QComboBox;
class DevFinder : public QThread {
    Q_OBJECT
    QComboBox *const cbxAspt, *const cbxUpn;

public:
    DevFinder(QComboBox* cbxAspt, QComboBox* cbxUpn);

signals:
    void updateProgressVal(int);
    void updateProgressMax(int);

protected:
    void run() override;
};
