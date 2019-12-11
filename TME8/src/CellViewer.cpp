#include "CellViewer.h"

using namespace Netlist;

CellViewer::CellViewer( QWidget * parent ): QMainWindow(parent), cellWidget_(NULL), saveCellDialog_(NULL), cells_() {
    cellWidget_     = new CellWidget();
    instancesWidget_= new InstancesWidget();
    cellsLib_       = new CellsLib();
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

    action = new QAction("Instances", this);
    action->setStatusTip("Tableau d'instances");
    action->setShortcut(QKeySequence("CTRL+I"));
    action->setVisible(true);
    fileMenu->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(showInstancesWidget()));

    action = new QAction("Cells", this);
    action->setStatusTip("Big titties no milk");
    action->setShortcut(QKeySequence("CTRL+L"));
    action->setVisible(true);
    fileMenu->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(showCellsLib()));

    action = new QAction( "Quit", this);
    action->setStatusTip( "Exit the Netlist Viewer " );
    action->setShortcut(QKeySequence("CTRL+Q"));
    action->setVisible( true );
    fileMenu->addAction( action );
    connect( action , SIGNAL(triggered ()) , this, SLOT(close()));
    // Fin Menu Bar File

    setWindowTitle(tr("SESIISES"));
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
        //emit CellLoaded();
        this->setCell(cell);     
    }
}

void CellViewer::showInstancesWidget() {
    instancesWidget_->setCellViewer(this);
    instancesWidget_->setCell(getCell());
    instancesWidget_->show();

}

void CellViewer::showCellsLib() {
    cellsLib_->show();
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