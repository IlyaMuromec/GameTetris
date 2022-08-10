#ifndef GLASS_H
#define GLASS_H


#include <QWidget>
#include <QVector>
#include <QColor>
#include "Figure.h"
#include "NextFigure.h"

#define emptyCell QColor(150,150,150)

class Glass : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(uint rows READ rows WRITE setRows)
    Q_PROPERTY(unsigned int columns READ columns WRITE setColumns)
public:
    explicit Glass(QWidget *parent = nullptr);

    uint rows() const;
    void setRows(uint newRows);

    unsigned int columns() const;
    void setColumns(unsigned int newColumns);

    void glassInit(void);
    void clearGlass(void);
    void acceptColors();
    void deleterHor(uint row, uint startHor, uint finishHor);
    void deleterVer(uint column, uint startVer, uint finishVer);
    bool findHor(void);
    bool findVer(void);
    void swapFigure(Figure* cur, Figure* next);
    virtual void paintEvent(QPaintEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void timerEvent(QTimerEvent *event);
    ~Glass();

signals:
    signalChangePattern(Figure*);
    signalSetScore(int);
public slots:
    void mySlotGame();

private:
    uint m_rows;
    uint m_columns;
    const uint m_ceil=19; // size of cell
    uint m_wGlass;
    uint m_hGlass;
    uint m_score;
    bool m_gameOn;
    int m_timerinterval;
    QVector<QVector<QColor>> m_glassArray;
    Figure *cur;
    Figure *next;
    int m_idTimer;
};

#endif // GLASS_H
