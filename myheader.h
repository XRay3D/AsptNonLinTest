#pragma once


#include <QHeaderView>

class MyHeader : public QHeaderView {
    Q_OBJECT
    friend class MyTable;

public:
    MyHeader(Qt::Orientation orientation, QWidget* parent = nullptr);
    ~MyHeader();

    void setChecked(bool checked);

    // QWidget interface
protected:
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const override;
    QSize sizeHint() const override;

signals:
    void checkedChanged(const QVector<bool>&, int);

private:
    QVector<bool> m_checked;
};


