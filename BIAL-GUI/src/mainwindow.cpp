#include "../ui/aboutdialog.h"
#include "Color.hpp"
#include "controller.h"
#include "controlswidget.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "imagewidget.h"

#include <QDockWidget>
#include <QFileDialog>
#include <QFileInfoList>
#include <QGraphicsPixmapItem>
#include <QImageReader>
#include <QMessageBox>
#include <QProgressDialog>
#include <QSettings>
#include "navigationtool.h"

MainWindow::MainWindow( QWidget *parent ) :
    QMainWindow( parent ), ui( new Ui::MainWindow ), m_controller( new Controller( 1, this ) ), defaultImageFolder( ),
    recentImageActs( ), recentLabelActs( ), controlsWidget( new ControlsWidget( m_controller, this ) ),
    controlsDock( new QDockWidget( tr( "Controls" ), this ) ), labelsWidget( new LabelsWidget( this ) ),
    labelsDock( new QDockWidget( tr( "Labels" ), this ) ), statusBarLabel( new QLabel( ) ) {
  BGN_CMT( "Begin.", 0 );
  ui->setupUi( this );
  COMMENT( "Adding docks.", 0 );
  setDockOptions( DockOption::AllowNestedDocks | DockOption::AllowTabbedDocks );
  controlsDock->setWidget( controlsWidget );
  controlsDock->hide( );
  labelsDock->setWidget( labelsWidget );
  labelsDock->hide( );
  addDockWidget( Qt::LeftDockWidgetArea, labelsDock, Qt::Vertical );
  addDockWidget( Qt::LeftDockWidgetArea, controlsDock, Qt::Vertical );
  ui->statusBar->addPermanentWidget( statusBarLabel );
  COMMENT( "Linking Controller with ImageViewer.", 0 );
  ui->imageViewer->linkToController( m_controller );
  //ui->actionExport->setEnabled( false );
  COMMENT( "Setting logo.", 0 );
  setupLogoview( );
  COMMENT( "Creating connections.", 0 );
  createConnections( );
  COMMENT( "Reading settings.", 0 );
  readSettings( );
  COMMENT( "Creating actions.", 0 );
  createActions( );
  COMMENT( "Updating MW elements.", 0 );
  updateMainWindowUIElements( );
  END_CMT( "End.", 0 );
}

MainWindow::~MainWindow( ) {
  BGN_CMT( "Begin.", 0 );
  delete ui;
  delete m_controller;
  delete controlsWidget;
  delete controlsDock;
  delete labelsWidget;
  delete labelsDock;
  END_CMT( "End.", 0 );
}

void MainWindow::createConnections( ) {
  /* Show/Hide docks. */
  BGN_CMT( "Begin.", 0 );
  connect( ui->actionShow_controls_dock, &QAction::toggled, controlsDock, &QDockWidget::setVisible );
  connect( ui->actionShow_labels_dock, &QAction::toggled, labelsDock, &QDockWidget::setVisible );
  connect( controlsDock, &QDockWidget::visibilityChanged, ui->actionShow_controls_dock, &QAction::setChecked );
  connect( labelsDock, &QDockWidget::visibilityChanged, ui->actionShow_labels_dock, &QAction::setChecked );
  connect( labelsWidget, &LabelsWidget::updatePixMap, m_controller, &Controller::updatePixMap );
  /* Controller. */
  connect( m_controller, &Controller::pixmapUpdated, this, &MainWindow::updateWidgets );
  connect( m_controller, &Controller::updateMainWindowUIElements, this, &MainWindow::updateMainWindowUIElements );
  connect( m_controller, &Controller::recentImagesUpdated, this, &MainWindow::updateRecentImageActions );
  connect( m_controller, &Controller::recentLabelsUpdated, this, &MainWindow::updateRecentLabelActions );
  /* ImageViewer */
  connect( ui->imageViewer, &ImageViewer::mouseClicked, this, &MainWindow::mouseClickMessageBar );
  connect( ui->imageViewer, &ImageViewer::mouseReleased, this, &MainWindow::mouseClickMessageBar );
  connect( ui->imageViewer, &ImageViewer::mouseDragged, this, &MainWindow::mouseClickMessageBar );
  connect( ui->logoView, &GraphicsView::dropImage, this, &MainWindow::loadImage );
  END_CMT( "End.", 0 );
}

void MainWindow::setupLogoview( ) {
  BGN_CMT( "Begin.", 0 );
  QGraphicsScene *scn = new QGraphicsScene( this );
  QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem( QPixmap( ":/icons/logo_shadow.png" ) );
  scn->addItem( pixmapItem );
  scn->setSceneRect( 0, 0, pixmapItem->pixmap( ).width( ), pixmapItem->pixmap( ).height( ) );
  ui->logoView->setScene( scn );
  END_CMT( "End.", 0 );
}

void MainWindow::updateWidgets( ) {
  BGN_CMT( "Begin.", 0 );
  GuiImage *gi = m_controller->currentGuiImage( );
  COMMENT( "Updating labelsWidget with new image and/orlabel.", 0 );
  labelsWidget->setGuiImage( gi );
  COMMENT( "Show/hide dock.", 0 );
  if( gi->hasLabel( ) ) {
    labelsDock->show( );
    ui->actionShow_labels_dock->setEnabled( true );
  }
  else {
    labelsDock->hide( );
    ui->actionShow_labels_dock->setDisabled( true );
  }
  COMMENT( "Updating message bar.", 0 );
  updateMessageBar( );
  END_CMT( "End.", 0 );
}

void MainWindow::updateMessageBar( ) {
  BGN_CMT( "Begin.", 0 );
  QPointF pt = ui->imageViewer->overlayPosition( );
  mouseClickMessageBar( pt, Qt::MouseButtons( ) );
  END_CMT( "End.", 0 );
}

void MainWindow::updateMainWindowUIElements( ) {
  BGN_CMT( "Begin", 0 );
  bool hasImage = m_controller->hasGuiImage( );
  COMMENT( "Has Image = " << hasImage, 0 );
  ui->menuWindow->setEnabled( hasImage );
  controlsDock->setVisible( hasImage );
  ui->logoView->setVisible( !hasImage );
  ui->imageViewer->setVisible( hasImage );
  ui->menuLayout->setEnabled( hasImage );
  ui->menuOverlay->setEnabled( hasImage );
  ui->actionExport->setEnabled( hasImage );
  ui->actionAddLabel->setEnabled( hasImage );
  ui->actionShow_controls_dock->setEnabled( hasImage );
  if( hasImage ) {
    COMMENT( "Calling updateWidgets.", 0 );
    updateWidgets( );
    GuiImage *gi = m_controller->currentGuiImage( );
    QFileInfo finfo = gi->fileName( );
    setWindowTitle( QString( "%1 - BIAL" ).arg( finfo.fileName( ) ) );
  }
  END_CMT( "End.", 0 );
}

void MainWindow::on_actionOpen_image_triggered( ) {
  BGN_CMT( "Begin.", 0 );
  QString fileName = getImageFileDialog( );
  if( fileName.isEmpty( ) ) {
    END_CMT( "End.", 0 );
    return;
  }
  if( !loadImage( fileName ) ) {
    QMessageBox::warning( this, "Warning", tr( "Could not open file!" ) );
    END_CMT( "End.", 0 );
    return;
  }
}

QString MainWindow::getImageFileDialog( ) {
  BGN_CMT( "Begin.", 0 );
  END_CMT( "End.", 0 );
  QFileDialog fileDialog;
  return( fileDialog.getOpenFileName(
            this, tr( "Open" ), defaultImageFolder,
            tr( "All images (*.dcm *.dcm.gz *.nii *.nii.gz *.scn *.scn.gz );;"
                "DICOM images (*.dcm *.dcm.gz);;"
                "NIfTI images (*.nii *.nii.gz);;"
                "SCN Files (*.scn *.scn.gz);;"
                "All files (*)" ) ) );
}

QString MainWindow::getLabelFileDialog( ) {
  BGN_CMT( "Begin.", 0 );
  END_CMT( "End.", 0 );
  QFileDialog fileDialog;
  return( fileDialog.getOpenFileName(
            this, tr( "Open" ), defaultLabelFolder,
            tr( "All images (*.dcm *.dcm.gz *.nii *.nii.gz *.scn *.scn.gz );;"
                "DICOM images (*.dcm *.dcm.gz);;"
                "NIfTI images (*.nii *.nii.gz);;"
                "SCN Files (*.scn *.scn.gz);;"
                "All files (*)" ) ) );
}

QString MainWindow::getExportPixmapDialog( ) {
  BGN_CMT( "Begin.", 0 );
  QFileDialog fileDialog;
  fileDialog.setFilter( fileDialog.filter( ) );
  fileDialog.setFileMode( QFileDialog::ExistingFile );
  fileDialog.setDefaultSuffix( "png" );
  fileDialog.setAcceptMode( QFileDialog::AcceptSave );
  fileDialog.setNameFilter( "[ 'PNG Files (*.png)' ]" );
  if( fileDialog.exec( ) == QFileDialog::Accepted ) {
    END_CMT( "End.", 0 );
    return( fileDialog.selectedFiles( ).first( ) );
  }
  else {
    END_CMT( "End.", 0 );
    return( "" );
  }
}


bool MainWindow::loadImage( QString filename ) {
  BGN_CMT( "Begin. Loading file: " << filename.toStdString( ), 0 );
  GuiImage *img( m_controller->loadImage( filename ) );
  if( img != nullptr ) {
    COMMENT( "Deleting previous image.", 0 );
    if( m_controller->hasGuiImage( ) )
      m_controller->clearGuiImage( );
    COMMENT( "Setting new image to controller.", 0 );
    m_controller->setImage( img );
    COMMENT( "Setting default folder as the last accessed folder.", 0 );
    defaultImageFolder = QFileInfo( filename ).canonicalPath( );
    QSettings settings;
    settings.beginGroup( "MainWindow" );
    settings.setValue( "defaultImageFolder", defaultImageFolder );
    settings.endGroup( );
    COMMENT( "Enabling recent labels.", 0 );
    updateRecentLabelActions( );
    for( int i = 0; i < Controller::MaxRecentFiles; ++i ) {
      ui->menuRecent_labels->addAction( recentLabelActs[ i ] );
    }
    END_CMT( "End.", 0 );
    return( true );
  }
  END_CMT( "End.", 0 );
  return( false );
}

bool MainWindow::checkExtension( const QFileInfo &fileInfo ) { /* receive to lower */
  BGN_CMT( "Begin.", 0 );
  QStringList list;
  list << "scn" << "scn.gz" << "img" << "img.gz" << "hdr"
       << "hdr.gz" << "nii" << "nii.gz" << "dcm" << "dcm.gz";
  QString suffix = fileInfo.completeSuffix( ).toLower( );
  if( list.contains( suffix ) ) {
    END_CMT( "End.", 0 );
    return( true );
  }
  else {
    QImageReader reader( fileInfo.absoluteFilePath( ) );
    END_CMT( "End.", 0 );
    return( !reader.format( ).isEmpty( ) );
  }
}

void MainWindow::readSettings( ) {
  BGN_CMT( "Begin.", 0 );
  QSettings settings;
  settings.beginGroup( "MainWindow" );
  defaultImageFolder = settings.value( "defaultImageFolder" ).toString( );
  defaultLabelFolder = settings.value( "defaultLabelFolder" ).toString( );
  if( defaultImageFolder.isEmpty( ) ) {
    defaultImageFolder = QDir::homePath( );
  }
  if( defaultLabelFolder.isEmpty( ) ) {
    defaultLabelFolder = QDir::homePath( );
  }
  END_CMT( "End.", 0 );
}

void MainWindow::commandLineOpen( const QCommandLineParser &parser,
                                  const QCommandLineOption &label ) {
  BGN_CMT( "Begin.", 0 );
  const QStringList args = parser.positionalArguments( );
  QString errorMsg;
  if( !args.isEmpty( ) ) {
    for( QString arg : args ) {
      QFileInfo file( arg );
      if( file.exists( ) ) {
        if( file.isFile( ) ) {
          GuiImage *gi = m_controller->loadImage( file.absoluteFilePath( ) );
          if( gi != nullptr ) {
            m_controller->setImage( gi );
            ui->statusBar->showMessage( file.baseName( ) + " loaded succesfully as image.", 2000 );
          }
          else {
            errorMsg = "Error loading " + file.absolutePath( ) + ".";
            BIAL_WARNING( errorMsg.toStdString( ) );
          }
        }
        else {
          errorMsg = file.absoluteFilePath( ) + " is not a file.";
          BIAL_WARNING( errorMsg.toStdString( ) );
        }
      }
      else {
        errorMsg = file.absoluteFilePath( ) + " does not exist.";
        BIAL_WARNING( errorMsg.toStdString( ) );
      }
    }
    if( parser.isSet( label ) ) {
      QFileInfo file( parser.value( label ) );
      if( file.exists( ) ) {
        if( file.isFile( ) ) {
          if( loadLabel( file.absoluteFilePath( ) ) ) {
            ui->statusBar->showMessage( file.baseName( ) + " loaded succesfully as label.", 2000 );
          }
          else {
            errorMsg = "Error loading " + file.absoluteFilePath( ) + ".";
            BIAL_WARNING( errorMsg.toStdString( ) );
          }
        }
        else {
          errorMsg = file.absoluteFilePath( ) + " is not a file.";
          BIAL_WARNING( errorMsg.toStdString( ) );
        }
      }
      else {
        errorMsg = file.absoluteFilePath( ) + " does not exist.";
        BIAL_WARNING( errorMsg.toStdString( ) );
      }
    }
  }
  if( !errorMsg.isEmpty( ) ) {
    QMessageBox::warning( this, "Warning", errorMsg );
    ui->statusBar->showMessage( errorMsg, 5000 );
  }
  END_CMT( "End.", 0 );
}

void MainWindow::on_actionQuit_triggered( ) {
  BGN_CMT( "Begin.", 0 );
  close( );
  END_CMT( "End.", 0 );
}

void MainWindow::openRecentImage( ) {
  BGN_CMT( "Begin.", 0 );
  QAction *action = qobject_cast< QAction* >( sender( ) );
  if( action ) {
    QString fileName = action->data( ).toString( );
    loadImage( fileName );
  }
  END_CMT( "End.", 0 );
}

void MainWindow::openRecentLabel( ) {
  BGN_CMT( "Begin.", 0 );
  QAction *action = qobject_cast< QAction* >( sender( ) );
  if( action ) {
    QString fileName = action->data( ).toString( );
    loadLabel( fileName );
  }
  END_CMT( "End.", 0 );
}

void MainWindow::updateRecentImageActions( ) {
  BGN_CMT( "Begin.", 0 );
  QSettings settings;
  QStringList files = settings.value( "recentImageList" ).toStringList( );
  int numRecentFiles = qMin( files.size( ), static_cast< int >( Controller::MaxRecentFiles ) );
  if( numRecentFiles > 0 ) {
    ui->menuRecent_images->setEnabled( true );
  }
  for( int i = 0; i < numRecentFiles; ++i ) {
    QString text = QString( "&%1 %2" ).arg( i + 1 ).arg( QFileInfo( files[ i ] ).fileName( ) );
    recentImageActs[ i ]->setText( text );
    recentImageActs[ i ]->setData( files[ i ] );
    recentImageActs[ i ]->setVisible( true );
  }
  for( int i = numRecentFiles; i < Controller::MaxRecentFiles; ++i ) {
    recentImageActs[ i ]->setVisible( false );
  }
  END_CMT( "End.", 0 );
}

void MainWindow::updateRecentLabelActions( ) {
  BGN_CMT( "Begin.", 0 );
  QSettings settings;
  QStringList files = settings.value( "recentLabelList" ).toStringList( );
  int numRecentFiles = qMin( files.size( ), static_cast< int >( Controller::MaxRecentFiles ) );
  if( numRecentFiles > 0 ) {
    ui->menuRecent_labels->setEnabled( true );
  }
  for( int i = 0; i < numRecentFiles; ++i ) {
    QString text = QString( "&%1 %2" ).arg( i + 1 ).arg( QFileInfo( files[ i ] ).fileName( ) );
    recentLabelActs[ i ]->setText( text );
    recentLabelActs[ i ]->setData( files[ i ] );
    recentLabelActs[ i ]->setVisible( true );
  }
  for( int i = numRecentFiles; i < Controller::MaxRecentFiles; ++i ) {
    recentLabelActs[ i ]->setVisible( false );
  }
  END_CMT( "End.", 0 );
}

void MainWindow::createActions( ) {
  BGN_CMT( "Begin.", 0 );
  COMMENT( "Image file actions.", 0 );
  for( int i = 0; i < Controller::MaxRecentFiles; ++i ) {
    recentImageActs[ i ] = new QAction( this );
    recentImageActs[ i ]->setVisible( false );
    connect( recentImageActs[ i ], &QAction::triggered, this, &MainWindow::openRecentImage );
    ui->menuRecent_images->addAction( recentImageActs[ i ] );
  }
  updateRecentImageActions( );
  for( int i = 0; i < Controller::MaxRecentFiles; ++i ) {
    ui->menuRecent_images->addAction( recentImageActs[ i ] );
  }
  COMMENT( "Label file actions.", 0 );
  for( int i = 0; i < Controller::MaxRecentFiles; ++i ) {
    recentLabelActs[ i ] = new QAction( this );
    recentLabelActs[ i ]->setVisible( false );
    connect( recentLabelActs[ i ], &QAction::triggered, this, &MainWindow::openRecentLabel );
    ui->menuRecent_labels->addAction( recentLabelActs[ i ] );
  }
  END_CMT( "End.", 0 );
}

bool MainWindow::loadLabel( QString filename ) {
  BGN_CMT( "Begin.", 0 );
  bool status = m_controller->addLabel( filename );
  if( status ) {
    labelsWidget->needUpdate( );
    m_controller->setRecentLabel( filename );
    COMMENT( "Setting default folder as the last accessed folder.", 0 );
    defaultLabelFolder = QFileInfo( filename ).canonicalPath( );
    QSettings settings;
    settings.beginGroup( "MainWindow" );
    settings.setValue( "defaultLabelFolder", defaultLabelFolder );
    settings.endGroup( );
  }
  else {
    QFileInfo file( filename );
    ui->statusBar->showMessage( "Error while opening " + file.fileName( ), 2000 );
  }
  END_CMT( "End.", 0 );
  return( status );
}

void MainWindow::on_actionAddLabel_triggered( ) {
  BGN_CMT( "Begin.", 0 );
  QString filename = getLabelFileDialog( );
  if( ( !filename.isEmpty( ) ) && ( !loadLabel( filename ) ) ) {
    QMessageBox::warning( this, "Warning", tr( "Could not open label!" ) );
  }
  END_CMT( "End.", 0 );
}

void MainWindow::mouseClickMessageBar( QPointF scnPos, Qt::MouseButtons buttons ) {
  BGN_CMT( "Begin.", 0 );
//  scnPos.setX( std::round( scnPos.x( ) ) );
//  scnPos.setY( std::round( scnPos.y( ) ) );
  Q_UNUSED( buttons )
  if( ( m_controller->currentTool( ) != nullptr ) && ( m_controller->hasGuiImage( ) ) ) {
    COMMENT( "Getting axis number.", 0 );
    size_t axis = m_controller->currentTool( )->axisNumber( );
    COMMENT( "Getting position.", 0 );
    GuiImage *img = m_controller->currentGuiImage( );
    Bial::Point3D pxl = img->getPosition( scnPos, axis );
    writePixelDataMessageBar( pxl, axis );
  }
  END_CMT( "End.", 0 );
}

void MainWindow::writePixelDataMessageBar( Bial::Point3D &pxl, size_t axis ) {
  BGN_CMT( "Begin.", 0 );
  GuiImage *img = m_controller->currentGuiImage( );
  QString msg;
  int max = img->max( );
  const Bial::Image< int > &bial_img( img->getImage( ) );
  if( bial_img.ValidCoordinate( pxl.x, pxl.y, pxl.z ) ) {
    int img_intensity = bial_img( pxl.x, pxl.y, pxl.z );
    if( img->hasLabel( ) ) {
      const Bial::Image< int > &bial_label( img->getLabel( ) );
      int lbl_intensity = bial_label( pxl.x, pxl.y, pxl.z );
      msg = QString( "Axis: %1 -- Image Coordinates: (%2, %3, %4) -- Image Intensity: %5/%6 -- Label Value: %7" )
            .arg( axis )
            .arg( ( int ) pxl.x )
            .arg( ( int ) pxl.y )
            .arg( ( int ) pxl.z )
            .arg( img_intensity )
            .arg( max )
            .arg( lbl_intensity );
    }
    else
      msg = QString( "Axis: %1 -- Image Coordinates: (%2, %3, %4) -- Image Intensity: %5/%6" )
            .arg( axis )
            .arg( ( int ) pxl.x )
            .arg( ( int ) pxl.y )
            .arg( ( int ) pxl.z )
            .arg( img_intensity )
            .arg( max );
  }
  writeMessageBar( msg );
  END_CMT( "End.", 0 );
}

void MainWindow::writeMessageBar( QString &msg ) {
  BGN_CMT( "Begin.", 0 );
  statusBarLabel->setText( msg );
  //ui->statusBar->showMessage( msg, 10000 );
  END_CMT( "End.", 0 );
}

void MainWindow::on_actionAxial_triggered( ) {
  BGN_CMT( "Begin.", 0 );
  if( ui->imageViewer->axisNumber( ) != 0 ) {
    m_controller->setAxis( 0 );
    controlsWidget->updateUIAxisElements( 0 );
    COMMENT( "Emitting controller->updatePixMap.", 0 );
    emit( m_controller->updatePixMap( ) );
  }
  END_CMT( "End.", 0 );
}

void MainWindow::on_actionCoronal_triggered( ) {
  BGN_CMT( "Begin.", 0 );
  if( ui->imageViewer->axisNumber( ) != 1 ) {
    m_controller->setAxis( 1 );
    controlsWidget->updateUIAxisElements( 1 );
    COMMENT( "Emitting controller->updatePixMap.", 0 );
    emit( m_controller->updatePixMap( ) );
  }
  END_CMT( "End.", 0 );
}

void MainWindow::on_actionSagittal_triggered( ) {
  BGN_CMT( "Begin.", 0 );
  if( ui->imageViewer->axisNumber( ) != 2 ) {
    m_controller->setAxis( 2 );
    controlsWidget->updateUIAxisElements( 2 );
    COMMENT( "Emitting controller->updatePixMap.", 0 );
    emit( m_controller->updatePixMap( ) );
  }
  END_CMT( "End.", 0 );
}

void MainWindow::on_actionToggle_overlay_triggered( ) {
  BGN_CMT( "Begin.", 0 );
  ui->imageViewer->getScene( )->changeOverlay( );
  m_controller->setPixMap( );
  END_CMT( "End.", 0 );
}

void MainWindow::actionChange_default_parameters_triggered( ) {
  BGN_CMT( "Begin.", 0 );
  END_CMT( "End.", 0 );
}

CursorChanger::CursorChanger( const QCursor &cursor ) {
  BGN_CMT( "Begin.", 0 );
  QApplication::setOverrideCursor( cursor );
  END_CMT( "End.", 0 );
}

CursorChanger::~CursorChanger( ) {
  BGN_CMT( "Begin.", 0 );
  QApplication::restoreOverrideCursor( );
  END_CMT( "End.", 0 );
}

void MainWindow::on_actionShow_labels_dock_triggered( ) {
  BGN_CMT( "Begin.", 0 );
  END_CMT( "End.", 0 );
}

void MainWindow::on_actionLabels_dock_triggered( ) {
  BGN_CMT( "Begin.", 0 );
  END_CMT( "End.", 0 );
}

void MainWindow::on_actionExport_triggered( ) {
  BGN_CMT( "Begin.", 0 );
  QString fileName = getExportPixmapDialog( );
  COMMENT( "Filename: " << fileName.toStdString( ), 0 );
  QFileInfo file( fileName );
  if( ui->imageViewer->exportPixmap( fileName ) ) {
    ui->statusBar->showMessage( file.baseName( ) + " succesfully exported.", 2000 );
  }
  else {
    ui->statusBar->showMessage( "Error while exporting " + file.fileName( ), 2000 );
  }
  END_CMT( "End.", 0 );
}

void MainWindow::on_actionAbout_triggered( ) {
  BGN_CMT( "Begin.", 0 );
  aboutDialog dialog;
  dialog.setModal( true );
  dialog.exec( );
  //aboutDialog *dialog = new aboutDialog( this );
  //dialog->show( );
  END_CMT( "End.", 0 );
}
