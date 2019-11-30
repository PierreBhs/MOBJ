#include "Term.h"
#include "Net.h"
#include "Cell.h"


namespace Netlist {
class Node;

Net::Net(Cell* cell, const std::string& name, Term::Type t) : owner_(cell), name_(name), 
        id_(cell->newNetId()), type_(t), nodes_() {
    
	owner_->add(this);
}
    
// supprime le lien (pointeur) entre vector<Net*> et l'objet Net pointé
Net::~Net() {
	// Terms connectés aux Nets
    for(std::vector<Node*>::iterator node = nodes_.begin(); node != nodes_.end(); node++) {
    	if (*node != nullptr)
    	dynamic_cast<NodeTerm*>((*node))->getTerm()->setNet(nullptr);
    }

    while (!nodes_.empty())
    	nodes_.pop_back();

	owner_->remove(this);
}


void Net::add(Node* node) {
	if(not node)
		return;
	size_t id = node->getId();
	
	if(id == Node::noid) {
		id = getFreeNodeId();
		node->setId(id);
	}
	if(id < nodes_.size()) {
		if (nodes_[id] != nullptr) {
			std::cerr << "ERROR\n";
			nodes_[id]->setId(Node::noid);
		}
		nodes_[id] = node;
	}
	else
	{
		for(size_t i = nodes_.size(); i < id; ++i) {
			nodes_.push_back(nullptr);
		}
		nodes_.push_back(node);
	}
	
}

bool Net::remove(Node* n) {
	for(auto& node: nodes_) {
		if(n == node) {
			node = nullptr;
			return true;
		}
	}
	return false;
}

void  Net::add ( Line* line )
{ if (line) lines_.push_back( line ); }

size_t Net::getFreeNodeId() const {
	size_t i;
	for(i = 0; i < nodes_.size(); i++) {
		if(nodes_[i] == nullptr)
			return i;
	}
	return nodes_.size();
}

bool  Net::remove ( Line* line ) {
  if (line) {
    for ( std::vector<Line*>::iterator il = lines_.begin()
        ; il != lines_.end() ; ++il ) {
      if (*il == line) {
        lines_.erase( il );
        return true;
      }
    }
  }
  return false;
}

Node* Net::getNode(size_t id) const {
	for(auto n : nodes_) {
		if(n->getId() == id)
			return n;
	}
	return nullptr;
}

void Net::toXml(std::ostream& o) {
    o << indent << "<net name=\"" << name_ <<"\" type=\"" << Term::toString(type_) << "\">\n";
	indent++;
	for(auto node: getNodes()) {
		if(node != nullptr)
			node->toXml(o);
	}
	indent--;
	o << indent << "</net>\n";
}

Net* Net::fromXml(Cell* cell, xmlTextReaderPtr reader) {
	const xmlChar* netTag   = xmlTextReaderConstString(reader, (const xmlChar*)"net");
	const xmlChar* lineTag  = xmlTextReaderConstString(reader, (const xmlChar*)"line" );
	const xmlChar* nodeTag  = xmlTextReaderConstString(reader, (const xmlChar*)"node");
	const xmlChar* nodeName = xmlTextReaderConstLocalName(reader);
	int            nodeType = xmlTextReaderNodeType(reader);

	enum  State { Init };
	State state = Init;

	Net* net;
	std::string name;
	Term::Type t;
 
	if(nodeName == netTag && nodeType == XML_READER_TYPE_ELEMENT) { // <net>
		name = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"name"));
		t    = Term::toType(xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"type")));
		net = new Net(cell, name, t);

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
					if(nodeName == netTag && nodeType == XML_READER_TYPE_END_ELEMENT) { // </net>
						return net;
					}

					else if (nodeName == lineTag) {
             	 		if (Line::fromXml(net, reader))
							continue; 
					}

					else if(nodeName == nodeTag && nodeType == XML_READER_TYPE_ELEMENT) { // <node>
						if(Node::fromXml(net, reader)) {
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