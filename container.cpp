#include "container.h"
#include <QDebug>

Container::Container(QPoint start, QPoint end, QColor color)
    : Component(start, end, color)
{}

Container::~Container()
{
    for (Component *i : components) {
        delete i;
    }
}

void Container::addComponent(Component *component)
{
    this->components.push_back(component);
}

void Container::display(QPainter *painter)
{
    QRect rect = QRect(start, end).normalized();
    painter->setPen(Qt::gray);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(rect);
    for (Component *i : components) {
        i->display(painter);
    }
}

void Container::update(QPoint &point, bool done)
{
    end = point;
}

Component *Container::inside(QPoint &point)
{
    for (Component* child : components) {
        Component* found = child->inside(point);
        if (found != nullptr) {
            return found;
        }
    }

    if (QRect(start, end).contains(point)) {
        return this;
    }
    return nullptr;
}
