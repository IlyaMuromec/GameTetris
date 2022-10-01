// This class describe glass. Glass is game field which consists on grid of cells
// Figure with color cells fells down into glass and takes free place. Thus glass is filling color cells.
// Timer defines frequency of steps.
// If three or more cell in row or column have same color, then they are deleted
// The game finish if figures full fill glass at least in one column

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

    uint rows() const; // getter
    void setRows(uint newRows); // setter

    unsigned int columns() const; // number of columns in glass
    void setColumns(unsigned int newColumns); //setter

    void glassInit(void); // create 2D-array
    void clearGlass(void); // fill array by empty cells
    void handleNewFigure(void); // save new cells, delete redundant cells, swap Figure, update score, redraw next figure
    void acceptColors(void); //save cells colors of figure in the glass
    void deleterHor(uint row, uint startHor, uint finishHor); // Delete three or more cells on row
    void deleterVer(uint column, uint startVer, uint finishVer); // Delete three or more cells on column
    bool findHor(void); // Analysis, are there three or more cells of the same color on row?
    bool findVer(void); // Analysis, are there three or more cells of the same color on column?
    void swapFigure(Figure* cur, Figure* next); // Set new next figure in glass
    virtual void paintEvent(QPaintEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void timerEvent(QTimerEvent *event); // check finish of game and acceptColors()
    ~Glass();

signals:
    signalChangePattern(Figure*); // initiate redraw new next figure
    signalSetScore(int); // set new score
public slots:
    void mySlotGame(); // start game after push button

private:
    uint m_rows; // number of rows in glass
    uint m_columns; // number of columns in glass
    const uint m_cell=19; // size of cell
    uint m_wGlass; // height of glass
    uint m_hGlass; // width og glass
    uint m_score; // score of game
    bool m_gameOn; // game continue flag
    int m_timerinterval; // delay between steps
    QVector<QVector<QColor>> m_glassArray; // create vector of vectors as a 2D-array
    Figure *cur; // pointer on current figure
    Figure *next; // pointer on next figure
    int m_idTimer;
};

#endif // GLASS_H
