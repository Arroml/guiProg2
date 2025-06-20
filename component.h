#ifndef COMPONENT_H
#define COMPONENT_H
#include <QColor>
#include <QPainter>
#include <QPoint>
#include "Node.h"

class Component:public Node
{
protected:
    QPoint start, end;
    QColor color;

public:
    Component(QPoint start, QPoint end, QColor color = Qt::black);
    virtual ~Component();
    virtual void display(QPainter *painter) = 0;
    virtual void update(QPoint &pint, bool done = false) = 0;
    virtual Component *inside(QPoint &point) = 0;
    std::string getStartString();
    std::string getEndString();
    QPoint getStartPoint();
    void setNode(Node node);
    Node getNode();

    void setStart(QPoint &point);
    void setEnd(QPoint &point);

};

#endif // COMPONENT_H
