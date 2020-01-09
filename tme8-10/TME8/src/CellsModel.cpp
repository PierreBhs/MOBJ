#include "CellsModel.h"
#include <QFont>
#include <iostream>
#include <vector>

using namespace Netlist;



CellsModel::CellsModel(QObject* parent): QAbstractTableModel(parent), cell_(NULL), cellsCModel(Cell::getAllCells()){ 

}

CellsModel::~CellsModel() {

}

void CellsModel::setCell(Cell* cell) { 
    emit layoutAboutToBeChanged(); // emmission d'un signal Qt (lABTBC est un signal). Attention, stop le rafraichissment, je change ce que tu dois afficher 
    cell_ = cell;
    emit layoutChanged();          // Signale que les données sont stables, puis rafraichis toi (il faut les relire)
}


// Permettent de dimensionner le tableau à afficher
int CellsModel::rowCount(const QModelIndex& parent) const { 
    
    return Cell::getAllCells().size();  
}

int CellsModel::columnCount(const QModelIndex& parent) const { 
    return  1; 
}


// Renvoie une "information" (type spécifié par role) pour une cellule du tableau (index)
QVariant CellsModel::data(const QModelIndex& index, int  role ) const {
    
    cellsCModel = Cell::getAllCells();
    if (/*not cell_ or */not index.isValid())
        return  QVariant();

    int row = index.row();
    int col = index.column();
    if (role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;

    if(role == Qt::DisplayRole) { // role -> quelle est l'information qu'on veut pour la case donnée
        //int row = index.row();
        switch(col) {
            case 0:  
                return cellsCModel[row]->getName().c_str();
        }
    }
    return QVariant();
}

// titre entete du tab
QVariant CellsModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if(orientation == Qt::Vertical) 
        return  QVariant();

    if (role != Qt::DisplayRole) 
        return  QVariant();

    switch(section) { 
        case 0: 
            return "Cell";
    }
    return  QVariant ();
}


Cell* CellsModel::getModel(int row) {
    /* if(not cell_) {
        std::cout << "nul comme quentin\n";
        return  nullptr;
    } */
    if(row >= (int)cellsCModel.size())  
        return  nullptr;
    
    return  cellsCModel[row];
}

void CellsModel::updateCellModelData() {
    emit layoutChanged();
}

