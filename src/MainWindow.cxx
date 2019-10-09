#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <vtkCamera.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkSphereSource.h>
#include <vtkSTLReader.h>
#include <vtkSmartPointer.h>

// Constructor
MainWindow::MainWindow()
{
  this->ui = new Ui_MainWindow;
  this->ui->setupUi(this);

  m_renderwindow	= vtkRenderWindow::New();
  m_renderer		= vtkRenderer::New();
  this->ui->qvtkWidget->SetRenderWindow(m_renderwindow);
  this->ui->qvtkWidget->GetRenderWindow()->AddRenderer(m_renderer);

  this->VisualizeStl();
}

MainWindow::~MainWindow()
{
	m_renderwindow->Delete();
	m_renderer->Delete();
}

void MainWindow::VisualizeStl()
{
	vtkSmartPointer<vtkSTLReader> reader = vtkSmartPointer<vtkSTLReader>::New();
	reader->SetFileName("E:/TestProgram/TestingRegistration/resources/Brain.stl");
	reader->Update();

	// Visualize
	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(reader->GetOutputPort());

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);

	m_renderer->AddActor(actor);
	m_renderer->ResetCameraClippingRange();
	m_renderer->ResetCamera();
}

void MainWindow::VisualizeSphere(double * xyz)
{
	vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
	sphereSource->Update();

	vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	sphereMapper->SetInputConnection(sphereSource->GetOutputPort());

	vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();
	sphereActor->SetMapper(sphereMapper);

	m_renderer->AddActor(sphereActor);
}
