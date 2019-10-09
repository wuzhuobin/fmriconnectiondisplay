#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vtkRenderWindow.h>
#include <vtkRenderer.h>

#include <QMainWindow>

class Ui_MainWindow;

class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  MainWindow();
  ~MainWindow();

  void VisualizeStl();

  void VisualizeSphere(double* xyz);

public slots:

private:
  // Designer form
  Ui_MainWindow* ui;

  vtkRenderWindow*	m_renderwindow;
  vtkRenderer*		m_renderer;
};

#endif //MAINWINDOW_H
