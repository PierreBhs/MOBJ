#include  <libxml/xmlreader.h>
#include "Box.h"
#include "Symbol.h"
#include <ostream>

namespace Netlist {

class Symbol;
class Box;

class  Shape {
public:
                     Shape            (Symbol*);
    virtual          ~Shape           ();
    inline   Symbol* getSymbol        ()  const;
    virtual  Box     getBoundingBox   ()  const = 0;
    static   Shape*  fromXml(Symbol*, xmlTextReaderPtr);
    
private:
    Symbol* owner_;
};

inline  Symbol* Shape:: getSymbol  ()  const { return  owner_; }

class  BoxShape : public  Shape {
public:
                     BoxShape         (Symbol*, const  Box& );
                     BoxShape         (Symbol*, int x1 , int y1, int x2, int y2 );
                    ~BoxShape         ();
    Box              getBoundingBox   ()  const;
    static Shape*    fromXml          ( Symbol*, xmlTextReaderPtr);
private:
    Box   box_;
};
 
class  TermShape : public  Shape {
public:
    enum NameAlign { TopLeft=1, TopRight, BottomLeft, BottomRight };

                     TermShape        ( Symbol*, std::string  name , int x, int y );
                    ~TermShape        ();
             Box     getBoundingBox   () const;
    inline   Term*   getTerm          () const;
    inline   int     getX             () const;
             void    toXml            (std::ostream&);
    static Shape*    fromXml          ( Symbol*, xmlTextReaderPtr);
private:
    Term* term_;
    int    x_ , y_;
};

///////// LineShape /////////
class LineShape : public Shape {
public:
                     LineShape(Symbol*, int, int, int, int);
    static Shape*    fromXml          ( Symbol*, xmlTextReaderPtr);
    Box              getBoundingBox   ()  const;

    // Getters
    inline int       getX1            () const;
    inline int       getY1            () const;
    inline int       getX2            () const;
    inline int       getY2            () const;
private:
    int x1_, x2_;
    int y1_, y2_;
};

inline int       LineShape::getX1            () const { return x1_;}
inline int       LineShape::getY1            () const { return y1_;}
inline int       LineShape::getX2            () const { return x2_;}
inline int       LineShape::getY2            () const { return y2_;}



class EllipseShape : public Shape {
public:
                     EllipseShape     (Symbol*, int, int, int, int);
    static Shape*    fromXml          ( Symbol*, xmlTextReaderPtr);
    Box              getBoundingBox   ()  const;

    inline int       getX1            () const;
    inline int       getY1            () const;
    inline int       getX2            () const;
    inline int       getY2            () const;
private:
    int x1_, x2_;
    int y1_, y2_;
};

class ArcShape : public Shape {
public:
    static Shape*    fromXml          ( Symbol*, xmlTextReaderPtr);
    Box              getBoundingBox   ()  const;
private:

};

}
