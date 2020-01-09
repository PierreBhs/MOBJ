#ifndef NETLIST_CELL_WIDGET_H
#define NETLIST_CELL_WIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QRect>
#include <QPoint>
#include <QMenuBar>
#include <QAction>
#include <QPen>

class QPainter;
#include "Box.h"


namespace Netlist {

  class Cell;
  class NodeTerm;


  class CellWidget : public QWidget {
      Q_OBJECT;
    public:
                      CellWidget          ( QWidget* parent=NULL );
      virtual        ~CellWidget          ();
              void    setCell             ( Cell* );
      inline  Cell*   getCell             () const;
      inline  QRect   boxToScreenRect     (const Box&) const;
      inline  QPoint  pointToScreenPoint  (const Point &) const;
      inline  Box     screenRectToBox     (const QRect &) const;
      inline  Point   screenPointToPoint  ( const QPoint &) const;
      inline  int     xToScreenX          (int) const;
      inline  int     yToScreenY          (int) const;
      inline  int     screenXToX          (int) const;
      inline  int     screenYToY          (int) const;
      virtual QSize   minimumSizeHint     () const;
      virtual void    resizeEvent         (QResizeEvent*);
              void    query               (unsigned int, QPainter&, QPen&);

    protected:
      virtual void    paintEvent          (QPaintEvent*);
      virtual void    keyPressEvent       (QKeyEvent*);
      void            goUp                ();
      void            goDown              ();
      void            goLeft              ();
      void            goRight             ();
    private:
      Cell* cell_;
      Box   viewport_ ; // partie du schéma vue à l'écran
  };


  inline Cell*  CellWidget::getCell             ()                 const {return cell_;}
  inline int    CellWidget::xToScreenX          ( int x )          const {return x - viewport_.getX1();}
  inline int    CellWidget::yToScreenY          ( int y )          const {return viewport_.getY2() - y;}
  inline int    CellWidget::screenXToX          ( int x )          const {return x + viewport_.getX1();}
  inline int    CellWidget::screenYToY          ( int y )          const {return viewport_.getY2() - y;}
  inline QRect  CellWidget::boxToScreenRect     (const Box&    b)  const {return QRect(xToScreenX(b.getX1()), yToScreenY(b.getY2()), b.getWidth(), b.getHeight());}
  inline QPoint CellWidget::pointToScreenPoint  (const Point&  p)  const {return QPoint(xToScreenX(p.getX()), yToScreenY(p.getY()));}
  inline Box    CellWidget::screenRectToBox     (const QRect&  r)  const {return Box(screenXToX(r.x()), screenYToY(r.y()), screenXToX(r.x()+r.width()), screenYToY(r.y()+r.height()));} 
  inline Point  CellWidget::screenPointToPoint  (const QPoint& p)  const {return Point(screenYToY(p.x()), screenYToY(p.y()));}
  
}  // Netlist namespace.

#endif  // NETLIST_CELL_WIDGET_H