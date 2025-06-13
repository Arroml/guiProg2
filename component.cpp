#include "component.h"

Component::Component(QPoint start, QPoint end, QColor color)
    : start(start)
    , end(end)
    , color(color)
    ,node("")
{}

Component::~Component() {}

std::string Component::getStartString()
{
    return std::to_string(start.x()) + " " + std::to_string(start.y());
}

std::string Component::getEndString()
{
    return std::to_string(start.x()) + " " + std::to_string(start.y());
}

QPoint Component::getStartPoint()
{
    return this->start;
}

void Component::setNode(Node node){
    this->node = node;
}
