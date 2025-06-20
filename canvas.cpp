#include "canvas.h"
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <fstream>
#include <ostream>
#include "button.h"
#include "container.h"
#include "Node.h"
#include <typeinfo>
#include <filesystem>

Canvas::Canvas(QWidget *parent)
    : QFrame(parent)
{
    setFrameStyle(QFrame::Box);
    setMouseTracking(true);

    type = NONE;
    design = true;
    dragging = false;

    // TODO; init of own members
}

Canvas::~Canvas() {}

QSize Canvas::minimumSizeHint() const
{
    return QSize(200, 200);
}

QSize Canvas::sizeHint() const
{
    return QSize(640, 480);
}

void Canvas::clearCanvas(void)
{
    components.clear();

    update();
}

void Canvas::setWidgetMode(int mode)
{
    type = (Canvas::WidgetMode) mode;

    if (type != NONE) {
        if (components.empty()) {
            Component *root = nullptr;
            root = new Container(QPoint(0, 0), QPoint(width(), height()), Qt::darkGray);
            if (root != nullptr) {
                components.push_back(root);
                qDebug() << "Root wurde erstellt.";
            }
        }
    }

    update(); // neu zeichnen
}

void Canvas::setInteractionMode(bool mode)
{
    design = mode;
}

void Canvas::setObjColor(const QColor &col)
{
    activeCol = col;
}

void Canvas::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event); // parent class draws border

    QPainter painter(this);

    // white background (inside parent's border)
    painter.fillRect(QRect(1, 1, width() - 2, height() - 2), Qt::white);

    for (Component *c : components) {
        c->display(&painter);
    }

    if (tempComponent) {
        tempComponent->display(&painter);
    }
}

void Canvas::resizeEvent(QResizeEvent *event)
{
    QFrame::resizeEvent(event);
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QPoint currPos = event->pos();
        qDebug() << currPos;

        lastPos = currPos;

        if (design) {
            // TODO; create new "widget" according to type

            switch (type) {
            case BUTTON: {
                dragging = true;
                tempComponent = new Button(currPos, currPos, Qt::black);
                break;
            }
            case CONTAINER: {
                dragging = true;
                tempComponent = new Container(currPos, currPos, Qt::black);
                break;
            }
            case NONE:{
                return;
                break;
            }
            default:
                break;
            }
            if (tempComponent) {
                qDebug() << "Komponente wurde zu Canvas hinzgefügt";
            }
        }

        update();
    }
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && dragging) {
        QPoint currPos = event->pos();
        //qDebug() << currPos;

        if (design) {
            // TODO; update size of widget
            tempComponent->update(currPos);
        }
        lastPos = currPos;

        update();
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && dragging) {
        QPoint currPos = event->pos();
        tempComponent->update(currPos);
        qDebug() << "Finale Position gesetzt";
        qDebug() << currPos;




        tempComponent->setTagName(type==BUTTON? "Button" : "Container");


        tempComponent->setAttribute(" endPos" ,tempComponent->getEndString());
        tempComponent->setAttribute(" startPos" ,tempComponent->getStartString());

        QPoint start = tempComponent->getStartPoint();

        Component* rootComponent = components[0]->inside(start);

        if (!rootComponent) {
            qDebug() << "rootComponent ist nullptr";
            return;
        }

        qDebug() << "typeid: " << typeid(*rootComponent).name();  // Debug-Ausgabe

        Container* rootContainer = dynamic_cast<Container*>(rootComponent);
        if (!rootContainer) {
            qDebug() << "Cast fehlgeschlagen – kein Container!";
            return;
        }
        Component * bufferComp = tempComponent;


        if (rootComponent != nullptr){
            rootContainer->addComponent(bufferComp);
            rootContainer->addChild(bufferComp);
        }else{
        }

        dragging = false;

        if (design) {
            update();
        }
        tempComponent = nullptr;
    }
}

void Canvas::writeToXmlFile(std::string fileName){

    std::ofstream of ("../../../../../output/"+fileName);
    if (!of.is_open()){

        qDebug() << "File konnte nicht geööffnet werden";
        return;
    }

    components[0]->printNodeInFile(of,0,Node::XML);
}
