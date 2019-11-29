#include "Box.h"
#include "Symbol.h"

namespace Netlist {

class Symbol;
class Box;

class  Shape {
public:
                     Shape            (Symbol*);
    virtual          ~Shape           ();
    inline   Symbol* getSymbol        ()  const;
    virtual Box      getBoundingBox   ()  const = 0;
    
private:
    Symbol* owner_;
};

inline  Symbol* Shape:: getSymbol  ()  const { return  owner_; }

class  BoxShape : public  Shape {
public:
                     BoxShape         (Symbol*, const  Box& );
                     BoxShape         (Symbol*, int x1 , int y1, int x2, int y2 );
                    ~BoxShape         ();
    Box              getBoundingBox  ()  const;
private:
    Box   box_;
};

}
