#include "Shape.h"
#include "Symbol.h"
namespace Netlist {


Shape::Shape ( Symbol* owner ) : owner_(owner) {
    owner->add(this); 
}

Shape::~Shape() {
    owner_ ->remove( this );
}

////// BoxShape //////

BoxShape::BoxShape( Symbol* owner , const  Box& box )
: Shape(owner), box_(box) { 

}

BoxShape::BoxShape( Symbol* owner, int x1, int y1, int x2, int y2 )
    :Shape(owner), box_(x1, y1, x2, y2) {

}
BoxShape ::~ BoxShape() { 

}
Box BoxShape::getBoundingBox () const {
    return  box_;
}


////// TermShape //////

TermShape::TermShape( Symbol* owner , std::string  name , int x, int y, NameAlign align)
                      : Shape(owner), term_(NULL), x_(x), y_(y), align_(align) {

    term_ = owner->getCell()->getTerm(name);
}
TermShape ::~ TermShape  () {

}
Box TermShape :: getBoundingBox  ()  const { 
    return  Box(x_, y_, x_, y_); 
}

// idem term
TermShape::NameAlign TermShape::stringToAlign(std::string align) {
    if ( align == "top_left")
      return TopLeft ;
    if ( align == "top_right" )
      return TopRight;
    if ( align == "bottom_left")
      return BottomLeft ;
    if ( align == "bottom_right")
      return BottomRight ;
    else
      return BottomRight;
}


// Idem a term
std::string TermShape::alignToString( NameAlign align ) {
    if ( align == TopLeft )
      return "top_left";
    if ( align == TopRight )
      return "top_right";
    if ( align == BottomLeft )
      return "bottom_left";
    else
      return "bottom_right";
}





////// LineShape //////
LineShape::LineShape(Symbol* owner, int x1, int y1, int x2, int y2) 
: Shape(owner), x1_(x1), y1_(y1), x2_(x2), y2_(y2) {

}

LineShape::~LineShape() {

}

Box LineShape::getBoundingBox () const {
    return  Box(x1_ ,y1_ ,x2_ ,y2_); 
}


////// EllipseShape ///////
EllipseShape::EllipseShape(Symbol* owner, int x1, int y1, int x2, int y2)
: Shape(owner), box_(x1, y1, x2, y2) {

}

EllipseShape::~EllipseShape() {

}

Box EllipseShape::getBoundingBox () const {
    return  box_; 
}

////// ArcShape ///////
ArcShape::ArcShape(Symbol* symbol, int x1, int y1, int x2, int y2, int start, int span) :
                  Shape(symbol), box_(x1, y1, x2, y2), start_(start), span_(span) {

}

ArcShape::~ArcShape() {
  
}

Box ArcShape::getBoundingBox () const {
    return  box_; 
}

}