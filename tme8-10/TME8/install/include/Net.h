#ifndef NETLIST_NET_H
#define NETLIST_NET_H

#include <string>
#include <vector>
#include <ostream>
#include "Term.h"
#include "XmlUtil.h"
#include "Line.h"

namespace Netlist {

class Cell;
class Node;
class Line;

class Net {
public:
    Net(Cell*, const std::string&, Term::Type);
    ~Net();

    // Modificateurs
    void add    (Node*);
    bool remove (Node*);
    void add    (Line*);
    bool remove (Line*);
    // Accesseurs
    inline Cell*               getCell         () const;
    inline const std::string&  getName         () const;
    inline unsigned int        getId           () const;
    inline Term::Type          getType         () const;
    inline const std::vector<Node*>& getNodes  () const;
    inline const std::vector<Line*>& getLines  () const;
    size_t                     getFreeNodeId   () const;
    Node*                      getNode         (size_t) const;

    void                       toXml            (std::ostream&);
    static Net*                fromXml(Cell*, xmlTextReaderPtr);
    void                       printNodes();
private:
    Cell*        owner_;
    std::string  name_;
    unsigned int id_;
    Term::Type   type_;
    std::vector<Node*> nodes_;
    std::vector<Line*> lines_;
};

inline const std::vector<Line*>& Net::getLines () const { return lines_; }
inline Cell*              Net::getCell()         const { return owner_; }
inline const std::string& Net::getName()         const { return name_; }
inline unsigned int       Net::getId()           const { return id_; }
inline Term::Type         Net::getType()         const { return type_; }
inline const std::vector<Node*>& Net::getNodes() const { return nodes_; }

} // fin namespace

#endif