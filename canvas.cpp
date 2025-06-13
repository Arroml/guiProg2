#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <typeinfo>
#include "canvas.h"
#include "button.h"
#include "container.h"

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

Canvas::~Canvas()
{
}

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
    tempComponent = nullptr;
    update();
}

void Canvas::setWidgetMode(int mode)
{
    type = (Canvas::WidgetMode)mode;

    if (type != NONE) {
        if (components.empty()) {
            Component *root = nullptr;
            root = new Container(QPoint(0,0), QPoint(width(),height()), Qt::darkGray);
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
    QFrame::paintEvent(event);  // parent class draws border

    QPainter painter(this);

    // white background (inside parent's border)
    painter.fillRect(QRect(1, 1, width() - 2, height() - 2), Qt::white);

    for (Component* c : components) {
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

        dragging = true;
        lastPos = currPos;

        if (design) {
            // TODO; create new "widget" according to type

            switch (type) {
            case BUTTON:
            {
                tempComponent = new Button(currPos, currPos, Qt::black);
                break;
            }
            case CONTAINER:{
                tempComponent = new Container(currPos, currPos, Qt::black);
                break;
            }
            default:
                break;
            }
            if (tempComponent){
                for (Component *comp : components) {
                    Container *container = dynamic_cast<Container *>(comp);
                    if (container && container->inside(currPos)) {
                        container->addComponent(tempComponent);
                        qDebug() << "Komponente wurde in Container eingefügt";
                        return;
                    }
                }
                components.push_back(tempComponent);
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

        QPoint start = tempComponent->getStartPoint();
        Component * rootContainer = tempComponent->inside(start);

        dragging = false;

        if (design) {
            
            update();
        }
    }

}
