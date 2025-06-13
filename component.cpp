#include "component.h"

Component::Component(QPoint start, QPoint end, QColor color):start(start), end(end), color(color) {}

Component::~Component(){

}

std::string Component::getStartSting(){
    return std::to_string(start.x()) + " " + std::to_string(start.y());
}

std::string Component::getEndString(){
    return std::to_string(start.x()) + " " + std::to_string(start.y());
}

QPoint Component::getStartPoint(){
    return this->start;
}


