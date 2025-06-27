#include <QMessageBox>
#include "canvas.h"
#include "Node.h"
#include "button.h"
#include "container.h"
#include <QDebug>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QMouseEvent>
#include <QPainter>
#include <filesystem>
#include <fstream>
#include <ostream>
#include <qfile.h>
#include <sstream>
#include <typeinfo>

Canvas::Canvas(QWidget *parent) : QFrame(parent) {
    setFrameStyle(QFrame::Box);
    setMouseTracking(true);

    type = NONE;
    design = true;
    dragging = false;

    // TODO; init of own members
}

Canvas::~Canvas() {}

QSize Canvas::minimumSizeHint() const { return QSize(200, 200); }

QSize Canvas::sizeHint() const { return QSize(640, 480); }

void Canvas::clearCanvas(void) {
    components.clear();

    update();
}

void Canvas::setWidgetMode(int mode) {
    type = (Canvas::WidgetMode)mode;

    if (type != NONE) {
        if (components.empty()) {
            Component *root = nullptr;
            root = new Container(QPoint(0, 0), QPoint(width(), height()), Qt::white);
            if (root != nullptr) {
                components.push_back(root);
                qDebug() << "Root wurde erstellt.";
            }
        }
    }

    update(); // neu zeichnen
}

void Canvas::setInteractionMode(bool mode) { design = mode; }

void Canvas::setObjColor(const QColor &col) { activeCol = col; }

void Canvas::paintEvent(QPaintEvent *event) {
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

void Canvas::resizeEvent(QResizeEvent *event) { QFrame::resizeEvent(event); }

void Canvas::mousePressEvent(QMouseEvent *event) {
    if (design == true) {
        if (event->button() == Qt::LeftButton) {
            QPoint currPos = event->pos();
            qDebug() << currPos;

            lastPos = currPos;

            if (design) {
                // TODO; create new "widget" according to type

                switch (type) {
                case BUTTON: {
                    dragging = true;
                    tempComponent = new Button(currPos, currPos, activeCol);
                    break;
                }
                case CONTAINER: {
                    dragging = true;
                    tempComponent = new Container(currPos, currPos, activeCol);
                    break;
                }
                case NONE: {
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
}

void Canvas::mouseMoveEvent(QMouseEvent *event) {
    if (design == true) {

        if ((event->buttons() & Qt::LeftButton) && dragging) {
            QPoint currPos = event->pos();
            // qDebug() << currPos;

            if (design) {
                // TODO; update size of widget
                tempComponent->update(currPos);
            }
            lastPos = currPos;

            update();
        }
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *event) {
    if (design == true) {

        if (event->button() == Qt::LeftButton && dragging) {
            QPoint currPos = event->pos();
            tempComponent->update(currPos);
            qDebug() << "Finale Position gesetzt";
            qDebug() << currPos;

            tempComponent->setTagName(type == BUTTON ? "Button" : "Container");

            tempComponent->setAttribute(" endPos", tempComponent->getEndString());
            tempComponent->setAttribute(" startPos", tempComponent->getStartString());

            QColor color = tempComponent->getColor();
            tempComponent->setAttribute(" fillColor", std::to_string(color.rgb()));

            QPoint start = tempComponent->getStartPoint();

            Component *rootComponent = components[0]->inside(start);

            if (!rootComponent) {
                qDebug() << "rootComponent ist nullptr";
                return;
            }

            Container *rootContainer = dynamic_cast<Container *>(rootComponent);
            if (!rootContainer) {
                qDebug() << "Cast fehlgeschlagen – kein Container!";
                return;
            }

            Component *bufferComp = tempComponent;

            if (check_if_smal(bufferComp)) {
                delete bufferComp;
                tempComponent = nullptr;
                update();
                return;
            }

            if (rootComponent != nullptr) {
                rootContainer->addComponent(bufferComp);
                rootContainer->addChild(bufferComp);
                components.push_back(bufferComp);
                if (type == BUTTON){
                    qDebug() << "neuer Button in listener";
                    Button* button = dynamic_cast<Button*>(bufferComp);
                    button->setText("Button " + QString::number(buttonZahl));
                    buttonZahl++;

                    addEventLinsterer(button);
                }
            } else {
            }

            dragging = false;

            if (design) {
                update();
            }
            tempComponent = nullptr;
        }

    }else if (design == false){

        qDebug() << "im Design Mode";

        QPoint clickPos = event->pos();

        for (Component *c : components) {
            if (Button *btn = dynamic_cast<Button *>(c)) {
                if (btn->inside(clickPos)) {
                    btn->notify();
                    return;
                }
            }
        }
    }
}

bool Canvas::check_if_smal(Component *comp) {
    int minSize = 10;
    QPoint start = tempComponent->getStartPoint();
    QPoint end = tempComponent->getEndPoint();
    int widhth,height;

    if(start.x()<end.x()){
        widhth = end.x() - start.x();
    }
    else {
        widhth = start.x() - end.x();
    }
    if(start.y()<end.y()){
        height = end.y() - start.y();
    }
    else {
        height = start.y() - end.y();
    }

    if (widhth < minSize || height < minSize) {
        return true;
    }
    return false;
}

void Canvas::writeToXmlFile(std::string fileName) {

    std::ofstream of("../../../../gitPr1/guiProg2/output/" + fileName); // only for me because stupid qt or stupid me LG.AlexA
    if (!of.is_open()) {

        qDebug() << "File konnte nicht geöffnet werden";
        return;
    }

    components[0]->printNodeInFile(of, 0, Node::XML);
}

void Canvas::loadFile() {

    // Root erstellen
    if (components.empty()) {
        Component *root = nullptr;
        root = new Container(QPoint(0, 0), QPoint(width(), height()), Qt::white);
        if (root != nullptr) {
            components.push_back(root);
            qDebug() << "Root wurde erstellt.";
        }
    }

    QString fileName = QFileDialog::getOpenFileName(
        this, "Datei Laden", "", "XML (*.xml);;Json (*.json);;CSV (*.csv)");
    if (fileName.isEmpty())
        return;
    qDebug() << fileName;

    std::ifstream ifs(fileName.toStdString());
    if (!ifs.is_open()) {
        qDebug() << "File konnte nicht geööfnet werden";
        return;
    }

    std::string line;

    while (std::getline(ifs, line)) {

        if (line.find("</Container>") != std::string::npos) {
            qDebug() << "end Tag von Container";
            continue;
        }

        int x1, x2, y1, y2, color;

        int posStart = line.find("startPos=\"");
        if (posStart != std::string::npos) {
            posStart += 10;
            int endquote = line.find("\"", posStart);
            std::string inhalt = line.substr(posStart, endquote - posStart);
            std::istringstream iss(inhalt);
            iss >> x1 >> y1;
        }

        QPoint start(x1, y1);

        int posEnd = line.find("endPos=\"");
        if (posEnd != std::string::npos) {
            posEnd += 8;
            int endquote = line.find("\"", posEnd);
            std::string inhalt = line.substr(posEnd, endquote - posEnd);
            std::istringstream iss(inhalt);
            iss >> x2 >> y2;
        }

        QPoint end(x2, y2);

        int posColor = line.find("fillColor=\"");
        if (posColor != std::string::npos) {
            posColor += 11;
            int endquote = line.find("\"", posColor);
            std::string inhalt = line.substr(posColor, endquote - posColor);
            std::istringstream iss(inhalt);
            iss >> color;
        }
        QColor fillColor = QColor::fromRgb(color);

        if (line.find("Container") != std::string::npos) {

            qDebug() << "in Container";
            Container *container = new Container(start, end, fillColor);
            container->setTagName("Container");
            container->setAttribute(" endPos", container->getEndString());
            container->setAttribute(" startPos", container->getStartString());
            QColor color = container->getColor();
            container->setAttribute(" fillColor", std::to_string(color.rgb()));

            Component *rootComponent = components[0]->inside(start);

            if (!rootComponent) {
                qDebug() << "rootComponent ist nullptr";
                return;
            }

            Container *rootContainer = dynamic_cast<Container *>(rootComponent);
            if (!rootContainer) {
                qDebug() << "Cast fehlgeschlagen – kein Container!";
                continue;
            }

            if (rootComponent != nullptr) {
                rootContainer->addComponent(container);
                rootContainer->addChild(container);
                components.push_back(container);
            }

        } else if (line.find("Button") != std::string::npos) {
            qDebug() << "in Button";

            Button *button = new Button(start, end, fillColor);
            button->setTagName("Container");
            button->setAttribute(" endPos", button->getEndString());
            button->setAttribute(" startPos", button->getStartString());
            QColor color = button->getColor();
            button->setAttribute(" fillColor", std::to_string(color.rgb()));
            button->setText("Button " + QString::number(buttonZahl));
            buttonZahl++;

            Component *rootComponent = components[0]->inside(start);

            if (!rootComponent) {
                qDebug() << "rootComponent ist nullptr";
                return;
            }

            Container *rootContainer = dynamic_cast<Container *>(rootComponent);

            if (!rootContainer) {
                qDebug() << "Cast fehlgeschlagen ";
                return;
            }

            if (rootComponent != nullptr) {
                rootContainer->addComponent(button);
                rootContainer->addChild(button);
                components.push_back(button);
                this->addEventLinsterer(button);
            }

        } else {
            qDebug() << "Weder Button noch Container";
            continue;
        }
        update();
    }


}

void Canvas::addEventLinsterer(Button *btn) {
    qDebug() << "im Event Listener";
    if (btn) {
        btn->attach([btn]() {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Information");
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setText(btn->getText() + " wurde geklickt.");
            msgBox.exec();
        });
    }
}
