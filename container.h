#ifndef CONTAINER_H
#define CONTAINER_H
#include <vector>

#include "component.h"

class Container : public Component
{
private:
    std::vector<Component*> components;
public:
    Container(QPoint start, QPoint end, QColor color  = Qt::black);
    void addComponent(Component* component);
    void display(QPainter* painter)override;
    void update(QPoint& point, bool done = false)override;
    Component * inside(QPoint& point) override;
    ~Container();
};

#endif // CONTAINER_H
