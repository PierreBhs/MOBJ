// fichier créé pour alléger le fichier Shape.cpp et retrouver plus rapidement les fonctions toXML et fromXml

#include "Shape.h"
#include "Symbol.h"
#include "Term.h"

namespace Netlist {

Shape* Shape::fromXml ( Symbol* owner, xmlTextReaderPtr reader ) {
    // Factory-like method.
    const xmlChar* boxTag      = xmlTextReaderConstString( reader, (const xmlChar*)"box" );
    const xmlChar* ellipseTag  = xmlTextReaderConstString( reader, (const xmlChar*)"ellipse" );
    const xmlChar* arcTag      = xmlTextReaderConstString( reader, (const xmlChar*)"arc");
    const xmlChar* lineTag     = xmlTextReaderConstString( reader, (const xmlChar*)"line" );
    const xmlChar* termTag     = xmlTextReaderConstString( reader, (const xmlChar*)"term" );
    const xmlChar* nodeName    = xmlTextReaderConstLocalName( reader );

    Shape* shape = NULL;
    if (boxTag == nodeName)
        shape = BoxShape::fromXml( owner, reader );
    if (ellipseTag == nodeName)
        shape = EllipseShape::fromXml( owner, reader );
    if (arcTag == nodeName) {
        shape = ArcShape::fromXml( owner, reader );
        std::cout << "ici\n";
    }
    if (lineTag == nodeName)
        shape = LineShape::fromXml( owner, reader );
    if (termTag == nodeName)
        shape = TermShape::fromXml( owner, reader );

    if (shape == NULL)
        std::cerr << "[ERROR] Unknown or misplaced tag <" << nodeName << "> (line:"
             << xmlTextReaderGetParserLineNumber(reader) << ")." << std::endl;

    return shape;
}


/////////////////////
///    BoxShape   ///
/////////////////////

BoxShape*   BoxShape::fromXml (Symbol* owner, xmlTextReaderPtr reader) {
    const xmlChar*  boxTag   = xmlTextReaderConstString(reader, (const xmlChar*)"box");
    const xmlChar*  nodeName = xmlTextReaderConstLocalName(reader);
    int             nodeType = xmlTextReaderNodeType(reader);

    int x1, y1, x2, y2;

    if(nodeName == boxTag && nodeType == XML_READER_TYPE_ELEMENT) {
        x1    = atoi(xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"x1")).c_str());
        y1    = atoi(xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"y1")).c_str());
        x2    = atoi(xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"x2")).c_str());
        y2    = atoi(xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"y2")).c_str());
        return new BoxShape(owner, x1, y1, x2, y2);
    } 

    else
        return nullptr;
}

void BoxShape::toXml(std::ostream& o) {
    o << indent << "<box x1=\"" << box_.getX1() << "\" y1=\"" << box_.getY1() << "\" x2=\"" << box_.getX2() << "\" y2=\"" << box_.getY2() <<"\"/>\n";
}


///////////////////////
///    TermShape    ///
///////////////////////

TermShape* TermShape::fromXml(Symbol* owner, xmlTextReaderPtr reader) {
    const xmlChar*  termTag  = xmlTextReaderConstString(reader, (const xmlChar*)"term");
    const xmlChar*  nodeName = xmlTextReaderConstLocalName(reader);
    int             nodeType = xmlTextReaderNodeType(reader);

    std::string name;
    int x1, y1;
    TermShape::NameAlign nameAlign;

    if(nodeName == termTag && nodeType == XML_READER_TYPE_ELEMENT) {
        name      = xmlCharToString((xmlTextReaderGetAttribute(reader, (const xmlChar*)"name")));
        x1        = atoi(xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"x1")).c_str());
        y1        = atoi(xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"y1")).c_str());
        nameAlign = stringToAlign(xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"align")));

        return new TermShape(owner, name, x1, y1, nameAlign);
    } 
    else
        return nullptr;

    return new TermShape(owner,  "name", 0, 0, NameAlign::TopLeft);
}

void TermShape::toXml(std::ostream& o) {
    o << indent << "<term name=\"" << term_->getName() << "\" x1=\"" << x_ <<"\" y1=\"" << y_ << "\" align=\"" << alignToString(align_) << "\"/>\n";
}



//////////////////////
///    LineShape   ///
//////////////////////

LineShape* LineShape::fromXml(Symbol* owner, xmlTextReaderPtr reader) {
    const xmlChar*  lineTag  = xmlTextReaderConstString(reader, (const xmlChar*)"line");
    const xmlChar*  nodeName = xmlTextReaderConstLocalName(reader);
    int             nodeType = xmlTextReaderNodeType(reader);

    int x1, y1, x2, y2;

    if(nodeName == lineTag && nodeType == XML_READER_TYPE_ELEMENT) {
        x1        = atoi(xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"x1")).c_str());
        y1        = atoi(xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"y1")).c_str());
        x2        = atoi(xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"x2")).c_str());
        y2        = atoi(xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"y2")).c_str());

        return new LineShape(owner, x1, y1, x2, y2);
    } 
    else
        return nullptr;
}

void LineShape::toXml(std::ostream& o) {
    o << indent << "<line x1=\"" << x1_ << "\" y1=\"" << y1_ << "\" x2=\"" << x2_ << "\" y2=\"" << y2_ <<"\"/>\n";
}



/////////////////////////
///    EllipseShape   ///
/////////////////////////

EllipseShape* EllipseShape::fromXml(Symbol* owner, xmlTextReaderPtr reader) {
    const xmlChar*  lineTag  = xmlTextReaderConstString(reader, (const xmlChar*)"ellipse");
    const xmlChar*  nodeName = xmlTextReaderConstLocalName(reader);
    int             nodeType = xmlTextReaderNodeType(reader);

    int x1, y1, x2, y2;

    if(nodeName == lineTag && nodeType == XML_READER_TYPE_ELEMENT) {
        x1        = atoi(xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"x1")).c_str());
        y1        = atoi(xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"y1")).c_str());
        x2        = atoi(xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"x2")).c_str());
        y2        = atoi(xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"y2")).c_str());

        return new EllipseShape(owner, x1, y1, x2, y2);
    } 
    else
        return nullptr;
}

void EllipseShape::toXml(std::ostream& o) {
     o << indent << "<ellipse x1=\"" << box_.getX1() << "\" y1=\"" << box_.getY1() << "\" x2=\"" << box_.getX2() << "\" y2=\"" << box_.getY2() <<"\"/>\n";
}


/////////////////////////
///    ArchShape   ///
/////////////////////////

void ArcShape::toXml(std::ostream& o) {
     o << indent << "<arc x1=\"" << box_.getX1() << "\" y1=\"" << box_.getY1() << "\" x2=\"" << box_.getX2() << "\" y2=\"" << box_.getY2() << "\" start=\"" << start_ << "\" span=\"" << span_ <<"\"/>\n";
}

ArcShape* ArcShape::fromXml(Symbol* owner, xmlTextReaderPtr reader) {
    const xmlChar*  lineTag  = xmlTextReaderConstString(reader, (const xmlChar*)"ellipse");
    const xmlChar*  nodeName = xmlTextReaderConstLocalName(reader);
    int             nodeType = xmlTextReaderNodeType(reader);

    int x1, y1, x2, y2, start, span;

    if(nodeName == lineTag && nodeType == XML_READER_TYPE_ELEMENT) {
        x1        = atoi(xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"x1")).c_str());
        y1        = atoi(xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"y1")).c_str());
        x2        = atoi(xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"x2")).c_str());
        y2        = atoi(xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"y2")).c_str());
        start     = atoi(xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"start")).c_str());
        span      = atoi(xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"span")).c_str());

        return new ArcShape(owner, x1, y1, x2, y2, start, span);
    } 
    else
        return nullptr;
}

}