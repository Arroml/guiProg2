#include <QApplication>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QColorDialog>
#include <QGridLayout>
#include <QCheckBox>
#include <QDebug>
#include "paint.h"
#include "canvas.h"

/** c'tor */
Paint::Paint(QWidget *parent)
    : QWidget(parent)
{
    // instantiate Canvas and widgets
    viewport = new Canvas();

    btnClearCanvas = new QPushButton("&Clear Canvas");

    startColor = Qt::lightGray;
    viewport->setObjColor(startColor);

    cobWidgetModes = new QComboBox();
    cobWidgetModes->addItem(tr("None"),      Canvas::NONE);
    cobWidgetModes->addItem(tr("Button"),    Canvas::BUTTON);
    cobWidgetModes->addItem(tr("Container"), Canvas::CONTAINER);

    lblWidgetModes = new QLabel("Widget Mode:");
    lblWidgetModes->setBuddy(cobWidgetModes);

    btnChangeCol = new QPushButton("&Set Color");
    btnChangeCol->setDisabled(true);
    cbIMode = new QCheckBox("&Design Mode");
    cbIMode->setChecked(true);
    cbIMode->setDisabled(true);

    // create layout and add gui elements to it
    QGridLayout *mainLayout = new QGridLayout;

    mainLayout->addWidget(viewport,       0, 0, 1, 3);
    mainLayout->addWidget(btnClearCanvas, 1, 0);
    mainLayout->addWidget(lblWidgetModes, 1, 1, Qt::AlignRight);
    mainLayout->addWidget(cobWidgetModes, 1, 2);
    mainLayout->addWidget(cbIMode,        2, 1, Qt::AlignRight);
    mainLayout->addWidget(btnChangeCol,   2, 2);

    // add layout to this widget instance
    setLayout(mainLayout);

    // connect button click event to clear canvas handler
    connect(btnClearCanvas, SIGNAL(clicked()),
            this, SLOT(clearBtnPressed()));
    // connect button click event to color chooser handler
    connect(btnChangeCol, SIGNAL(clicked()),
            this, SLOT(colorBtnPressed()));
    // connect list view to primitive changed event handler
    connect(cobWidgetModes, SIGNAL(activated(int)),
            this, SLOT(widgetModeChanged(int)));
    // connect checkbox to toggle interaction handler
    connect(cbIMode, SIGNAL(toggled(bool)),
            this, SLOT(interactModeChanged(bool)));
}

/** d'tor */
Paint::~Paint()
{
}

/** method for handling button clicked event */
void Paint::clearBtnPressed()
{
    qDebug() << "Clear image called";
    viewport->clearCanvas();
    // force redraw
    update();
}

void Paint::colorBtnPressed()
{
    QColor color(QColorDialog::getColor(startColor, this));
    if (color.isValid()) {
        qDebug() << "Color Choosen : " << color.name();
        viewport->setObjColor(color);
        update();
    }
}

void Paint::widgetModeChanged(int index)
{
    int mode = cobWidgetModes->itemData(index >= 0 ? index : 0).toInt();
    viewport->setWidgetMode(mode);
    qDebug() << "Widget Mode changed to " << mode;
    update();
}

void Paint::interactModeChanged(bool mode)
{
    viewport->setInteractionMode(mode);
    qDebug() << "Design Mode changed to " << mode;
    update();
}
