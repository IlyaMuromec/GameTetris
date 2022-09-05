#include "glass.h"
#include <QPainter>
#include <QBrush>
#include <QKeyEvent>


Glass::Glass(QWidget *parent)
    : QWidget{parent}
{
    m_timerinterval=200; // delay between steps
    cur = new Figure(m_cell);
    next = new Figure(m_cell);
    m_gameOn = false;
    m_score = 0;
}

Glass::~Glass()
{
    delete cur;
    delete next;
}

// getters
uint Glass::rows() const
{
    return m_rows;
}

unsigned int Glass::columns() const
{
    return m_columns;
}

// setters
void Glass::setRows(uint newRows)
{
    m_rows = newRows;
}

void Glass::setColumns(unsigned int newColumns)
{
    m_columns = newColumns;
}

void Glass::glassInit(void)
{
    // set number of rows in 2D-array
    m_glassArray.resize(m_rows);
    // set number of columns in 2D-array
    for (uint i=0; i<m_rows;i++)
    {
        m_glassArray[i].resize(m_columns);
    }
    this->clearGlass();
    m_wGlass= m_columns * (m_cell+1); // define hight of glass
    m_hGlass= m_rows * (m_cell+1); // define wight of glass
    this->setFixedSize(QSize(m_wGlass, m_hGlass));
    m_score=0;
}

// сlean glass
void Glass::clearGlass(void)
{
    for (uint i=0; i<m_rows; i++)
    {
        for (uint j=0; j<m_columns; j++)
        {
            m_glassArray[i][j]=emptyCell;
        }
    }
}

// Paint glass
void Glass::paintEvent(QPaintEvent*event)
{
    QPainter painter(this);

    painter.setBrush(emptyCell);
    painter.setPen(Qt::NoPen);

    // paint glass depend on values of 2D-array
    for (uint i=0; i<m_rows;i++)
    {
        for (uint j=0; j<m_columns;j++)
        {
            painter.setBrush(m_glassArray[i][j]);
            painter.drawRect(j*20, i*20, m_cell, m_cell);
        }
    }

    // add figure
    if(m_gameOn)
    {
        for(uint j=0; j<3; j++ )
        {
            painter.setBrush(cur->color(j));
            painter.drawRect(cur->indxW()*20, (cur->indxH()-j)*20, m_cell, m_cell);
        }
    }
}

// initiate and start game
void Glass::mySlotGame()
{
    clearGlass();
    glassInit();
    cur->setIndxH(0); // start position in vertical is head of figure in the first row
    cur->setIndxW((this->m_columns)>>1); // stat position in horizontal is mid glass
    next->setIndxH(0);
    next->setIndxW(0);
    emit signalChangePattern(next); //

    if(m_gameOn==true)
    {
        qDebug()<<"Kill timer"<<m_idTimer;
        this->killTimer(m_idTimer);
    }
    m_idTimer=this->startTimer(m_timerinterval);
    qDebug()<<"Start timer:"<<m_idTimer;
    this->setFocus(); //
    m_gameOn=true;
}

// handling of button
void Glass::keyPressEvent(QKeyEvent*event)
{
    if(m_gameOn)
    {
        switch(event->key())
        {
            case Qt::Key_Left:
                if(cur->indxW()>0 && m_glassArray[cur->indxH()][cur->indxW()-1] == emptyCell)
                {
                    cur->stepLeft();
                }
                break;
            case Qt::Key_Right:
                if(cur->indxW()<m_columns-1  && m_glassArray[cur->indxH()][cur->indxW()+1] == emptyCell)
                {
                    cur->stepRight();
                }
                break;
            case Qt::Key_Down:
                cur->rotateColors(Qt::Key_Down);
                break;
            case Qt::Key_Up:
                cur->rotateColors(Qt::Key_Up);
                break;
            case Qt::Key_Space:
                // fell figure
                while( (cur->indxH()!=m_rows-1) && (m_glassArray[cur->indxH()+1][cur->indxW()]==emptyCell) )
                {
                    cur->stepDown(); // step down while figure does not achieve bottom
                }
                acceptColors(); // save colors of figure in 2D-array of glass
                break;
            default:
                QWidget::keyPressEvent(event); //
        }
    }
    else
    {
        QWidget::keyPressEvent(event); //
    }
}

void Glass::timerEvent(QTimerEvent *event)
{
    // check if figure has fallen
    if(cur->indxH() == m_rows-1 || m_glassArray[cur->indxH()+1][cur->indxW()] != emptyCell )
    {
        if(cur->indxH()<=2) // Glass is full, game over
        {
            qDebug()<<"Kill timer:"<<m_idTimer;
            this->killTimer(m_idTimer);
            glassInit();
            m_gameOn=false;
        }
        else
        {
            this->acceptColors(); // save colors of figure in 2D-array of glass
        }
    }
    else
    {
        cur->stepDown(); // step down
        this->update(); // redraw
    }
}

// save colors of figure in 2D-array of glass
void Glass::acceptColors()
{
    for(uint j=0; j<3; j++)
    {
        m_glassArray[cur->indxH()-j][cur->indxW()] = cur->color(j);
    }

    bool flagDelete=true;

    // Delete all combination of three and more cells same colors in row or columns
    while(flagDelete)
    {
        flagDelete=findHor();
        flagDelete=findVer();
    }
    emit signalSetScore(m_score); //

    swapFigure(cur, next);
    cur->setIndxH(0);
    cur->setIndxW((this->m_columns)>>1);
    next->setIndxH(0);
    next->setIndxW(0);
    next->makeRandomColors();
    emit Glass::signalChangePattern(next); //
}

// Check and delete three or more cells of the same color on row
bool Glass::findHor()
{
    uint startHor;
    uint finishHor;
    bool flagDeleteGlob=false;
    bool flagDeleteHor=true;

    while(flagDeleteHor) // do until all combinations have been removed
    {
        flagDeleteHor=false;
        for (uint i=0; i<m_rows;i++)
        {
            for (uint j=0; j<m_columns-1;j++)
            {
                startHor=j;
                finishHor=startHor+1;
                while(   m_glassArray[i][startHor]!=emptyCell // current cell isn't empty
                      && finishHor!=m_columns // current cell isn't last in row
                      && m_glassArray[i][startHor]==m_glassArray[i][finishHor]) // next cell color is same as current cell color
                {
                    finishHor++; // save and check next cell
                }
                if(finishHor-startHor>2) // if three or more cells of the same color on row
                {
                    m_score+=finishHor-startHor; // add point
                    flagDeleteHor=true; // if some cells delete then it is necessary repeat cycle in rows
                    flagDeleteGlob=true; // and execute cycle in columns
                    j--; // return index in place before this combination
                    deleterHor(i, startHor,finishHor); // delete cells
                }
            }
        }
    }
    return flagDeleteGlob;
}

// Delete three or more cells of the same color on row
void Glass::deleterHor(uint row, uint startHor, uint finishHor)
{
    for (uint i=row; i>0; i--) // start from row where cells are deleted
    {
        for (uint j=startHor; j<finishHor; j++) // handle only that columns where cells are deleted
        {
            m_glassArray[i][j]=m_glassArray[i-1][j]; // save new colors from cells which locate upper
        }
    }
    for (uint j=startHor; j<finishHor; j++) // handle 0 raw
    {
        m_glassArray[0][j]=emptyCell;
    }
}

// Check and delete three or more cells of the same color in column
bool Glass::findVer()
{
    uint startVer;
    uint finishVer;
    bool flagDeleteGlob=false;
    bool flagDeleteVer=true;

    while(flagDeleteVer) // do until all combinations have been removed
    {
        flagDeleteVer=false;
        for (uint j=0; j<m_columns; j++) //
        {
            for (uint i=0; i<m_rows-1; i++)
            {
                startVer=i;
                finishVer=startVer+1;
                while(   m_glassArray[startVer][j]!=emptyCell // current cell isn't empty
                      && finishVer!=m_rows  // current cell isn't last in column
                      && m_glassArray[startVer][j]==m_glassArray[finishVer][j]) // next cell color is same as current cell color
                {
                    finishVer++; // save and check next cell
                }
                if(finishVer-startVer>2) // if three or more cells of the same color on row
                {
                    m_score+=finishVer-startVer; // add point
                    flagDeleteVer=true; // if some cells delete then it is necessary repeat cycle in column
                    flagDeleteGlob=true; // and execute cycle in row
                    i--; // return index in place before this combination
                    deleterVer(j, startVer, finishVer-1); // delete cells
                }
            }
        }
    }
    return flagDeleteGlob;
}

// Delete three or more cells of the same color on column
void Glass::deleterVer(uint column, uint startVer, uint finishVer) // fdvdfv
{
    // save new colors from cells which locate upper than deleted combination
    uint i=finishVer, k=startVer-1; // finishVer>start
    uint del = i-k;

    for(; k>0; i--, k-- )
    {
        m_glassArray[i][column]=m_glassArray[k][column];
    }
    for(; del>0; del--)
    {
        m_glassArray[del][column]=emptyCell;
    }
}

// Swap current and next figures
void Glass::swapFigure(Figure* cur, Figure* next)
{
    Figure tmp = *cur;
    *cur=*next;
    *next=tmp;
}

// add new methos
