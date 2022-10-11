# GameTetris
Tetris a simple version of application where the player manipulates a dropping into glass figures.

The result of the decomposition of the project is the list of modules: main, mainwindow, glass, figure, nextfigure.

Mainwindow is class inherited from the QMainWindow. Widget layout is done with QT Designer.

Glass is class inherited from the QWidget. Glass is game field which consists on grid of cells. 
Figure with color cells fells down into glass and takes free place. Thus glass is filling color cells.
Timer defines frequency of steps. If three or more cell in row or column have same color, then they are deleted.
The game finish if figures full fill glass at least in one column.

Figure consist on three vertical random colors cell. User can:
 - move right and left figure by buttons right and left arrows.
 - rotate colors into figure by bottons up and down arrows
 - quickly drop down figure by botton space.

NextFigure is is auxiliary class releted with class figure. Goal this class is display in window next figure
