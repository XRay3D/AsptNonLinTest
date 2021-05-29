#pragma once

#include <QWidget>

class QComboBox;

class QLineEdit;
namespace Ui {
class TapUpn;
}

class TapUpn : public QWidget {
    Q_OBJECT

public:
    explicit TapUpn(QWidget* parent = nullptr);
    ~TapUpn();

    void update();

    void setUiWidgets(QComboBox* _cbxUpn, QLineEdit* _leUpnSerNum);

signals:
    void setResistor(int r = 0);

private slots:
    void on_pbUpnRead_clicked();
    void on_pbUpnWrite_clicked();

private:
    Ui::TapUpn* ui;
    QComboBox* cbxUpn;
    QLineEdit* leUpnSerNum;
};
