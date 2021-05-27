#pragma once

#include <QHeaderView>

class MyHeader : public QHeaderView {
    Q_OBJECT
    friend class MyTable;

public:
    MyHeader(Qt::Orientation orientation, QWidget* parent = nullptr);
    ~MyHeader();

    void setChecked(Qt::CheckState checkState);

    // QWidget interface
protected:
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const override;
    QSize sizeHint() const override;

signals:
    void checkedChanged(const std::vector<Qt::CheckState>&, Qt::Orientation);

private:
    std::vector<Qt::CheckState> m_checked;
    void togle(Qt::CheckState& state)
    {
        state = state == Qt::Checked ? Qt::Unchecked : Qt::Checked;
    }
};
