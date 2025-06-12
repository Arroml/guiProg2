#ifndef PAINT_H
#define PAINT_H
#include <QWidget>

class QPushButton;
class QComboBox;
class QLabel;
class QCheckBox;
class Canvas;

class Paint : public QWidget
{
    Q_OBJECT

public:
    Paint(QWidget *parent = 0);
    ~Paint();

private slots:
    void clearBtnPressed();
    void colorBtnPressed();
    void widgetModeChanged(int);
    void interactModeChanged(bool);

private:
    Canvas *viewport;
    QColor startColor;

    QPushButton *btnClearCanvas;
    QPushButton *btnChangeCol;
    QCheckBox *cbIMode;
    QComboBox *cobWidgetModes;
    QLabel *lblWidgetModes;
};

#endif // PAINT_H
