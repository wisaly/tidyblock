#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_btnNew_clicked()
{
    int horzCnt = 6;
    int vertCnt = 6;

    qreal unitWidth = ui->frame->width() / horzCnt;
    qreal unitHeight = ui->frame->height() / vertCnt;

    foreach (QObject *o, ui->frame->children()) {
        delete o;
    }
    blocks_.clear();
    map_.clear();
    firstChoose_ = 0;
    ui->lcdScore->display(0);

    int redCnt = 0;
    int blueCnt = 0;

    for (int i = 0;i < horzCnt;i++)
    {
        for (int j = 0;j < vertCnt;j++)
        {
            QPushButton *button = new QPushButton(ui->frame);
            button->setGeometry(i * unitWidth,j * unitHeight,unitWidth,unitHeight);

            Block block;

            block.x = i;
            block.y = j;

            button->setCheckable(true);
            //button->setAutoExclusive(true);
            button->show();
            //button->setText(QString("(%1,%2)").arg(i).arg(j));

            blocks_[button] = block;
            map_[i][j] = button;

            // rand color
            Block::Color color = qrand() % 2 ? Block::Red : Block::Blue;
            if (color == Block::Red)
            {
                if (redCnt == horzCnt * vertCnt / 2)
                {
                    color = Block::Blue;
                }
                else
                {
                    redCnt++;
                }
            }
            else
            {
                if (blueCnt == horzCnt * vertCnt / 2)
                {
                    color = Block::Red;
                }
                else
                {
                    blueCnt++;
                }
            }

            // set color
            setColor(button,color);

            connect(button,SIGNAL(clicked()),SLOT(blockClicked()));
        }
    }
}

void Widget::blockClicked()
{
    QPushButton *button = static_cast<QPushButton*>(sender());

    clearText();

    if (firstChoose_ == 0)
    {
        firstChoose_ = button;
        return;
    }

    if (firstChoose_ == button)
    {
        firstChoose_ = 0;
        button->setChecked(false);
        return;
    }

    bool moved = false;
    if (blocks_[firstChoose_].x == blocks_[button].x)
    {
        int x = blocks_[button].x;
        int from = qMin(blocks_[firstChoose_].y,blocks_[button].y);
        int len = qAbs(blocks_[firstChoose_].y - blocks_[button].y);

        for (int i = 0;i <= len / 2;i++)
        {
            Block::Color color = blocks_[map_[x][from + i]].color;
            setColor(map_[x][from + i],blocks_[map_[x][from + len - i]].color);
            setColor(map_[x][from + len - i],color);
        }

        moved = true;
        drawVLine(x,from,from + len);
    }
    else if (blocks_[firstChoose_].y == blocks_[button].y)
    {
        int y = blocks_[button].y;
        int from = qMin(blocks_[firstChoose_].x,blocks_[button].x);
        int len = qAbs(blocks_[firstChoose_].x - blocks_[button].x);

        for (int i = 0;i <= len / 2;i++)
        {
            Block::Color color = blocks_[map_[from + i][y]].color;
            setColor(map_[from + i][y],blocks_[map_[from + len - i][y]].color);
            setColor(map_[from + len - i][y],color);
        }

        moved = true;
        drawHLine(y,from,from + len);
    }
    else
    {
        firstChoose_->setChecked(false);
        firstChoose_ = button;
    }

    if (moved)
    {
        firstChoose_->setChecked(false);
        button->setChecked(false);
        firstChoose_ = 0;
        ui->lcdScore->display(ui->lcdScore->intValue()+1);

        QList<Block::Color> leftPart;
        for (int i = 0;i < map_.count() / 2;i++)
        {
            for (int j = 0;j < map_[i].count();j++)
            {
                leftPart.append(blocks_[map_[i][j]].color);
            }
        }

        if ((leftPart.contains(Block::Red) && !leftPart.contains(Block::Blue))
                || (leftPart.contains(Block::Blue) && !leftPart.contains(Block::Red)))
        {
            map_[0][0]->setText("Y");
            map_[1][0]->setText("o");
            map_[2][0]->setText("u");
            map_[3][0]->setText("W");
            map_[4][0]->setText("i");
            map_[5][0]->setText("n");
        }
    }
}

void Widget::setColor(QPushButton *button, Widget::Block::Color color)
{
    blocks_[button].color = color;
    QPalette palette = button->palette();
    if (color == Block::Red)
    {
        palette.setColor(QPalette::Active,QPalette::Button,QColor(Qt::red));
    }
    else
    {
        palette.setColor(QPalette::Active,QPalette::Button,QColor(Qt::blue));
    }
    button->setPalette(palette);
}

void Widget::drawHLine(int y, int xfrom, int xto)
{
    for (int i = xfrom;i <= xto;i++)
    {
        map_[i][y]->setText("-");
    }
}

void Widget::drawVLine(int x, int yfrom, int yto)
{
    for (int i = yfrom;i <= yto;i++)
    {
        map_[x][i]->setText("|");
    }
}

void Widget::clearText()
{
    for (int i = 0;i < map_.count();i++)
    {
        for (int j = 0;j < map_.count();j++)
        {
            map_[i][j]->setText("");
        }
    }
}


