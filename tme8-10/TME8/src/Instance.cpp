#include "Instance.h"
#include "Cell.h"
#include "Term.h"
#include "Net.h"
#include "Shape.h"
#include <vector>
namespace Netlist {

Instance::Instance(Cell* owner, Cell* model, const std::string& name): owner_(owner), 
            masterCell_(model), name_(name), terms_() {
               
                const std::vector<Term*>& terms = model->getTerms();
                for(auto it = terms.begin(); it < terms.end(); it++){
                    new Term(this, (*it));
                }
                 owner->add(this);
            } 

Instance::~Instance() {
    while(!terms_.empty())
        delete *terms_.begin();

    owner_->remove(this);
}


Term* Instance::getTerm(const std::string& name) const {
    for(auto& t: terms_) {
        if(t->getName() == name) 
            return t;
    }
    return nullptr;
}

bool Instance::connect(const std :: string & name , Net * n) {
    Term *term = getTerm(name);
	if (term == NULL)
		return false;

	term->setNet(n);
	return true;
}

void Instance::add(Term* t) {
    if (getTerm(t->getName()))
		{
			std::cerr << "[ERROR] Attemp to add duplicated terminal <" << t->getName() << ">.\n";
			exit(1);
		}
		terms_.push_back(t);
}

void  Instance::remove( Term* t) {
    for (std::vector<Term* > ::iterator it = terms_.begin() ; it != terms_.end() ; ++it){
        if (*it == t) terms_.erase(it);
        break;
    }
}

void  Instance::setPosition(const Point& p){
    position_.setX(p.getX());
    position_.setY(p.getY());

     for(auto& shape : getMasterCell()->getSymbol()->getShapes()) { 
            TermShape* termShape = dynamic_cast<TermShape*>(shape);
            if (termShape) {                                              
                std::string name = termShape->getTerm()->getName();       
                for(auto& termInst : getTerms()) {                      
                    if(name == termInst->getName())                          
                        termInst->setPosition(termShape->getX() + p.getX(), termShape->getY() + p.getY());               
                }
            }
        }
}

void  Instance::setPosition(int x, int y){
    position_.setX(x);
    position_.setY(y); 

    for(auto& shape : getMasterCell()->getSymbol()->getShapes()) { 
            TermShape* termShape = dynamic_cast<TermShape*>(shape);
            if (termShape) {                                              
                std::string name = termShape->getTerm()->getName();       
                for(auto& termInst : getTerms()) {                      
                    if(name == termInst->getName())                          
                        termInst->setPosition(termShape->getX() + x , termShape->getY() + y);               
                }
            }
        }
}

void Instance::toXml(std::ostream& o) {
    o << indent << "<instance name=\"" << name_ <<"\" mastercell=\"" << masterCell_->getName()
      << "\" x=\"" << position_.getX() << "\" y=\"" << position_.getY() << "\"/>\n";
}

Instance* Instance::fromXml(Cell* cell, xmlTextReaderPtr reader) {
    const xmlChar* instanceTag = xmlTextReaderConstString(reader, (const xmlChar*)"instance");
    const xmlChar* nodeName    = xmlTextReaderConstLocalName(reader);
    int nodeType = xmlTextReaderNodeType(reader);

    Cell* model;
    std::string modelName;
    Instance* instance;
    std::string name;
    int x, y;

    if(nodeName == instanceTag && nodeType == XML_READER_TYPE_ELEMENT) {
        modelName = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"mastercell"));
        model     = Cell::find(modelName);
        if(!model)
            return nullptr;
        name = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"name"));
        x    = atoi(xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"x")).c_str());
        y    = atoi(xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"y")).c_str());
        instance = new Instance(cell, model, name);
        instance->setPosition(x, y);
        return instance;
    }
    else
        return nullptr; 
} 

} // namespace Netlist
