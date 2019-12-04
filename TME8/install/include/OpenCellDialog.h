#include <QDialog>
#include <QLineEdit>
#include <string>

class OpenCellDialog : public QDialog {
    Q_OBJECT;
public :
                    OpenCellDialog  (QWidget * parent = NULL);
    bool            run             (std::string&);
    const QString   getCellName     () const ;
    void            setCellName     ( const QString & );
protected :
    QLineEdit * lineEdit_ ;
};