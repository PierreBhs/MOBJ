#include "Symbol.h"
#include "Shape.h"
namespace Netlist {

Symbol::Symbol( Cell* cell): owner_(cell){

}

Symbol::~Symbol() {}

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

}

