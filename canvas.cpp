#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
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
    update();
}

void Canvas::setWidgetMode(int mode)
{
    type = (Canvas::WidgetMode)mode;

    if (type != NONE) {
        if (components.empty()) {
            Component *root = nullptr;

            switch (type) {
            case BUTTON:
                break;
            case CONTAINER:
            {
                QPoint start(20,20);
                QPoint end(this->width()-20, this->height()-60);
                root = new Container(start, end, Qt::darkGray);

                break;
            }
            default:
                break;
            }

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

}

void Canvas::resizeEvent(QResizeEvent *event)
{
    QFrame::resizeEvent(event);
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QPoint currPos = event->pos();
        //qDebug() << currPos;

        dragging = true;
        lastPos = currPos;

        if (design) {
            // TODO; create new "widget" according to type

            switch (type) {
            case BUTTON:
            {
                Button* button = new Button(currPos, currPos, activeCol);
                components.push_back(button);
                break;
            }
            case CONTAINER:{
                Container*container = new Container(currPos, currPos, activeCol);
                components.push_back(container);
                break;
            }
            case NONE:{
                break;
            }
            default:
                break;
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
        }
        lastPos = currPos;

        update();
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && dragging) {
        QPoint currPos = event->pos();
        //qDebug() << currPos;

        dragging = false;

        if (design) {
            // TODO; finalize created widget
        }
        lastPos = currPos;

        update();
    }
}
