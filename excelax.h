#ifndef EXCELAX_H
#define EXCELAX_H

class MyTableModel;
class QString;

class ExcelAx {
public:
    ExcelAx();

protected:
    void loadFile(const QString& fileName, MyTableModel* m_model);
    void saveFile(const QString& fileName, const QString& asptNum, const QString& fio, MyTableModel* m_model);

    void printFile(const QString& fileName);
};

#endif // EXCELAX_H
