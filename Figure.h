// This class describe figure. Figure consist on three vertical random colors cells
// Data about position of figure store in param m_indxH and m_indxW
// Data about colors of figure store in array[3]

#ifndef FIGURE_H
#define FIGURE_H

#include <Qt>
#include <QPainter>

class Figure
{
    QColor m_array[3];
    uint m_indxH; // horizontal coordinate
    uint m_indxW; // vertical coordinate of head
    uint m_cell; // size of cell
public:
    Figure(uint cell);
    void stepDown(); // move down one step
    void stepLeft(); // move left one step
    void stepRight(); // move right one step
    void setIndxH(uint); // setter
    void setIndxW(uint);
    uint indxH(); // getter
    uint indxW();
    QColor color(uint);
    void rotateColors(Qt::Key); // rotate colors in figure up or down depend on push button
    void makeRandomColors(); // set random collors for cells from yellow, green, red, blue
    void paintFigure(QPainter& painter); // draw figure
};

#endif // FIGURE_H

