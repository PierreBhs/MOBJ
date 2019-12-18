#ifndef NETLIST_SHAPE_H
#define NETLIST_SHAPE_H

#include  <libxml/xmlreader.h>
#include "Box.h"
#include "Symbol.h"
#include "Cell.h"
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
    virtual  void    toXml            (std::ostream&) = 0;
    static   Shape*  fromXml(Symbol*, xmlTextReaderPtr);
    
private:
    Symbol* owner_;
};

inline  Symbol* Shape:: getSymbol  ()  const { return  owner_; }


//////   BoxShape   //////

class  BoxShape : public  Shape {
public:
                     BoxShape         (Symbol*, const  Box& );
                     BoxShape         (Symbol*, int x1 , int y1, int x2, int y2 );
                    ~BoxShape         ();
    Box              getBoundingBox   ()  const;
    void             toXml            (std::ostream&);
    static BoxShape* fromXml          ( Symbol*, xmlTextReaderPtr);
private:
    Box   box_;
};
 
//////   TermShape   //////

class  TermShape : public  Shape {
public:
    enum NameAlign { TopLeft=1, TopRight, BottomLeft, BottomRight };

                        TermShape        (Symbol*, std::string, int, int, NameAlign);
                        ~TermShape       ();
            Box         getBoundingBox   () const;
    inline  Term*       getTerm          () const;
    inline  int         getX             () const;
    inline  int         getY             () const;
    static  std::string alignToString    (NameAlign);
    static  NameAlign   stringToAlign    (std::string);
            void        toXml            (std::ostream&);
    static  TermShape*  fromXml          ( Symbol*, xmlTextReaderPtr);
private:
    Term*     term_;
    int       x_ , y_;
    NameAlign align_;
};

inline Term*     TermShape::getTerm          () const { return term_; }
inline  int      TermShape::getX             () const { return x_; }
inline  int      TermShape::getY             () const { return y_; }


///////// LineShape /////////
class LineShape : public Shape {
public:
                        LineShape(Symbol*, int, int, int, int);
                        ~LineShape();
    Box                 getBoundingBox   ()  const;
    void                toXml            (std::ostream&);                        
    static LineShape*   fromXml          (Symbol*, xmlTextReaderPtr);

    // Getters
    inline int          getX1            () const;
    inline int          getY1            () const;
    inline int          getX2            () const;
    inline int          getY2            () const;
private:
    int x1_;
    int y1_;
    int x2_;
    int y2_;
};

inline int       LineShape::getX1            () const { return x1_;}
inline int       LineShape::getY1            () const { return y1_;}
inline int       LineShape::getX2            () const { return x2_;}
inline int       LineShape::getY2            () const { return y2_;}



class EllipseShape : public Shape {
public:
                            EllipseShape     (Symbol*, int, int, int, int);
                            EllipseShape     (Symbol*, const Box&);
                            ~EllipseShape    ();
    void                    toXml            (std::ostream&);               
    static EllipseShape*    fromXml          (Symbol*, xmlTextReaderPtr);
    Box                     getBoundingBox   ()  const;

private:
    Box   box_;
};

class ArcShape : public Shape {
public:
                     ArcShape         (Symbol*, int, int, int , int, int, int);
                     ~ArcShape        ();
    void             toXml            (std::ostream&);          
    static ArcShape* fromXml          ( Symbol*, xmlTextReaderPtr);
    Box              getBoundingBox   () const;
    inline int       getStart         () const;
    inline int       getSpan          () const;
private:
    Box box_;
    int start_;
    int span_;

};

inline int ArcShape::getStart           () const { return start_;}
inline int ArcShape::getSpan           () const { return span_;}
}


#endif  // NETLIST_SYMBOL_H
