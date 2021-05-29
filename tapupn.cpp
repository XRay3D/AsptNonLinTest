#include "tapupn.h"
#include "measuring_interface/mi.h"
#include "ui_tapupn.h"

#include <QComboBox>
#include <QLineEdit>
#include <QMessageBox>

TapUpn::TapUpn(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::TapUpn) //
{
    ui->setupUi(this);

    setEnabled(false);

    connect(this, &TapUpn::setResistor, MI::upn(), &Upn::setResistor);

    connect(ui->pbUpnR1Ch0, &QPushButton::clicked, [this] { emit setResistor(0); });
    connect(ui->pbUpnR1Ch1, &QPushButton::clicked, [this] { emit setResistor(3); });
    connect(ui->pbUpnR2Ch0, &QPushButton::clicked, [this] { emit setResistor(1); });
    connect(ui->pbUpnR2Ch1, &QPushButton::clicked, [this] { emit setResistor(4); });
    connect(ui->pbUpnR3Ch0, &QPushButton::clicked, [this] { emit setResistor(2); });
    connect(ui->pbUpnR3Ch1, &QPushButton::clicked, [this] { emit setResistor(5); });
}

TapUpn::~TapUpn() { delete ui; }

void TapUpn::update() {
    QVector<double> r(cbxUpn->currentData().value<QVector<double>>());
    if(r.size() == 6) {
        leUpnSerNum->setText(QString("%1").arg(r[5]));
        ui->dsbxUpnR1->setValue(r[0]);
        ui->dsbxUpnR2->setValue(r[1]);
        ui->dsbxUpnR3->setValue(r[2]);
        ui->sbxUpnSerNum->setValue(static_cast<int>(r[5]));
        setEnabled(true);
        //        ui->tableView->model()->setResistorsValue(ui->cbxUpn->currentData().value<QVector<double>>());
    } else {
        leUpnSerNum->clear();
        ui->dsbxUpnR1->setValue(0.0);
        ui->dsbxUpnR2->setValue(0.0);
        ui->dsbxUpnR3->setValue(0.0);
        ui->sbxUpnSerNum->setValue(0);
        setEnabled(false);
    }
}

void TapUpn::on_pbUpnRead_clicked() {
    MI::upn()->ping(cbxUpn->currentText());
    if(MI::upn()->readResistorValue()) {
        ui->dsbxUpnR1->setValue(MI::upn()->resistors()[0]);
        ui->dsbxUpnR2->setValue(MI::upn()->resistors()[1]);
        ui->dsbxUpnR3->setValue(MI::upn()->resistors()[2]);
        ui->sbxUpnSerNum->setValue(static_cast<int>(MI::upn()->resistors()[5]));
        QMessageBox::information(
            this, tr("УПН"), tr("Значения из УПН успешно считаны!"), "Хоошо :-)");
    } else {
        QMessageBox::critical(this, tr("УПН"), tr("Не удалось прочитать значения из УПН!"), "Плохо :-(");
    }
}

void TapUpn::on_pbUpnWrite_clicked() {
    MI::upn()->ping(cbxUpn->currentText());
    if(QMessageBox::question(this, tr("УПН"), tr("Вы действительно хотите записать новые значения в УПН?"), "Да", "Нет", "", 1, 1) == 0) {
        if(MI::upn()->writeResistorValue({
               ui->dsbxUpnR1->value(),
               ui->dsbxUpnR2->value(),
               ui->dsbxUpnR3->value(),
               0.0,
               0.0,
               double(ui->sbxUpnSerNum->value()),
           })) {
            QMessageBox::information(this, tr("УПН"), tr("Новые значения в УПН успешно записаны!"), "Хоошо :-)");
            cbxUpn->setItemData(cbxUpn->currentIndex(), QVariant::fromValue(MI::upn()->resistors()));
            //            ui->tableView->model()->setResistorsValue(ui->cbxUpn->currentData().value<QVector<double>>());
            leUpnSerNum->setText(ui->sbxUpnSerNum->text());
        } else {
            QMessageBox::critical(this, tr("УПН"), tr("Не удалось записать новые значения в УПН!"), "Плохо :-(");
        }
    }
}

void TapUpn::setUiWidgets(QComboBox* _cbxUpn, QLineEdit* _leUpnSerNum) {
    cbxUpn = _cbxUpn;
    leUpnSerNum = _leUpnSerNum;
}
