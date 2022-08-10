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
    uint m_ceil=19;
signals:

public slots:
    void slotChangePattern(Figure *pfig);

};

#endif // NEXTFIGURE_H
