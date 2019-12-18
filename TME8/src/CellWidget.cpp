#include  <QResizeEvent>
#include  <QPainter>
#include  <QPen>
#include  <QBrush>
#include  <QFont>
#include  <QApplication>
#include  "CellWidget.h"
#include  "Term.h"
#include  "Instance.h"
#include  "Symbol.h"
#include  "Shape.h"
#include  "Cell.h"
#include  "Line.h"
#include  "Node.h"
#include  "Net.h"


namespace Netlist {

  using namespace std;


  ostream& operator<< ( ostream& o, const QRect& rect )
  {
    o << "<QRect x:" << rect.x()
      <<       " y:" << rect.y()
      <<       " w:" << rect.width()
      <<       " h:" << rect.height() << ">";
    return o;
  }


  ostream& operator<< ( ostream& o, const QPoint& p )
  { o << "<QRect x:" << p.x() << " y:" << p.y() << ">"; return o; }



CellWidget::CellWidget ( QWidget* parent )
  : QWidget(parent)
  , cell_  (NULL)
  , viewport_(Box(0 ,0 ,500 ,500)) 
  {
      setAttribute    ( Qt::WA_OpaquePaintEvent );   // repeindre toute la cell
      setAttribute    ( Qt::WA_NoSystemBackground ); // avant que je trace, ne mets pas le fond par défaut
      setAttribute    ( Qt::WA_StaticContents );     
      setSizePolicy   ( QSizePolicy::Expanding, QSizePolicy::Expanding ); // peut s'étendre mais pas etre plus petit que 500,500
      setFocusPolicy  ( Qt::StrongFocus );
      setMouseTracking( true ); // recevoir tous les évènements de la souris
  }


CellWidget::~CellWidget (){ 

}


void  CellWidget::setCell ( Cell* cell ) {
  cell_ = cell;
  repaint();
}


QSize  CellWidget::minimumSizeHint () const { // pour Qt, fait aussi 500 par 500 pixels -> donc au démarrage on aura la meme taille de fenetre
  return QSize(500,500); 
}


void  CellWidget::resizeEvent ( QResizeEvent* event ) { // quand on redimensionne la fenetre
  const QSize & size = event->size();
  // Assume the resize is always done by drawing the bottom right corner .
  viewport_.setX2(viewport_.getX1() + size.width());
  viewport_.setY1(viewport_.getY2 () - size.height());
  cerr << " CellWidget :: resizeEvent () viewport_ : " << viewport_ << endl ;
  repaint(); 
}


void  CellWidget::paintEvent ( QPaintEvent* event ) { 
  QPainter painter(this);
  painter.setBackground(QBrush(Qt::black)); 
  painter.eraseRect(QRect(QPoint(0,0), size()));
  painter.setPen( QPen (Qt::darkGreen, 1)); // bordure
  QRect rect = boxToScreenRect(viewport_);
  painter.drawRect(rect);
  painter.setPen(QPen(Qt::red, 0));
  painter.setBrush(QBrush(Qt::blue));
  query(1, painter);
  // ...
  //painter.drawRect(rect2);
}

/* void CellWidget::paintEvent(QPaintEvent * event) {
  QPainter painter(this);
  painter.setBackground(QBrush(Qt::black)); 
  painter.eraseRect(QRect(QPoint(0,0), size()));
  painter.setPen( QPen (Qt::darkGreen, 1)); // bordure
  QRect rect = boxToScreenRect(box);
  painter.drawRect(rect1);
  painter.setPen(QPen(Qt::red, 0));
  painter.setBrush(QBrush(Qt::red));
  // ...
  painter.drawRect(rect2);
}
 */

void CellWidget::keyPressEvent(QKeyEvent * event ) {
	event->ignore();
  	if(event->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier)) // si on appuie sur Shift ou Ctrl en meme temps -> on ignore 
    	return;
 
  	switch(event->key()) {
    	case Qt::Key_Up    : goUp();    break;
    	case Qt::Key_Down  : goDown();  break;
    	case Qt::Key_Left  : goLeft();  break;
    	case Qt::Key_Right : goRight(); break;
    	default : return ;
  }
  event->accept();
}

void CellWidget::goRight() {
  viewport_ .translate(Point(20, 0));
  repaint();
}

void CellWidget::goLeft() {
  viewport_ .translate(Point(-20, 0));
  repaint();
}

void CellWidget::goDown() {
  viewport_ .translate(Point(0, -20));
  repaint();
}

void CellWidget::goUp() {
  viewport_.translate(Point(0, 20));
  repaint();
}

void CellWidget::query( unsigned int flags , QPainter & painter ) {
	if((not cell_) or (not flags)) {
    	std::cout << "not cells\n";
    	return;
  	}
   
  	const vector <Instance*>& instances = cell_->getInstances ();
  	for( size_t i = 0; i < instances.size(); ++i) {
		Point instPos = instances[i]->getPosition();
	  	std::cout << "i = " << i << std::endl;
      	const Symbol * symbol = instances[i]->getMasterCell()->getSymbol();
    
      	if (not symbol) 
    		continue;

    	std::cout << instances[i]->getName() << std::endl;
    	if(flags /* &  InstanceShapes*/) {
			const vector <Shape*>& shapes = symbol->getShapes();
			for (size_t j =0 ; j < shapes.size () ; ++ j) {
				//shapes[j]->toXml(std::cout);
				ArcShape*  arcShape  = dynamic_cast<ArcShape*>(shapes[j]);
				LineShape* lineShape = dynamic_cast<LineShape*>(shapes[j]);
				TermShape* termShape = dynamic_cast<TermShape*>(shapes[j]);
				if (arcShape) {
					//arcShape->toXml(std::cout);
					Box box = arcShape->getBoundingBox();
					
					QRect rect = boxToScreenRect(box.translate(instPos));
					painter.setBrush(QBrush(Qt::gray));
					painter.setPen(QPen(Qt::darkGreen, 3));
					std::cout << instPos << std::endl;
					painter.drawArc(rect, 16 * arcShape->getStart(), 16 * arcShape->getSpan());
				}

				if(lineShape) {
					
					painter.setPen(QPen(Qt::darkGreen, 3));
					Box box = lineShape->getBoundingBox();
			
					QRect rect = boxToScreenRect(box.translate(instPos));
					painter.drawRect(rect);
				}

				if(termShape) {
					termShape->toXml(std::cout);
					Box box = termShape->getBoundingBox();
					std::cout << termShape->getBoundingBox() << std::endl;
					std::cout << "box translate " << box.translate(instPos) << std::endl;
					
					QRect rect = boxToScreenRect(box.translate(instPos));
					painter.setBrush(QBrush(Qt::red));
					painter.setPen(QPen(Qt::red, 3));
					rect.setHeight(5);
					rect.setWidth(5);
					std::cout << "rect x " << rect.x() << std::endl;
					std::cout << "rect y " << rect.y() << std::endl;
					std::cout << "rect width " << rect.width() << std::endl;
					std::cout << "rect height " << rect.height() << std::endl;

					painter.drawRect(rect);
				}
			}
    	}
  }

	const vector<Net*>& nets = cell_->getNets();
  	for(size_t i = 0; i < nets.size(); i++) {
		  
  	}
}

}  // Netlist namespace.