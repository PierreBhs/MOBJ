#include <string>
#include <sstream>
#include <iostream>
#include <memory>
using namespace std;

#include "Term.h"
#include "Net.h"
#include "Instance.h"
#include "Cell.h"
using namespace Netlist;

#include <QApplication>
#include <QtGui>
#include "CellViewer.h"

void loadCells();

int main ( int argc , char * argv []) {

  loadCells();

  QApplication* qa   = new QApplication( argc , argv );
  CellViewer* viewer = new CellViewer();
  //viewer->setCell(or2);
  viewer->show();
  int rvalue = qa->exec();
  delete qa;
  return rvalue;
}

void loadCells() {
  Cell::load( "vdd" );
  Cell::load( "gnd" );
  Cell::load( "TransistorN" );
  Cell::load( "TransistorP" );
  Cell::load( "and2" );
  Cell::load( "or2" );
  Cell::load( "xor2" );
  Cell::load( "halfadder" );
}