#include "InstancesModel.h"
#include <QFont>
#include <stdlib.h>     /* srand, rand */
#include <time.h> 
#include <iostream>

using namespace Netlist;

//srand(time(nullptr));
int rand_color() {
    return rand() % 256;
}

InstancesModel::InstancesModel(QObject* parent): QAbstractTableModel(parent), cell_(NULL) { 
    
}

InstancesModel::~InstancesModel() {

}

void InstancesModel::setCell(Cell* cell) { 
    emit layoutAboutToBeChanged(); // emmission d'un signal Qt (lABTBC est un signal). Attention, stop le rafraichissment, je change ce que tu dois afficher 
    cell_ = cell;
    emit layoutChanged();          // Signale que les données sont stables, puis rafraichis toi (il faut les relire)
}


// Permettent de dimensionner le tableau à afficher
int InstancesModel::rowCount(const QModelIndex& parent) const { 
    return (cell_) ? cell_->getInstances().size() : 0;  
}

int InstancesModel::columnCount(const QModelIndex& parent) const { 
    return  2; 
}


// Renvoie une "information" (type spécifié par role) pour une cellule du tableau (index)
QVariant InstancesModel::data(const QModelIndex& index, int  role ) const {
    
    if (not cell_ or not index.isValid())
        return  QVariant();

    int row = index.row();
    int col = index.column();

    if (role == Qt::TextColorRole) {
        if (col == 0) {
            return QColor(rand_color(), rand_color(), rand_color());
        }
    }
    if (role == Qt::BackgroundColorRole) {
        if (col == 1) {
            return QColor(rand_color(), rand_color(), rand_color());
            }
    }

    if(role == Qt::FontRole) {
            QFont boldFont;
            boldFont.setBold(true);
            return boldFont;
        
    }

    if (role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;

    if(role == Qt::DisplayRole) { // role -> quelle est l'information qu'on veut pour la case donnée
        //int row = index.row();
        switch(index.column()) {
            case 0:  
                return  cell_->getInstances()[row]->getName().c_str();
            case 1:  
                return  cell_->getInstances()[row]->getMasterCell()->getName().c_str();
        }
    }
    return QVariant();
}

// titre entete du tab
QVariant InstancesModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if(orientation == Qt::Vertical) 
        return  QVariant();

    if (role != Qt::DisplayRole) 
        return  QVariant();

    switch(section) { 
        case 0: 
            return "Instance";
        case 1:  
            return "MasterCell";
    }
    return  QVariant ();
}


Cell* InstancesModel::getModel(int row) {
    if(not cell_) 
        return  nullptr;
    if(row >= (int)cell_->getInstances().size())  
        return  nullptr;
    
    return  cell_->getInstances()[row]->getMasterCell();
}

