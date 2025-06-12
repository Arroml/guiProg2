#ifndef BUTTON_H
#define BUTTON_H

#include "component.h"

class Button : public Component
{
public:
    Button();
    Button(QPoint start, QPoint end, QColor color = Qt::black);
    void display(QPainter* painter) override;
    void update(QPoint& point, bool done = false) override;
    Component* inside(QPoint& point) override;
};

#endif // BUTTON_H
