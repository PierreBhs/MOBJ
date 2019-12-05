#include "CellViewer.h"
#include "CellWidget.h"

using namespace Netlist;

CellViewer::CellViewer( QWidget * parent ): QMainWindow(parent), cellWidget_(NULL), saveCellDialog_(NULL), cells_() {
    cellWidget_     = new CellWidget();
    saveCellDialog_ = new SaveCellDialog(this);
    openCellDialog_ = new OpenCellDialog(this);
    setCentralWidget(cellWidget_);


    // Menu Bar File
    QMenu* fileMenu = menuBar()->addMenu("File");
    QAction* action = new QAction("Save As" , this);
    action->setStatusTip(" Save to disk ( rename ) the Cell ");
    action->setShortcut(QKeySequence("CTRL+S"));
    action->setVisible(true);
    fileMenu->addAction(action);
    connect( action , SIGNAL(triggered()) , this, SLOT(saveCell()));

    action = new QAction("Open Cell" , this);
    action->setStatusTip(" Open the Cell ");
    action->setShortcut(QKeySequence("CTRL+O"));
    action->setVisible(true);
    fileMenu->addAction(action);
    connect(action , SIGNAL(triggered()) , this, SLOT(openCell()));

    action = new QAction( "Quit", this);
    action->setStatusTip( "Exit the Netlist Viewer " );
    action->setShortcut(QKeySequence("CTRL+Q"));
    action->setVisible( true );
    fileMenu->addAction( action );
    connect( action , SIGNAL(triggered ()) , this, SLOT(close()));
    // Fin Menu Bar File
}

CellViewer::~CellViewer() {

}

Cell* CellViewer::getCell() const {
    return cellWidget_->getCell();
}

void  CellViewer::setCell(Cell* c) {
    cellWidget_->setCell(c);
}

void  CellViewer::openCell() {
    std::string cellStr;
    Cell* cell;
    if (openCellDialog_->run(cellStr)) {
        if(!(cell = Cell::find(cellStr)))
            cell = Cell::load(cellStr);

        this->setCell(cell);     
    }
}

// Sauvegarde la cell
void CellViewer::saveCell()
{
    Cell * cell = getCell();
    if (cell == NULL) 
        return ;
    
    QString cellName = cell->getName().c_str();
    if (saveCellDialog_->run(cellName)) {
        cell->setName (cellName.toStdString());
        cell->save(cellName.toStdString());
    }
}