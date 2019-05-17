#include <QShortcut>
#include "aboutdialog.h"
#include "ui_aboutdialog.h"

aboutDialog::aboutDialog( QWidget *parent ) :
  QDialog( parent ),
  ui( new Ui::aboutDialog ) {
  //QShortcut *shortcut = new QShortcut( QKeySequence( Qt::Key_Escape ), ui->exitButton, SLOT( click( ) ) );
  //shortcut->setAutoRepeat( false );
  //ui->exitButton->setShortcut( shortcut );
  ui->setupUi( this );
}

aboutDialog::~aboutDialog( ) {
  delete ui;
}

void aboutDialog::on_exitButton_clicked( ) {
   this->close( );
}
