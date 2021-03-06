#pragma once

#include <QAbstractTableModel>
#include <QModelIndex>
#include <QVariant>
#include <QPainter>
#include <QItemDelegate>
#include "Cell.h"
#include "Instance.h"



using namespace Netlist;

class  InstancesModel : public  QAbstractTableModel { 
    Q_OBJECT;
public:
                InstancesModel   ( QObject* parent=NULL );
                ~InstancesModel  ();

    // Nos méthodes
    void        setCell          ( Cell* );
    Cell*       getModel         (int row);

    // Méthodes à surcharger car elles sont héritées de QAbstractModel
    int         rowCount         (const QModelIndex& parent=QModelIndex ()) const; //QModelIndex -> encapsulation d'index d'un point de vue ligne et colonne
    int         columnCount      (const QModelIndex& parent=QModelIndex ()) const; 
    QVariant    data             (const QModelIndex& index , int  role=Qt:: DisplayRole) const; // QVariant -> encapsule tous les types plus ou moins standards, conteneur multiforme
    QVariant    headerData       (int section, Qt::Orientation orientation, int role=Qt:: DisplayRole ) const;
    
private:
    Cell* cell_;
};