#include  <libxml/xmlreader.h>
#include "Symbol.h"
#include "Shape.h"


namespace Netlist {

Symbol::Symbol( Cell* cell): owner_(cell), shapes_() {

}

Symbol::~Symbol() {

}

Cell* Symbol::getCell() const {
    return owner_;
}

void Symbol::add(Shape* s) {
    shapes_.push_back(s);
}

void Symbol::remove(Shape* s) {
    for(std::vector<Shape*>::iterator it = shapes_.begin(); it < shapes_.end(); ++it) {
        if((*it) == s)
            shapes_.erase(it);
    }
}

TermShape* Symbol::getTermShape(Term* term) const {
    TermShape* termShape;
    for (auto shape: shapes_) {
    	termShape = dynamic_cast<TermShape*>(shape);
      if (termShape && termShape->getTerm() == term)
        return termShape;
    }
    return nullptr;
}

Box Symbol::getBoundingBox() const {
    Box box;
    for (auto b : shapes_) {
      box.merge((b)->getBoundingBox());
    }
    return box;
}

Point Symbol::getTermPosition(Term* term) const {
    TermShape* termShape = getTermShape(term);
    if(!termShape) {
        std::cerr << "Error in getTermPosition, no term found\n";
        std::abort();
    }

    return Point(termShape->getX(), termShape->getY());
}

void Symbol::toXml(std::ostream& stream) const {
    stream << indent++ << "<symbol>\n";

    for(auto* shape: shapes_) {
		if(shape)
       		shape->toXml(stream);
    }

    stream << --indent << "</symbol>\n";
}

Symbol* Symbol::fromXml(Cell* cell, xmlTextReaderPtr reader) {
    const xmlChar* symbolTag = xmlTextReaderConstString(reader, (const xmlChar*)"symbol");
	const xmlChar* boxTag    = xmlTextReaderConstString(reader, (const xmlChar*)"box");
    const xmlChar* termTag   = xmlTextReaderConstString(reader, (const xmlChar*)"term");
	const xmlChar* lineTag   = xmlTextReaderConstString(reader, (const xmlChar*)"line" );
	const xmlChar* elliTag   = xmlTextReaderConstString(reader, (const xmlChar*)"ellipse");
	const xmlChar* arcTag    = xmlTextReaderConstString(reader, (const xmlChar*)"arc");
	const xmlChar* nodeName  = xmlTextReaderConstLocalName(reader);
	int            nodeType  = xmlTextReaderNodeType(reader);

	enum  State { Init };
	State state = Init;

	Symbol* sym;
 
	if(nodeName == symbolTag && nodeType == XML_READER_TYPE_ELEMENT) { // <symbol>
		sym = cell->getSymbol(); 

		while ( true ) {
			int status = xmlTextReaderRead(reader);
			if (status != 1) {
				if (status != 0) {
					std::cerr << "[ERROR] Cell::fromXml(): Unexpected termination of the XML parser." << std::endl;
				}
			break;
			}

			switch ( xmlTextReaderNodeType(reader) ) {
				case XML_READER_TYPE_COMMENT:
				case XML_READER_TYPE_WHITESPACE:
				case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
					continue;
			}

			nodeName = xmlTextReaderConstLocalName(reader);
			nodeType = xmlTextReaderNodeType(reader);

			switch(state) {
				
				case Init:
					if(nodeName == symbolTag && nodeType == XML_READER_TYPE_END_ELEMENT) { // </symbol>
						return sym;
					}

					else if(nodeName == boxTag) {
             	 		if (BoxShape::fromXml(sym, reader)){
							continue; 
                          }
					}

                    else if(nodeName == termTag) {
             	 		if (TermShape::fromXml(sym, reader)){
							continue; 
                          }
					}

					else if(nodeName == lineTag) {
             	 		if (LineShape::fromXml(sym, reader))
							continue; 
						  
					}

					else if(nodeName == arcTag) {
             	 		if (ArcShape::fromXml(sym, reader)) {
							continue; 
						  }
					}

					else if(nodeName == elliTag && nodeType == XML_READER_TYPE_ELEMENT) { 
						if(EllipseShape::fromXml(sym, reader)) {
							continue;
						}
					}

					else { 
						std::cout << "No tag found\n";
						std::cout << nodeName << std::endl;
						break;
					}
				default:
					break;
			}
			
		}
	}

	return nullptr;
}


} 