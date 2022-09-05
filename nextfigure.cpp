
#include <QPainter>
#include <QColor>
#include <QBrush>
#include <QPen>
#include <QRect>
#include "nextfigure.h"

NextFigure::NextFigure(QWidget *parent)
    : QWidget{parent}
{
    Next = nullptr;
}

//
void NextFigure::slotChangePattern(Figure *pfig)
{
    Next = pfig;
    this->update();
}

//
void NextFigure::paintEvent(QPaintEvent *event)
{
    if(Next!=nullptr)
    {
        QPainter painter(this);
        Next->paintFigure(painter);
    }
}

