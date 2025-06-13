#ifndef CANVAS_H
#define CANVAS_H
#include <QColor>
#include <QFrame>
#include <QPoint>
#include "Node.h"
#include "component.h"
#include "container.h"
#include <vector>

class Canvas : public QFrame
{
    Q_OBJECT

public:
    enum WidgetMode { NONE, BUTTON, CONTAINER };

    Canvas(QWidget *parent = 0);
    ~Canvas();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void clearCanvas(void);
    void setWidgetMode(int mode);
    void setInteractionMode(bool mode);
    void setObjColor(const QColor &col);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    bool dragging;
    QPoint lastPos;
    QColor activeCol;
    bool design;
    WidgetMode type;
    std::vector<Component *> components;
    Component *tempComponent = nullptr;
};

#endif // CANVAS_H
