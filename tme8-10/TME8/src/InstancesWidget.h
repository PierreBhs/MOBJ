#pragma once

#include <QWidget>
#include <QTableView>
#include <QHeaderView>
#include <QPushButton>

#include "Cell.h"
#include "CellViewer.h"
#include "InstancesModel.h"

class CellViewer;
class InstancesModel;


class InstancesWidget : public QWidget {
    Q_OBJECT;
  public:
                  InstancesWidget ( QWidget* parent=NULL );
    inline  void  setCellViewer   ( CellViewer* );
            int   getSelectedRow  () const; // quelle est la rangée qui est selectionnée
    inline  void  setCell         ( Cell* );
  public slots:
            void  load            ();       // slot qui provoque le rafraichissement des données apres le setCell
  private:
            CellViewer*     cellViewer_;
            InstancesModel* baseModel_;
            QTableView*     view_;
            QPushButton*    load_; // peut etre inutile car se connecte avec le constructeur
};

inline void InstancesWidget::setCell        (Cell* c) { baseModel_->setCell(c);}
inline void InstancesWidget::setCellViewer  (CellViewer* c) { cellViewer_= c;}