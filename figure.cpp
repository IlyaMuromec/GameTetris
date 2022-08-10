#include "figure.h"
#include <QRandomGenerator>
#include <QColor>
#include <QPainter>
#include <QRect>
#include <QBrush>

Figure::Figure(uint ceil)
{
    m_ceil = ceil;
    m_indxW=0;
    m_indxH=0;
    makeRandomColors();
}

void Figure::stepDown()
{
    m_indxH++;
}

void Figure::stepRight()
{
    m_indxW++;
}

void Figure::stepLeft()
{
    m_indxW--;
}

void Figure::setIndxH(uint indxH)
{
    m_indxH=indxH;
}

void Figure::setIndxW(uint indxW)
{
    m_indxW=indxW;
}

uint Figure::indxH()
{
    return m_indxH;
}

uint Figure::indxW()
{
    return m_indxW;
}


QColor Figure::color(uint indx)
{
    return m_array[indx];
}

void Figure::rotateColors(Qt::Key key)
{
    if (key == Qt::Key_Down)
    {
        QColor tmp = m_array[0];
        m_array[0]=m_array[1];
        m_array[1]=m_array[2];
        m_array[2]=tmp;
    }
    else if (key == Qt::Key_Up)
    {
        QColor tmp = m_array[2];
        m_array[2]=m_array[1];
        m_array[1]=m_array[0];
        m_array[0]=tmp;
    }
    else {}
}

void Figure::makeRandomColors()
{
    uint indx;
    static QColor colors[4] = {Qt::yellow, Qt::green, Qt::red, Qt::blue };
    for(uint i=0; i<3; i++)
    {
         indx = QRandomGenerator::global()->generate()%4;
         m_array[i]=colors[indx];
    }
}

void Figure::paintFigure(QPainter& painter)
{
    painter.setPen(Qt::NoPen);
    for(uint j=0; j<3; j++ )
    {
        painter.setBrush(this->color(j));
        painter.drawRect(0, j*20, m_ceil, m_ceil);
    }
}
