#include "Shape.h"

namespace Netlist {


Shape::Shape ( Symbol* owner ) : owner_(owner) {
    owner ->add( this ); 
}

Shape::~Shape() {
    owner_ ->remove( this );
}

Shape* Shape::fromXml ( Symbol* owner, xmlTextReaderPtr reader ) {
    // Factory-like method.
    const xmlChar* boxTag      = xmlTextReaderConstString( reader, (const xmlChar*)"box" );
    const xmlChar* ellipseTag  = xmlTextReaderConstString( reader, (const xmlChar*)"ellipse" );
    const xmlChar* arcTag      = xmlTextReaderConstString( reader, (const xmlChar*)"arc" );
    const xmlChar* lineTag     = xmlTextReaderConstString( reader, (const xmlChar*)"line" );
    const xmlChar* termTag     = xmlTextReaderConstString( reader, (const xmlChar*)"term" );
    const xmlChar* nodeName    = xmlTextReaderConstLocalName( reader );

    Shape* shape = NULL;
    if (boxTag == nodeName)
        shape = BoxShape::fromXml( owner, reader );
    if (ellipseTag == nodeName)
        shape = EllipseShape::fromXml( owner, reader );
    if (arcTag == nodeName)
        shape = ArcShape::fromXml( owner, reader );
    if (lineTag == nodeName)
        shape = LineShape::fromXml( owner, reader );
    if (termTag == nodeName)
        shape = TermShape::fromXml( owner, reader );

    if (shape == NULL)
        std::cerr << "[ERROR] Unknown or misplaced tag <" << nodeName << "> (line:"
             << xmlTextReaderGetParserLineNumber(reader) << ")." << std::endl;

    return shape;
    }



////// BoxShape //////

BoxShape::BoxShape ( Symbol* owner , const  Box& box )
: Shape(owner), box_(box) { 
}
BoxShape ::~ BoxShape() { 

}
Box BoxShape::getBoundingBox () const {
    return  box_;
}

<box x1="20" y1="0" x2="100" y2="80"/>
void TermShape::toXml(std::ostream& o) {
    o << indent << "<box x1=\"" << x1_ <<"\" y1=\"" << y1_ << "x2=\"" << x2_ <<"\" y2=\"" << y2_ << "\"/>\n";
}

////// TermShape //////

TermShape :: TermShape ( Symbol* owner , std::string  name , int x1, int y1 )
                      : Shape(owner), term_(NULL), x1_(x1), y1_(y1) {
    Cell* cell = getCell ();
    term_ = cell ->getTerm( name );
}
TermShape ::~ TermShape  () {

}
Box TermShape :: getBoundingBox  ()  const { 
    return  Box(x1_ ,y1_ ,x1_ ,y1_); 
}


////// LineShape //////
LineShape::LineShape(Symbol* owner, int x1, int y1, int x2, int y2) 
: Shape(owner_), x1_(x1), y1_(y1), x2_(x2), y2_(y2) {

}

Box LineShape::getBoundingBox () const {
    return  Box(x1_ ,y1_ ,x2_ ,y2_); 
}


////// EllipseShape ///////
EllipseShape::EllipseShape(Symbol* owner, int x1, int y1, int x2, int y2)
: Shape(owner_), x1_(x1), y1_(y1), x2_(x2), y2_(y2) {

}


}