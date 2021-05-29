#pragma once

class MeasureModel;
class QString;
namespace Excel {
class Application;
}

class MyExcel {
    Excel::Application* excel{};
    void closeAll() const;

public:
    MyExcel();
    ~MyExcel();

    void loadFile(const QString& fileName, MeasureModel* m_model);
    void saveFile(const QString& fileName, const QString& asptNum, const QString& fio, MeasureModel* m_model);
    void printFile(const QString& fileName);
};
