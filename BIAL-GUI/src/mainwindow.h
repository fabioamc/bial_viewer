#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "controller.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QFileInfo>
#include <QMainWindow>
#include <QLabel>
#include "controlswidget.h"
#include "labelswidget.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow( QWidget *parent = 0 );
  ~MainWindow( );
  void commandLineOpen( const QCommandLineParser &parser,
                        const QCommandLineOption &label );
  bool loadLabel( QString filename );
  
private:
  void createConnections( );
  void setupLogoview( );
  /**
   * @brief getImageFileDialog: Dialog to open an label.
   * @return Filename.
   */
  QString getImageFileDialog( );
  /**
   * @brief getLoadFileDialog: Dialog to open an label.
   * @return Filename.
   */
  QString getLabelFileDialog( );
  /**
   * @brief getExportPixmapDialog: Dialog to export pixmap.
   * @return Filename.
   */
  QString getExportPixmapDialog();
  bool loadImage( QString filename );
  void createActions( );

signals:
  void updateProgress( int p );

public slots:
  /**
   * @brief updateMessageBar: Update messagebar with overlay, image, and label information.
   */
  void updateMessageBar();

private slots:
  void on_actionOpen_image_triggered( );
  void on_actionQuit_triggered( );
  void updateWidgets( );
  void updateMainWindowUIElements( );
  void readSettings( );
  void openRecentImage( );
  void openRecentLabel();
  void updateRecentImageActions( );
  void updateRecentLabelActions();
  void on_actionAddLabel_triggered( );
  /**
   * @brief writeMessageBar: Writes axis, coordinates and intensity of current cursor click position.
   * @param scnPos: Position of the click.
   * @param buttons: Mouse buttons.
   */
  void writeMessageBar( QString &msg );
  /**
   * @brief writePixelDataMessageBar: writes axis, coordinates and intensity of input pixel, and current axis.
   * @param pxl: Input pixel.
   * @param axis: current axis.
   */
  void writePixelDataMessageBar( Bial::Point3D &pxl, size_t axis );
  /**
   * @brief writeMessageBar: Writes string to message bar
   * @param msg: string to be written.
   */
  void mouseClickMessageBar( QPointF scnPos, Qt::MouseButtons buttons );
  void on_actionAxial_triggered( );
  void on_actionCoronal_triggered( );
  void on_actionSagittal_triggered( );
  void on_actionToggle_overlay_triggered( );
  void actionChange_default_parameters_triggered( );
  void on_actionShow_labels_dock_triggered();
  void on_actionLabels_dock_triggered();

  void on_actionExport_triggered();

  void on_actionAbout_triggered();

private:
  Ui::MainWindow *ui;
  Controller *m_controller;
  QString defaultImageFolder;
  QString defaultLabelFolder;
  QAction *recentImageActs[ Controller::MaxRecentFiles ];
  QAction *recentLabelActs[ Controller::MaxRecentFiles ];
  bool checkExtension( const QFileInfo &fileInfo );
  ControlsWidget *controlsWidget;
  QDockWidget *controlsDock;
  LabelsWidget *labelsWidget;
  QDockWidget *labelsDock;
  QLabel *statusBarLabel; // Permanent widget in message bar.
};

class CursorChanger {
public:
  explicit CursorChanger( const QCursor &cursor );
  ~CursorChanger( );
};

#endif /* MAINWINDOW_H */
