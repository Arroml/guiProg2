#include "button.h"

Button::Button(QPoint start, QPoint end, QColor color):Component(start,end,color) {}

void Button::display(QPainter* painter){
    QRect rect = QRect(start, end).normalized();
    painter->setPen(Qt::black);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(rect);
    painter->drawText(rect.adjusted(5, 5, -5, -5), Qt::AlignLeft | Qt::AlignTop, "Button");
}

void Button::update(QPoint& point, bool done){
    end = point;
}

Component* Button::inside(QPoint& point){
    return QRect(start, end).contains(point) ? this : nullptr;
}
