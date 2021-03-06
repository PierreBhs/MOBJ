#include "CellsLib.h"
#include "CellsModel.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>
#include <QLabel>
#include <iostream>

CellsLib::CellsLib(QWidget* parent): QWidget(parent), cellViewer_(NULL), baseModel_(new CellsModel(this)), 
    view_(new QTableView(this)), load_(new QPushButton(this)) { 
    
    // Pas préconfigurée comme QDialog, on doit prendre des précautions 
    setAttribute(Qt::WA_QuitOnClose, false);    // Ne ferme pas l'application
    setAttribute(Qt::WA_DeleteOnClose, false);  // Ne détruit pas le Widget (simplement caché pour simplement faire un show() si on veut le revoir (sinon coredump généré))
    setContextMenuPolicy(Qt::ActionsContextMenu);

    // Customise la view
    view_->setShowGrid(true);
    view_->setAlternatingRowColors(true);
    view_->setSelectionBehavior(QAbstractItemView::SelectRows);
    view_->setSelectionMode(QAbstractItemView::SingleSelection);
    view_->setSortingEnabled(true);
    view_->setModel(baseModel_); // On  associe  le  modele
    view_->resizeColumnsToContents();
    

    QHeaderView* horizontalHeader = view_->horizontalHeader();
    horizontalHeader->setDefaultAlignment(Qt::AlignHCenter);
    horizontalHeader->setMinimumSectionSize(300);
    horizontalHeader->setStretchLastSection(true); // beau dimensionnement
    
    // pas afficher le header vertical
    QHeaderView* verticalHeader = view_->verticalHeader();
    verticalHeader->setVisible(false);
    
    // Assigne une action quand on clique sur load et design
    load_->setText("Load");
    connect(load_ , SIGNAL(clicked ()), this, SLOT(load ()));

    // Design & Layout du Widget
    setMinimumSize(480, 640);

    //layout
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(view_);
    layout->addWidget(load_);
    setLayout(layout);
    setWindowTitle(tr("Available Cells"));

}

// numéro de la ligne qui est sélectionnée
int CellsLib::getSelectedRow() const {
    QModelIndexList selecteds = view_->selectionModel()->selection().indexes();
    
    if(selecteds.empty()) 
        return  -1;
    
    return selecteds.first().row(); // on récupère la première et on récupère la ligne
}

void CellsLib::load() {
    int selectedRow = getSelectedRow();
    if (selectedRow < 0)  
        return;
        
    cellViewer_->setCell(baseModel_->getModel(selectedRow));
}
