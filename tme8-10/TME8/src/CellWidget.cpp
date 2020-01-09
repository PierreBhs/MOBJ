#include  <QResizeEvent>
#include  <QPainter>
#include  <QPen>
#include  <QBrush>
#include  <QFont>
#include  <QApplication>
#include <QPointF> 
#include <QString> 
#include <QStaticText> 
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
	QPen pen;
	
  	painter.setBackground(QBrush(Qt::gray)); 
	painter.eraseRect(QRect(QPoint(0,0), size()));
  	painter.setPen( QPen (Qt::darkGreen, 1)); // bordure
  	QRect rect = boxToScreenRect(viewport_);
  	painter.drawRect(rect);
  	painter.setPen(QPen(Qt::red, 0));
  	painter.setBrush(QBrush(Qt::blue));
	painter.setPen(pen);
  	query(1, painter, pen);

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


  // nombre de lignes pour une intersection -> si >= 3 -> faire un point sur le schéma
int numLines(vector<size_t>& ids, size_t n_id){
	int n = 0;
    for (auto& id : ids) {
        if (id == n_id)
          n++;
	}
    
	return n;
}

void CellWidget::query( unsigned int flags , QPainter& painter, QPen& pen) {
	if((not cell_) or (not flags)) {
    	std::cout << "not cells\n";
    	return;
  	}
   

	// Dessins des lignes en premier pour ne pas dessiner par dessus les shapes
	for(auto& net:  cell_->getNets()) {
		//vecteur des id des lignes
		std::vector<size_t> lines_id;
		//dessin des lignes
		for(auto& line: net->getLines()) {
			Point source = line->getSourcePosition();
			Point target = line->getTargetPosition();

			lines_id.push_back(line->getSource()->getId());
            lines_id.push_back(line->getTarget()->getId());

			painter.setPen(QPen(Qt::yellow, 1));
			painter.drawLine(xToScreenX(source.getX()), yToScreenY(source.getY()), xToScreenX(target.getX()), yToScreenY(target.getY()));
		}

		// dessins des intersections
		for(auto& node: net->getNodes()) {
			if(!node || numLines(lines_id, node->getId()) < 3) // si < 3 -> pas besoin car pas d'inter
				continue;

            QPoint position = pointToScreenPoint(node->getPosition());

			painter.setPen(QPen(Qt::yellow, 1));
			painter.drawEllipse(QPointF(xToScreenX(node->getPosition().getX()), yToScreenY(node->getPosition().getY())), 4, 4);
			painter.setPen(QPen(Qt::cyan, 4));
            painter.drawPoint(position);
		}
	}


	// dessins des shapes
  	const vector <Instance*>& instances = cell_->getInstances();
	
  	for(size_t i = 0; i < instances.size(); ++i) {
		Point instPos = instances[i]->getPosition();
      	const Symbol * symbol = instances[i]->getMasterCell()->getSymbol();
    
      	if (not symbol) 
    		continue;

		// nom associé
			QFont font = painter.font();
			font.setPixelSize(12);
			painter.setFont(font);
			painter.setPen(QPen(Qt::green, 2));
			QStaticText stat = QStaticText(QString::fromStdString(instances[i]->getName()));

			// pour ne pas avoir les noms qui passent par dessus le polygone
			painter.drawStaticText(QPointF(xToScreenX(instPos.getX() + 15), yToScreenY(instPos.getY() - 5)), stat);


    	if(flags /* &  InstanceShapes*/) {
			const vector <Shape*>& shapes = symbol->getShapes();
			for (size_t j =0 ; j < shapes.size () ; ++ j) {
				
				ArcShape*  arcShape  = dynamic_cast<ArcShape*>(shapes[j]);
				LineShape* lineShape = dynamic_cast<LineShape*>(shapes[j]);
				//TermShape* termShape = dynamic_cast<TermShape*>(shapes[j]);
				BoxShape*  boxShape  = dynamic_cast<BoxShape*>(shapes[j]);

				if (arcShape) {
					
					Box box = arcShape->getBoundingBox();
					
					QRect rect = boxToScreenRect(box.translate(instPos));
					pen.setColor(Qt::green);
					pen.setWidth(2);
					pen.setCapStyle(Qt::RoundCap);
					pen.setJoinStyle(Qt::RoundJoin);
					painter.setPen(pen);
					painter.drawArc(rect, 16 * arcShape->getStart(), 16 * arcShape->getSpan());
					continue; // plus efficace, saute toutes les autres conditions car seuelemnt celle la sera valide
				}

				if(lineShape) {
					Box box = lineShape->getBoundingBox();
					QRect rect = boxToScreenRect(box.translate(instPos));

					painter.setPen(QPen(Qt::green, 2));
					painter.drawLine(rect.x(), rect.y(), rect.x()+rect.width(), rect.y()+rect.height());
					continue;
				}

				if(boxShape) {
					Box box = boxShape->getBoundingBox();
					
					QRect rect = boxToScreenRect(box.translate(instPos));
					painter.setBrush(QBrush(Qt::blue));
					painter.setPen(QPen(Qt::magenta, 1));
	
					painter.drawRect(rect); 
					continue;

				}

				// Dessinné a la fin pour ne pas etre recouvert par les dessins des arcShapes
				/* if(termShape) {
					Box box = termShape->getBoundingBox();
					box.inflate(2);
					
					QRect rect = boxToScreenRect(box.translate(instPos));
					painter.setBrush(QBrush(Qt::red));
					painter.setPen(QPen(Qt::red, 3));
					pen.setCapStyle(Qt::SquareCap);
					pen.setJoinStyle(Qt::MiterJoin);
	
					painter.drawRect(rect);

					QFont font = painter.font();
					font.setPixelSize(12);
					painter.setFont(font);
					painter.setPen(QPen(Qt::red, 4));

					// drawText ne marchait pas seulement avec les flags, j'ai donc modifié le QRect du text

					if(termShape->getAlign() == 1) // top_left
						painter.drawText(QRect(rect.left() - 12, rect.top() - 12, 20, 20), Qt::AlignTop | Qt::AlignLeft, tr(termShape->getTerm()->getName().c_str()));
					
				 	if(termShape->getAlign() == 2) // top_right
						painter.drawText((QRect(rect.right() - 5, rect.top() - 12, 20, 20)), Qt::AlignTop | Qt::AlignRight, tr(termShape->getTerm()->getName().c_str()));

					if(termShape->getAlign() == 3) // bottom_left
						painter.drawText((QRect(rect.left() - 12, rect.bottom() - 12, 20, 20)), Qt::AlignBottom | Qt::AlignLeft, tr(termShape->getTerm()->getName().c_str()));
 

					continue;
				} */
			}
    	}
	}

	// external terms
	if(instances.size()) {
	for(auto& term: cell_->getTerms()) {
		Point pos = term->getPosition();
		int direction = term->getDirection();
	
		// In
		if(direction == 1) {
			const QPointF points[5] = {
				QPointF(xToScreenX(pos.getX()),      yToScreenY(pos.getY())),      // point de base, a droite du triangle
				QPointF(xToScreenX(pos.getX() - 10), yToScreenY(pos.getY() - 10)),   
 				QPointF(xToScreenX(pos.getX() - 30), yToScreenY(pos.getY() - 10)),
				QPointF(xToScreenX(pos.getX() - 30), yToScreenY(pos.getY() + 10)),
				QPointF(xToScreenX(pos.getX() - 10), yToScreenY(pos.getY() + 10))
			};
			painter.setPen(QPen(Qt::darkBlue, 2));
			painter.setBrush(QBrush(Qt::blue));
			painter.drawConvexPolygon(points, 5);

			// nom associé
			QFont font = painter.font();
			font.setPixelSize(12);
			painter.setFont(font);
			painter.setPen(QPen(Qt::black, 1));
			QStaticText stat = QStaticText( QString::fromStdString(term->getName()));

			// pour ne pas avoir les noms qui passent par dessus le polygone
			painter.drawStaticText(QPointF(xToScreenX(pos.getX() - 40 - term->getName().size() * 10), yToScreenY(pos.getY() + 10)), stat);
			continue;
		}

		// Out
		if(direction == 2) {
			const QPointF points[5] = {
				QPointF(xToScreenX(pos.getX()),      yToScreenY(pos.getY())),      // point de base, a gauche du triangle
				QPointF(xToScreenX(pos.getX() + 10), yToScreenY(pos.getY() - 10)),   
 				QPointF(xToScreenX(pos.getX() + 30), yToScreenY(pos.getY() - 10)),
				QPointF(xToScreenX(pos.getX() + 30), yToScreenY(pos.getY() + 10)),
				QPointF(xToScreenX(pos.getX() + 10), yToScreenY(pos.getY() + 10))
			};
			painter.setPen(QPen(Qt::darkBlue, 2));
			painter.setBrush(QBrush(Qt::blue));
			painter.drawConvexPolygon(points, 5);

			// nom associé
			QFont font = painter.font();
			font.setPixelSize(12);
			painter.setFont(font);
			painter.setPen(QPen(Qt::black, 1));
			QStaticText stat = QStaticText( QString::fromStdString(term->getName()));

			painter.drawStaticText(QPointF(xToScreenX(pos.getX() + 5), yToScreenY(pos.getY() - 20)), stat);
			continue;
		}

	}
	}


		// On dessine les terms internes à la fin pour qu'ils soient bien visibles
		for(size_t i = 0; i < instances.size(); ++i) {
			Point instPos = instances[i]->getPosition();
			const Symbol * symbol = instances[i]->getMasterCell()->getSymbol();
		
			if (not symbol) 
				continue;


			const vector <Shape*>& shapes = symbol->getShapes();
			for (size_t j =0 ; j < shapes.size () ; ++ j) {
			
				TermShape* termShape = dynamic_cast<TermShape*>(shapes[j]);

				if(termShape) {
					Box box = termShape->getBoundingBox();
					box.inflate(2);
					
					QRect rect = boxToScreenRect(box.translate(instPos));
					painter.setBrush(QBrush(Qt::red));
					painter.setPen(QPen(Qt::red, 3));
					pen.setCapStyle(Qt::SquareCap);
					pen.setJoinStyle(Qt::MiterJoin);
	
					painter.drawRect(rect);

					QFont font = painter.font();
					font.setPixelSize(12);
					painter.setFont(font);
					painter.setPen(QPen(Qt::red, 4));

					// drawText ne marchait pas seulement avec les flags, j'ai donc modifié le QRect du text

					if(termShape->getAlign() == 1) // top_left
						painter.drawText(QRect(rect.left() - 12, rect.top() - 17, 30, 20), Qt::AlignTop | Qt::AlignLeft, tr(termShape->getTerm()->getName().c_str()));
					
				 	if(termShape->getAlign() == 2) // top_right
						painter.drawText((QRect(rect.right() - 15, rect.top() - 17, 30, 20)), Qt::AlignTop | Qt::AlignRight, tr(termShape->getTerm()->getName().c_str()));

					if(termShape->getAlign() == 3) // bottom_left
						painter.drawText((QRect(rect.left() - 12, rect.bottom() - 12, 30, 20)), Qt::AlignBottom | Qt::AlignLeft, tr(termShape->getTerm()->getName().c_str()));

					if(termShape->getAlign() == 4) // bottom_right
						painter.drawText((QRect(rect.right() - 7, rect.bottom() - 12, 30, 20)), Qt::AlignBottom | Qt::AlignRight, tr(termShape->getTerm()->getName().c_str()));
 

					continue;
				}
			}

		}

		// si pas d'instances, on dessine juste les shapes
		if(instances.size() == 0) {
			const  Symbol* symbol = cell_->getSymbol();
			for(auto& shape : symbol->getShapes()) {
				
				ArcShape*  arcShape  = dynamic_cast<ArcShape*>(shape);
				LineShape* lineShape = dynamic_cast<LineShape*>(shape);
				TermShape* termShape = dynamic_cast<TermShape*>(shape);
				BoxShape*  boxShape  = dynamic_cast<BoxShape*>(shape);

				if (arcShape) {
					
					Box box = arcShape->getBoundingBox();
					
					QRect rect = boxToScreenRect(box);
					pen.setColor(Qt::green);
					pen.setWidth(2);
					pen.setCapStyle(Qt::RoundCap);
					pen.setJoinStyle(Qt::RoundJoin);
					painter.setPen(pen);
					painter.drawArc(rect, 16 * arcShape->getStart(), 16 * arcShape->getSpan());
					continue; // plus efficace, saute toutes les autres conditions car seuelemnt celle la sera valide
				}

				if(lineShape) {
					Box box = lineShape->getBoundingBox();
					QRect rect = boxToScreenRect(box);

					painter.setPen(QPen(Qt::green, 2));
					painter.drawLine(rect.x(), rect.y(), rect.x()+rect.width(), rect.y()+rect.height());
					continue;
				}

				if(boxShape) {
					Box box = boxShape->getBoundingBox();
					
					QRect rect = boxToScreenRect(box);
					painter.setBrush(QBrush(Qt::blue));
					painter.setPen(QPen(Qt::magenta, 1));
	
					painter.drawRect(rect); 
					continue;

				}

				// Dessinné a la fin pour ne pas etre recouvert par les dessins des arcShapes
				if(termShape) {
					Box box = termShape->getBoundingBox();
					box.inflate(2);
					
					QRect rect = boxToScreenRect(box);
					painter.setBrush(QBrush(Qt::red));
					painter.setPen(QPen(Qt::red, 3));
					pen.setCapStyle(Qt::SquareCap);
					pen.setJoinStyle(Qt::MiterJoin);
	
					painter.drawRect(rect);

					QFont font = painter.font();
					font.setPixelSize(12);
					painter.setFont(font);
					painter.setPen(QPen(Qt::red, 4));

					// drawText ne marchait pas seulement avec les flags, j'ai donc modifié le QRect du text

					if(termShape->getAlign() == 1) // top_left
						painter.drawText(QRect(rect.left() - 12, rect.top() - 12, 20, 20), Qt::AlignTop | Qt::AlignLeft, tr(termShape->getTerm()->getName().c_str()));
					
				 	if(termShape->getAlign() == 2) // top_right
						painter.drawText((QRect(rect.right() - 5, rect.top() - 12, 20, 20)), Qt::AlignTop | Qt::AlignRight, tr(termShape->getTerm()->getName().c_str()));

					if(termShape->getAlign() == 3) // bottom_left
						painter.drawText((QRect(rect.left() - 12, rect.bottom() - 12, 20, 20)), Qt::AlignBottom | Qt::AlignLeft, tr(termShape->getTerm()->getName().c_str()));
 

					continue;
				}
			}
		}
}

}  // Netlist namespace.