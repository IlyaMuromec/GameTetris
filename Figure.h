#ifndef FIGURE_H
#define FIGURE_H

#include <Qt>
#include <QPainter>

class Figure
{
    QColor m_array[3];
    uint m_indxH;
    uint m_indxW;
    uint m_ceil;
public:
    Figure(uint ceil);
    void stepDown();
    void stepLeft();
    void stepRight();
    void setIndxH(uint);
    void setIndxW(uint);
    uint indxH();
    uint indxW();
    QColor color(uint);
    void rotateColors(Qt::Key);
    void makeRandomColors();
    void paintFigure(QPainter& painter);
};

#endif // FIGURE_H

