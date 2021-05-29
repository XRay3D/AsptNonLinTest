#include "myexcel.h"
#include "excel.h"
#include "measuremodel.h"

#include <QApplication>
#include <QDebug>
#include <QFileInfo>

const auto ADC_CH0 = QStringLiteral("D6:D53");
const auto ADC_CH1 = QStringLiteral("G6:G53");

const auto DATA_BEGIN = QStringLiteral("D55");
const auto DATA_END = QStringLiteral("D56");

const auto FIO = QStringLiteral("D57");
const auto ASPT_SER_NUM = QStringLiteral("E2");

MyExcel::MyExcel() {
    system("taskkill /f /im excel.exe");
    excel = new Excel::Application;
    excel->SetVisible(true);
}

MyExcel::~MyExcel() {
    closeAll();
    excel->Quit();
}

void MyExcel::closeAll() const {
    while(excel->Workbooks()->Count())
        excel->ActiveWorkbook()->Close(false);
}

void MyExcel::loadFile(const QString& fileName, MeasureModel* m_model) {
    closeAll();
    excel->Workbooks()->Open(fileName);
    if(excel->Workbooks()->Count()) {
        auto read = [m_model, this](int ch) {
            auto values = excel->Range(ch ? ADC_CH0 : ADC_CH1)->Value().value<QVariantList>();
            for(int row = 0; row < RowCount; ++row)
                for(int i = 0; i < 3; ++i)
                    m_model->addData(row, i + ch * 3, values[row * 3 + i].value<QVariantList>().front().toDouble());
        };
        read(0);
        read(1);
        m_model->save();
        excel->ActiveWorkbook()->Close(false);
    }
}

void MyExcel::saveFile(const QString& fileName, const QString& asptNum, const QString& fio, MeasureModel* m_model) {
    closeAll();
    const bool newFile = !QFileInfo::exists(fileName.trimmed());
    excel->Workbooks()->Open(newFile ? qApp->applicationDirPath() + "/blank.xlsx"
                                     : fileName.trimmed());

    if(excel->Workbooks()->Count()) {
        if(newFile)
            excel->Range(DATA_BEGIN)->SetValue(QDateTime::currentDateTime());
        excel->Range(DATA_END)->SetValue(QDateTime::currentDateTime());
        excel->Range(FIO)->SetValue(fio);
        excel->Range(ASPT_SER_NUM)->SetValue(asptNum);

        auto write = [m_model, this](int ch) {
            QVariantList values;
            for(int row = 0; row < RowCount; ++row)
                for(int i = 0; i < 3; ++i)
                    values.append(QVariant(QVariantList{m_model->getData(row).average(i + ch * 3)}));
            excel->Range(ch ? ADC_CH0 : ADC_CH1)->SetValue(values);
        };
        write(0);
        write(2);

        if(newFile) {
            excel->ActiveWorkbook()->SaveCopyAs(fileName.trimmed());
            excel->ActiveWorkbook()->Close(false);
        } else
            excel->ActiveWorkbook()->Close(true);
    }
}

void MyExcel::printFile(const QString& fileName) {
    closeAll();
    excel->Workbooks()->Open(fileName);
    if(excel->Workbooks()->Count()) {
        Excel::_Worksheet(excel->ActiveSheet()).PrintOut(1, 1, 1);
        closeAll();
    }
}
