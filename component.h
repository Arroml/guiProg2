#ifndef COMPONENT_H
#define COMPONENT_H
#include <QColor>
#include <QPoint>
#include <QPainter>

class Component
{
protected:
    QPoint start, end;
    QColor color;
public:
    Component(QPoint start, QPoint end, QColor color = Qt::black);
    virtual ~Component();
    virtual void display(QPainter* painter) = 0;
    virtual void update(QPoint& pint, bool done = false) = 0;
    virtual Component* inside(QPoint& point) = 0;
    std::string getStartSting();
    std::string getEndString;
    QPoint getStart();

};

#endif // COMPONENT_H
