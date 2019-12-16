#ifndef NETLIST_CELL_WIDGET_H
#define NETLIST_CELL_WIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QRect>
#include <QPoint>
#include <QMenuBar>
#include <QAction>
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
      virtual QSize   minimumSizeHint     () const;
      virtual void    resizeEvent         (QResizeEvent*);
              void    query               (unsigned int, QPainter&);

    protected:
      virtual void    paintEvent          (QPaintEvent*);
      virtual void    keyPressEvent       (QKeyEvent*);
      void            goUp                ();
      void            goDown              ();
      void            goLeft              ();
      void            goRight             ();
    private:
      Cell* cell_;
      Box   viewport_ ;
  };


  inline Cell*  CellWidget::getCell             ()                 const {return cell_;}
  inline QRect  CellWidget::boxToScreenRect     (const Box&    b)  const {return QRect(b.getX1(), b.getY1(), b.getWidth(), b.getHeight());}
  inline QPoint CellWidget::pointToScreenPoint  (const Point&  p)  const {return QPoint(p.getX(), p.getY());}
  inline Box    CellWidget::screenRectToBox     (const QRect&  r)  const {return Box(r.x(), r.y(), r.x()+r.width(), r.y()+r.height());} 
  inline Point  CellWidget::screenPointToPoint  (const QPoint& p)  const {return Point(p.x(), p.y());}

}  // Netlist namespace.

#endif  // NETLIST_CELL_WIDGET_H