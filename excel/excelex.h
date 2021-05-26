#ifndef EXCELAX_H
#define EXCELAX_H

#include "excel.h"

class MyTableModel;
class QString;

class ExcelEx {
public:
    ExcelEx(QObject* parent);
    ~ExcelEx();

protected:
    void loadFile(const QString& fileName, MyTableModel* m_model);
    void saveFile(QString& fileName, const QString& asptNum, const QString& fio, MyTableModel* m_model);

    void printFile(const QString& fileName);

    Excel::Application* excel;
};

#endif // EXCELAX_H
