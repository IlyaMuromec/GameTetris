// This class is auxiliary and releted with class figure
// Goal this class is display in window next figure

#ifndef NEXTFIGURE_H
#define NEXTFIGURE_H

#include <QWidget>
#include "Figure.h"

class NextFigure : public QWidget
{
    Q_OBJECT
public:
    explicit NextFigure(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);

private:
    Figure * Next;
    uint m_cell=19; // size of cell
signals:

public slots:
    void slotChangePattern(Figure *pfig);

};

#endif // NEXTFIGURE_H
