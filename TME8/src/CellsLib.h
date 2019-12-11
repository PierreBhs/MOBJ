#pragma once

#include <QWidget>
#include <QTableView>
#include <QHeaderView>
#include <QPushButton>

#include "Cell.h"
#include "CellViewer.h"
#include "CellsModel.h"

class CellViewer;
class CellsModel;

class CellsLib : public QWidget {
    Q_OBJECT;
  public:
                       CellsLib       ( QWidget* parent=NULL );
           void        setCellViewer  ( CellViewer* );
           int         getSelectedRow () const;
    inline CellsModel* getBaseModel   ();
  public slots:
           void        load           ();
  private:
    CellViewer*  cellViewer_;
    CellsModel*  baseModel_;
    QTableView*  view_;
    QPushButton* load_;
};

