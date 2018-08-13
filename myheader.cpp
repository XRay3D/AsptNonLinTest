#include "myheader.h"

#include "tableparams.h"
#include <QPainter>
#include <QSettings>
#include <QTimer>
#include <qevent.h>

MyHeader::MyHeader(Qt::Orientation orientation, QWidget* parent)
    : QHeaderView(orientation, parent)
    , m_checked(16, false)
{
    QSettings settings;
    int size = settings.beginReadArray("MyHeader" + QString::number(orientation));
    for (int i = 0, key = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        key = settings.value("key").toInt();
        if (key > -1)
            m_checked[key] = settings.value("value").toBool();
    }
    settings.endArray();
    //Отправить сигнал после установки всех соединений.
    QTimer::singleShot(1, Qt::CoarseTimer, [=] { emit checkedChanged(m_checked, orientation); });
    if (orientation == Qt::Vertical)
        setSectionResizeMode(QHeaderView::ResizeToContents);
}

MyHeader::~MyHeader()
{
    QSettings settings;
    settings.beginWriteArray("MyHeader" + QString::number(orientation()));
    for (int i = 0; i < m_checked.size(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue("key", i);
        settings.setValue("value", m_checked[i]);
    }
    settings.endArray();
}

void MyHeader::setChecked(bool checked)
{
    for (int index = 0; index < m_checked.size(); ++index) {
        m_checked[index] = checked;
        updateSection(index);
    }
    emit checkedChanged(m_checked, orientation());
}

void MyHeader::mouseMoveEvent(QMouseEvent* event)
{
    if (orientation() == Qt::Horizontal)
        return;
    static int index = 0;
    if (index != logicalIndexAt(event->pos())) {
        index = logicalIndexAt(event->pos());
        if (index < 0)
            return;
        if (event->buttons() & Qt::LeftButton) {
            m_checked[index] = !m_checked[index];
            updateSection(index);
            emit checkedChanged(m_checked, orientation());
        }
    }
    QHeaderView::mouseMoveEvent(event);
}

void MyHeader::mousePressEvent(QMouseEvent* event)
{
    int index = logicalIndexAt(event->pos());
    if (index < 0)
        return;
    if (orientation() == Qt::Horizontal) {
        for (int var = 0; var < count(); ++var)
            m_checked[var] = false;
        m_checked[index] = true;
        for (int var = 0; var < count(); ++var)
            updateSection(var);
    } else {
        m_checked[index] = !m_checked[index];
        updateSection(index);
    }

    emit checkedChanged(m_checked, orientation());

    QHeaderView::mousePressEvent(event);
}

void MyHeader::paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const
{
    painter->save();
    QHeaderView::paintSection(painter, rect, logicalIndex);
    painter->restore();

    QStyleOptionButton option;
    option.rect = QRect(rect.left() + 4, (rect.height() - 16) / 2 + rect.top(), 16, 16);

    m_checked[logicalIndex] ? option.state = QStyle::State_On : option.state = QStyle::State_Off;
    option.state |= (isEnabled() ? QStyle::State_Enabled : QStyle::State_None);
    if (orientation() == Qt::Horizontal)
        style()->drawPrimitive(QStyle::PE_IndicatorRadioButton, &option, painter);
    else
        style()->drawPrimitive(QStyle::PE_IndicatorCheckBox, &option, painter);
}

QSize MyHeader::sizeHint() const
{
    return QSize(50, 50);
}
