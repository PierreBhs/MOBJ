#include "Term.h"
#include "Node.h"
#include "Cell.h"
#include "Net.h"
#include "Instance.h"
#include "Point.h"
#include <string>


namespace Netlist {

Term::Term(Cell* cell, const std::string& name, Direction d) : owner_(cell), name_(name), direction_(d), 
        type_(External), net_(nullptr), node_(this)
    {
        
        static_cast<Cell*>(owner_)->add(this); // cell->add(this);
    }

Term::Term(Instance* i, const Term* modelTerm) : owner_(i), name_(modelTerm->getName()), 
        direction_(modelTerm->getDirection()), type_(Internal), net_(modelTerm->getNet()), node_(this)
    {
        static_cast<Instance*>(owner_)->add(this); //// i->add(this);
    }

Term::~Term() {
    net_->remove(&node_);
    if (type_ == Type::External)
        ((Cell*)owner_)->remove(this);
    else 
        ((Instance*)owner_)->remove(this);
}

void Term::setNet(Net* net) {

    if(getNet() != nullptr) {
        /* for(std::vector<Node*>::iterator i = getNet()->getNodes().begin(); i != getNet()->getNodes().end(); i++) {
            if(dynamic_cast<NodeTerm*>((*i))->getTerm() == this)
                getNet()->remove(*i);
        } */
        net_ = nullptr;
    }
    if(net == nullptr)
        net_ = nullptr;
    else {
        net_ = net;
        getNet()->add(getNode());
    }
}

void Term::setNet(const std::string& name) {
    Cell* cell = getOwnerCell();
    for(auto& net : cell->getNets()) {
        if (net->getName() == name) {
            setNet (net);
            return;
        }
    }
}

void Term::setPosition(const Point& p) {
    node_.setPosition(p);
}

void Term::setPosition(int x, int y) {
    node_.setPosition(x, y);
}

Cell* Term::getOwnerCell() const {
    if(isInternal())
        return static_cast<Cell*>(getInstance()->getCell());
    
    return static_cast<Cell*>(owner_);
}


std::string Term::toString(Direction d) {
    switch(d) {
        case 1: return "In";
        case 2: return "Out";
        case 3: return "Inout";
        case 4: return "Tristate";
        case 5: return "Transcv";
        case 6: return "Unknown";
        default:
            return "Mauvaise direction";
    }
}


Term::Direction Term::toDirection(std::string s) {
   
    if( s == "In")       return In;
    if( s == "Out")      return Out;
    if( s == "Inout")    return Inout;
    if( s == "Tristate") return Tristate;
    if( s == "Transcv")  return Transcv;
    if( s == "Unknown")  return Unknown;
    else 
        return Unknown;
}

std::string Term::toString(Type t) {
    switch(t) {
        case 1: return "Internal";
        case 2: return "External";
        default:
            return "Mauvaise direction";
    }
}

Term::Type Term::toType(std::string s) {
   
    if( s == "Internal")  return Internal;
    if( s == "External")  return External;
    else 
        return Error;
}


void Term::toXml(std::ostream& o) {
    o << indent << "<term name=\"" << name_ <<"\" direction=\"" << toString(direction_) << "\"  x=\"" << getNode()->getPosition().getX()
		<< "\" y=\"" << getNode()->getPosition().getY() <<"\"/>\n";
}

Term*  Term::fromXml(Cell* cell, xmlTextReaderPtr reader) { 
    const xmlChar*  termTag  = xmlTextReaderConstString(reader, (const xmlChar*)"term" );
    const xmlChar*  nodeName = xmlTextReaderConstLocalName(reader);
    int             nodeType = xmlTextReaderNodeType(reader);

    Term* term;
    int x, y;
    std::string name;
    Direction direction;

    if(nodeName == termTag && nodeType == XML_READER_TYPE_ELEMENT) {
        x    = atoi(xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"x")).c_str());
        y    = atoi(xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"y")).c_str());
        name = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"name"));
        direction = toDirection(xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"direction")));
        term = new Term(cell, name, direction);
        term->setPosition(x, y);
        return term;
    } 
    else
        return nullptr;
}


} // fin namespace

