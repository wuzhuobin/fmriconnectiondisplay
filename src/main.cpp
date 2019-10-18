// std
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
// vtk
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkCamera.h>
#include <vtkAxesActor.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSTLReader.h>
#include <vtkProperty.h>
#include <vtkWindowToImageFilter.h>
#include <vtkPNGWriter.h>
// boost
#include <boost/tokenizer.hpp>
//
#include "vtkFmriConnectionDisplay.h"
typedef boost::tokenizer<boost::escaped_list_separator<char>> tokenizer;
typedef std::vector<std::vector<double>> matrix_d;
int main( int argc, char** argv )
{
  // std::string fileName = "D:/ccode/fmriconnectiondisplay/test/fmri_correlation_1.csv";
  std::string fileName = "test/fmri_correlation_1.csv";
  std::ifstream fin;
  fin.open(fileName, std::ios::in);
  std::string a_line;
  matrix_d a_matrix;
  while(std::getline(fin, a_line)) {
    a_matrix.push_back(matrix_d::value_type());
    tokenizer token(a_line);
    for(tokenizer::const_iterator cit = token.begin(); cit != token.end(); ++cit) {
      a_matrix.back().push_back(std::stod(*cit));
    }
  }

  vtkSmartPointer<vtkFmriConnectionDisplay> fmri_filter =
    vtkSmartPointer<vtkFmriConnectionDisplay>::New();
  fmri_filter->SetMatrix(&a_matrix);
  fmri_filter->Update();
  std::cout << *fmri_filter;
  std::cin.get();

  vtkSmartPointer<vtkPolyDataMapper> sphereMapper =
      vtkSmartPointer<vtkPolyDataMapper>::New();
  sphereMapper->SetInputConnection(fmri_filter->GetOutputPort(0));

  vtkSmartPointer<vtkActor> sphereActor =
      vtkSmartPointer<vtkActor>::New();
  sphereActor->SetMapper(sphereMapper);
  sphereActor->GetProperty()->SetColor(1, 0, 0);

  vtkSmartPointer<vtkPolyDataMapper> tubeMapper =
      vtkSmartPointer<vtkPolyDataMapper>::New();
  tubeMapper->SetInputConnection(fmri_filter->GetOutputPort(1));

  vtkSmartPointer<vtkActor> tubeActor =
      vtkSmartPointer<vtkActor>::New();
  tubeActor->SetMapper(tubeMapper);
  tubeActor->GetProperty()->SetColor(0.5, 0.5, 0.5);

  vtkSmartPointer<vtkSTLReader> stlReader1 = 
    vtkSmartPointer<vtkSTLReader>::New();
  // stlReader1->SetFileName("D:/ccode/fmriconnectiondisplay/resources/brain-left.stl");
  stlReader1->SetFileName("resources/brain-left.stl");

  vtkSmartPointer<vtkPolyDataMapper> leftMapper = 
    vtkSmartPointer<vtkPolyDataMapper>::New();
  leftMapper->SetInputConnection(stlReader1->GetOutputPort());

  vtkSmartPointer<vtkActor> leftActor =
    vtkSmartPointer<vtkActor>::New();
  leftActor->SetMapper(leftMapper);
  leftActor->GetProperty()->SetOpacity(0.3);

  vtkSmartPointer<vtkSTLReader> stlReader2 = 
    vtkSmartPointer<vtkSTLReader>::New();
  // stlReader2->SetFileName("D:/ccode/fmriconnectiondisplay/resources/brain-right.stl");
  stlReader2->SetFileName("resources/brain-right.stl");

  vtkSmartPointer<vtkPolyDataMapper> rightMapper = 
    vtkSmartPointer<vtkPolyDataMapper>::New();
  rightMapper->SetInputConnection(stlReader2->GetOutputPort());

  vtkSmartPointer<vtkActor> rightActor =
    vtkSmartPointer<vtkActor>::New();
  rightActor->SetMapper(rightMapper);
  rightActor->GetProperty()->SetOpacity(0.3);

  vtkSmartPointer<vtkAxesActor> axesActor =
    vtkSmartPointer<vtkAxesActor>::New();
  axesActor->SetTotalLength(100, 100, 100);

  vtkSmartPointer<vtkRenderer> renderer =
      vtkSmartPointer<vtkRenderer>::New();
  renderer->AddActor(sphereActor);
  renderer->AddActor(tubeActor);
  renderer->AddActor(leftActor);
  renderer->AddActor(rightActor);
  renderer->AddActor(axesActor);

  vtkSmartPointer<vtkRenderWindow> renderWindow =
      vtkSmartPointer<vtkRenderWindow>::New();
  // renderWindow->OffScreenRenderingOn();
  renderWindow->AddRenderer(renderer);
  renderWindow->SetSize(1024, 1024);
  renderWindow->Render();

  vtkSmartPointer<vtkWindowToImageFilter> windowToImageFIlter =
    vtkSmartPointer<vtkWindowToImageFilter>::New();
  windowToImageFIlter->SetInput(renderWindow);

  vtkSmartPointer<vtkPNGWriter> pngWriter =
    vtkSmartPointer<vtkPNGWriter>::New();
  pngWriter->SetInputConnection(windowToImageFIlter->GetOutputPort());
  pngWriter->SetFileName("test.png");
  pngWriter->Write();

  renderer->GetActiveCamera()->SetPosition(0, 0, 1);
  // renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
  renderer->ResetCamera();
  renderer->ResetCameraClippingRange();
  renderWindow->Render();

  windowToImageFIlter->Modified();
  pngWriter->SetFileName("fmri-connection-superior.png");
  pngWriter->Write();


  renderer->GetActiveCamera()->SetPosition(0, 0, -1);
  // renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
  renderer->ResetCamera();
  renderer->ResetCameraClippingRange();
  renderWindow->Render();

  windowToImageFIlter->Modified();
  pngWriter->SetFileName("fmri-connection-anterior.png");
  pngWriter->Write();

  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
      vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

  renderWindowInteractor->Start();

  return 0;
}
