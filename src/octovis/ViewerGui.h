// $Id$

/**
* Octomap:
* A  probabilistic, flexible, and compact 3D mapping library for robotic systems.
* @author K. M. Wurm, A. Hornung, University of Freiburg, Copyright (C) 2009.
* @see http://octomap.sourceforge.net/
* License: GNU GPL v2, http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*/

/*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
* for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#ifndef VIEWERGUI_H
#define VIEWERGUI_H

#include <QtGui/QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QDockWidget>
#include <string>
#include "TrajectoryDrawer.h"
#include "PointcloudDrawer.h"
#include "ViewerWidget.h"
#include "ViewerSettings.h"
#include "ViewerSettingsPanel.h"
#include "ui_ViewerGui.h"

namespace octomap {

class ViewerGui : public QMainWindow {
  Q_OBJECT

 public:
  ViewerGui(const std::string& filename="", QWidget *parent = 0);
  ~ViewerGui();

  static const unsigned int LASERTYPE_URG  = 0;
  static const unsigned int LASERTYPE_SICK = 1;


 public slots:
  void changeTreeDepth(int depth);
  void addNextScans(unsigned scans);
  void gotoFirstScan();


 private slots:

  // auto-connected Slots (by name))

  void on_actionExit_triggered();
  void on_actionOpen_file_triggered();
  void on_actionOpen_graph_incremental_triggered();
  void on_actionSave_file_triggered();
  void on_actionExport_view_triggered();
  void on_actionHelp_triggered();
  void on_actionSettings_triggered();
  void on_actionPrune_tree_triggered();
  void on_actionExpand_tree_triggered();
  void on_actionConvert_ml_tree_triggered();
  void on_actionReload_Octree_triggered();
  void on_actionPrintout_mode_toggled(bool checked);
  void on_actionHeight_map_toggled(bool checked);
  void on_actionStore_camera_triggered();
  void on_actionRestore_camera_triggered();
  void on_actionPointcloud_toggled(bool checked);
  void on_actionTrajectory_toggled(bool checked);

  // use it for testcases etc.
  void on_actionTest_triggered();

  signals:
   void updateStatusBar(QString message, int duration);
   void changeNumberOfScans(unsigned scans);
   void changeCurrentScan(unsigned scans);
   void changeResolution(double resolution);

 private:
  /**
   * (Re-)load the data file stored in m_fileName.
   * Depending on the extension, the respective load function is used.
   */
  void openFile();

  /**
   * Reads in a .dat file which consists of single points in ASCII,
   * one point per line, values separated by white spaces
   */
  void openPointcloud();

  /**
   * Opens a .graph file and generates a ScanGraph from it. Afterwards,
   * loadGraph() is called.
   */
  void openGraph(bool completeGraph = true);

  /**
   * Finishes loading a ScanGraph, either from .log or .graph.
   */
  void loadGraph(bool completeGraph = true);

  /**
   * Adds a scan from the graph to the OcTree
   */
  void addNextScan();

  /**
   * Opens a .pc PointCloud
   */
  void openPC();


  // open binary file containing an octree
  void openTree();

  /*!
   * (Re-)generates OcTree from the internally stored ScanGraph
   */
  void generateOctree();
  void showOcTree();

  void showInfo(QString string, bool newline=false);


  octomap::ScanGraph* m_scanGraph;
  octomap::OcTree*    m_ocTree;
  octomap::ScanGraph::iterator m_nextScanToAdd;

  Ui::ViewerGuiClass ui;
  ViewerWidget* m_glwidget;
  TrajectoryDrawer* m_trajectoryDrawer;
  PointcloudDrawer* m_pointcloudDrawer;
  double m_octreeResolution;
  double m_occupancyThresh; // FIXME: This is not really used at the moment...
  unsigned int m_max_tree_depth;
  unsigned int m_laserType; // SICK or Hokuyo /URG
  bool m_cameraStored;
  QLabel* m_mapSizeStatus;
  QLabel* m_mapMemoryStatus;

  //! Filename of last loaded file, in case it is necessary to reload it (no matter what kind of log)
  std::string m_filename;

};

}

#endif // VIEWERGUI_H