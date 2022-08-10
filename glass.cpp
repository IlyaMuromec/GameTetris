#include "glass.h"
#include <QPainter>
#include <QBrush>
#include <QKeyEvent>


Glass::Glass(QWidget *parent)
    : QWidget{parent}
{
    m_timerinterval=200;
    cur = new Figure(m_ceil);
    next = new Figure(m_ceil);
    m_gameOn=false;
    m_score=0;
}
Glass::~Glass()
{
    delete cur;
    delete next;
}

uint Glass::rows() const
{
    return m_rows;
}

void Glass::setRows(uint newRows)
{
    m_rows = newRows;
}

unsigned int Glass::columns() const
{
    return m_columns;
}

void Glass::setColumns(unsigned int newColumns)
{
    m_columns = newColumns;
}

void Glass::glassInit(void)
{
    //изменяем размер вектора, он будет содержать m_rows строк
    m_glassArray.resize(m_rows);
    //изменяем размер вектора строки, он будет содержать m_columns элементов
    for (uint i=0; i<m_rows;i++)
    {
        m_glassArray[i].resize(m_columns);
    }
    this->clearGlass();
    m_wGlass= m_columns * (m_ceil+1);
    m_hGlass= m_rows * (m_ceil+1);
    this->setFixedSize(QSize(m_wGlass, m_hGlass));
    m_score=0;
}

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

void Glass::paintEvent(QPaintEvent*event)
{
    QPainter painter(this);

    painter.setBrush(emptyCell);
    painter.setPen(Qt::NoPen);

    for (uint i=0; i<m_rows;i++)
    {
        for (uint j=0; j<m_columns;j++)
        {
            painter.setBrush(m_glassArray[i][j]);
            painter.drawRect(j*20, i*20, m_ceil, m_ceil);
        }
    }

    if(m_gameOn) //а фигурку будем рисовать только, если «идет игра»
    {
        for(uint j=0; j<3; j++ )
        {
            painter.setBrush(cur->color(j));
            painter.drawRect(cur->indxW()*20, (cur->indxH()-j)*20, m_ceil, m_ceil);
        }
    }
}

void Glass::mySlotGame()
{
    clearGlass();
    glassInit();
    cur->setIndxH(0);
    cur->setIndxW((this->m_columns)>>1);
    next->setIndxH(0);
    next->setIndxW(0);
    emit signalChangePattern(next);

    if(m_gameOn==true)
    {
        qDebug()<<"Kill timer"<<m_idTimer;
        this->killTimer(m_idTimer);
    }
    m_idTimer=this->startTimer(m_timerinterval);
    qDebug()<<"Start timer:"<<m_idTimer;
    this->setFocus();
    m_gameOn=true;
}

void Glass::keyPressEvent(QKeyEvent*event)
{
    if(m_gameOn)
    {
    //Если «идет игра»
        switch(event->key())
        {//код нажатой клавиши
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
                //циклически ”переливаем” цвета в фигурке сверху вниз
                break;
            case Qt::Key_Up:
                cur->rotateColors(Qt::Key_Up);
                //циклически ”переливаем” цвета в фигурке снизу вверх
                break;
            case Qt::Key_Space:
                //«роняем» фигурку
                while( (cur->indxH()!=m_rows-1) && (m_glassArray[cur->indxH()+1][cur->indxW()]==emptyCell) )
                {
                    cur->stepDown();
                }
                acceptColors();
                break;
            default:
                QWidget::keyPressEvent(event); //при нажатии любых других клавиш вызываем базовую обработку события
        }
    }
    else
    {
        QWidget::keyPressEvent(event);//предоставляем возможность базовому
        //классу обработать событие
    }
}

void Glass::timerEvent(QTimerEvent *event)
{
    //qDebug()<<"TimerEvent";

    if(cur->indxH() == m_rows-1 ||
            m_glassArray[cur->indxH()+1][cur->indxW()]!=emptyCell )
    {
        if(cur->indxH()<=2)
        {
            qDebug()<<"Kill timer:"<<m_idTimer;
            this->killTimer(m_idTimer);
            glassInit();
            m_gameOn=false;
        }
        else
        {
            this->acceptColors();
        }
    }
    else
    {
        cur->stepDown();
        this->update();
    }
}


void Glass::acceptColors()
{
    for(uint j=0; j<3; j++)
    {
        m_glassArray[cur->indxH()-j][cur->indxW()]=cur->color(j);
    }

    bool flagDelete=true;
    while(flagDelete)
    {
        flagDelete=findHor();
        flagDelete=findVer();
    }
    emit signalSetScore(m_score);

    swapFigure(cur, next);
    cur->setIndxH(0);
    cur->setIndxW((this->m_columns)>>1);
    next->setIndxH(0);
    next->setIndxW(0);
    next->makeRandomColors();
    emit Glass::signalChangePattern(next);
}


bool Glass::findHor()
{
    uint startHor;
    uint finishHor;
    bool flagDeleteGlob=false;
    bool flagDeleteHor=true;

    while(flagDeleteHor)
    {
        flagDeleteHor=false;
        for (uint i=0; i<m_rows;i++)
        {
            for (uint j=0; j<m_columns-2;j++)
            {
                startHor=j;
                finishHor=startHor+1;
                while(   m_glassArray[i][startHor]!=emptyCell
                      && finishHor!=m_columns
                      && m_glassArray[i][startHor]==m_glassArray[i][finishHor])
                {
                    finishHor++;
                }
                if(finishHor-startHor>2)
                {
                    m_score+=finishHor-startHor;
                    flagDeleteHor=true;
                    flagDeleteGlob=true;
                    j--;
                    deleterHor(i, startHor,finishHor);
                }
            }
        }
    }
    return flagDeleteGlob;
}

void Glass::deleterHor(uint row, uint startHor, uint finishHor)
{
    for (uint i=row; i>0; i--)
    {
        for (uint j=startHor; j<finishHor; j++)
        {
            m_glassArray[i][j]=m_glassArray[i-1][j];
        }
    }
    for (uint j=startHor; j<finishHor; j++)
    {
        m_glassArray[0][j]=emptyCell;
    }
}

bool Glass::findVer()
{
    uint startVer;
    uint finishVer;
    bool flagDeleteGlob=false;
    bool flagDeleteVer=true;

    while(flagDeleteVer)
    {
        flagDeleteVer=false;
        for (uint j=0; j<m_columns; j++)
        {
            for (uint i=0; i<m_rows-1; i++)
            {
                startVer=i;
                finishVer=startVer+1;
                while(   m_glassArray[startVer][j]!=emptyCell
                      && finishVer!=m_rows
                      && m_glassArray[startVer][j]==m_glassArray[finishVer][j])
                {
                    finishVer++;
                }
                if(finishVer-startVer>2)
                {
                    m_score+=finishVer-startVer;
                    flagDeleteVer=true;
                    flagDeleteGlob=true;
                    i--;
                    deleterVer(j, startVer, finishVer-1);
                }
            }
        }
    }
    return flagDeleteGlob;
}

void Glass::deleterVer(uint column, uint startVer, uint finishVer)
{
    for (uint i=finishVer; i>finishVer-startVer; i--)
    {
        m_glassArray[i][column]=m_glassArray[startVer-1-(finishVer-i)][column];
    }
    for(uint i=finishVer-startVer-1; i!=0; i--)
    {
        m_glassArray[i][column]=emptyCell;
    }
}

void Glass::swapFigure(Figure* cur, Figure* next)
{
    Figure tmp = *cur;
    *cur=*next;
    *next=tmp;
}
