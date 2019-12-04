#ifndef NETLIST_TERM_H
#define NETLIST_TERM_H

#include <string>
#include <ostream>
#include "Node.h"
#include "XmlUtil.h"

namespace Netlist {

class Net;
class Instance;
class Cell;
class Point;
class Node;

// peut appartenir à une instance ou une cell
class Term {
public:
    enum Type      { Internal=1, External=2, Error=3}; // si connecteur appartient a une cell -> External; si appartient a une instance ->Internal  
    enum Direction { In=1, Out=2,  Inout=3, Tristate=4, Transcv=5, Unknown=6};

    static std::string toString    (Type);
    static std::string toString    (Direction);
    static Type        toType (std::string);
    static Direction   toDirection (std::string);

    Term(Cell*, const std::string& name, Direction);
    Term(Instance*, const Term* modelTerm);
    ~Term();


    // Modificateurs
    void setNet (Net*);
    void setNet (const std::string&); // Cherche net avec ce nom et net_ devient ce net || optimisation, plus nécessaire ajd, une copie au lieu de 2 
    inline void setDirection(Direction);
    void setPosition(const Point&);
    void setPosition(int x, int y);

    // Accesseurs
    inline bool               isInternal   () const;
    inline bool               isExternal   () const;
    inline const std::string& getName      () const;
    inline NodeTerm*          getNode      ();
    inline Net*               getNet       () const;
    inline Cell*              getCell      () const;
           Cell*              getOwnerCell () const;
    inline Instance*          getInstance  () const;
    inline Direction          getDirection () const;
    inline Point              getPosition  () const;
    inline Type               getType      () const;

    

    void               toXml(std::ostream&);
    static Term*       fromXml(Cell*, xmlTextReaderPtr);
private:
    void*       owner_; // Instance* ou Cell*
    std::string name_;
    Direction   direction_;
    Type        type_;
    Net*        net_;
    NodeTerm    node_;
};

inline void       Term::setDirection(Direction d) { direction_ = d;}

// Accesseurs
inline bool       Term::isInternal()       const { return type_ == 1;}
inline bool       Term::isExternal()       const { return type_ == 2;}
inline const std::string& Term::getName()  const { return name_;}
inline NodeTerm*      Term::getNode()                { return &node_;}
inline Net*       Term::getNet()           const { return net_;}
inline Cell*      Term::getCell()          const { return (type_ == External) ? static_cast<Cell*>(owner_) : nullptr;}
inline Instance*  Term::getInstance()      const { return (type_ == Internal) ? static_cast<Instance*>(owner_): nullptr ;}
inline Term::Direction Term::getDirection()const { return direction_;}
inline Point      Term::getPosition()      const { return node_.getPosition();}
inline Term::Type Term::getType()          const { return type_;}


} // fin namespace

#endif