#include "button.h"

Button::Button(QPoint start, QPoint end, QColor color):Component(start,end,color) {}

void Button::display(QPainter* painter){
    painter->setBrush(color);
    painter->drawRect(QRect(start, end));
}

void Button::update(QPoint& point, bool done){
    end = point;
}

Component* Button::inside(QPoint& point){
    return QRect(start, end).contains(point) ? this : nullptr;
}
