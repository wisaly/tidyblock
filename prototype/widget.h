#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMap>
#include <QPushButton>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

    struct Block
    {
        enum Color{Blue,Red};
        Color color;
        int x;
        int y;
    };

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void on_btnNew_clicked();
    void blockClicked();

private:
    void setColor(QPushButton *button,Block::Color color);
    void drawHLine(int y,int xfrom,int xto);
    void drawVLine(int x,int yfrom,int yto);
    void clearText();

    Ui::Widget *ui;

    QMap<QPushButton*,Block> blocks_;
    QMap<int,QMap<int,QPushButton*> > map_;

    QPushButton *firstChoose_;
};

#endif // WIDGET_H
