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
#include <vtkRenderWindowInteractor.h>
#include <vtkSTLReader.h>
#include <vtkProperty.h>
// boost
#include <boost/tokenizer.hpp>
//
#include "vtkFmriConnectionDisplay.h"
typedef boost::tokenizer<boost::escaped_list_separator<char>> tokenizer;
typedef std::vector<std::vector<double>> matrix_d;
int main( int argc, char** argv )
{
  std::string fileName = "D:/ccode/fmriconnectiondisplay/test/fmri_correlation_1.csv";
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
    // a_matrix.push_back(static_cast<matrix_d::value_type &&>(a_vector));
  }
  // std::cout << a_matrix[0][0] << '\n';
  // tokenizer token(str);
  // for(const auto &t: token) {
  //   std::cout << t << '\n';
  // }
  vtkSmartPointer<vtkFmriConnectionDisplay> fmri_filter =
    vtkSmartPointer<vtkFmriConnectionDisplay>::New();
  fmri_filter->SetMatrix(&a_matrix);
  fmri_filter->Update();
  std::cout << *fmri_filter;
  std::cin.get();

  vtkSmartPointer<vtkPolyDataMapper> mapper =
      vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(fmri_filter->GetOutputPort());

  vtkSmartPointer<vtkActor> actor =
      vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  // actor->GetProperty()->SetColor(colors->GetColor3d("Cornsilk").GetData());

  vtkSmartPointer<vtkSTLReader> stlReader1 = 
    vtkSmartPointer<vtkSTLReader>::New();
  stlReader1->SetFileName("D:/ccode/fmriconnectiondisplay/resources/brain-left.stl");

  vtkSmartPointer<vtkPolyDataMapper> leftMapper = 
    vtkSmartPointer<vtkPolyDataMapper>::New();
  leftMapper->SetInputConnection(stlReader1->GetOutputPort());

  vtkSmartPointer<vtkActor> leftActor =
    vtkSmartPointer<vtkActor>::New();
  leftActor->SetMapper(leftMapper);
  leftActor->GetProperty()->SetOpacity(0.3);

  vtkSmartPointer<vtkSTLReader> stlReader2 = 
    vtkSmartPointer<vtkSTLReader>::New();
  stlReader2->SetFileName("D:/ccode/fmriconnectiondisplay/resources/brain-right.stl");

  vtkSmartPointer<vtkPolyDataMapper> rightMapper = 
    vtkSmartPointer<vtkPolyDataMapper>::New();
  rightMapper->SetInputConnection(stlReader2->GetOutputPort());

  vtkSmartPointer<vtkActor> rightActor =
    vtkSmartPointer<vtkActor>::New();
  rightActor->SetMapper(rightMapper);
  rightActor->GetProperty()->SetOpacity(0.3);

  vtkSmartPointer<vtkRenderer> renderer =
      vtkSmartPointer<vtkRenderer>::New();
  renderer->AddActor(actor);
  renderer->AddActor(leftActor);
  renderer->AddActor(rightActor);

  vtkSmartPointer<vtkRenderWindow> renderWindow =
      vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->SetWindowName("Sphere");
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
      vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

  // renderer->SetBackground(colors->GetColor3d("DarkGreen").GetData());

  renderWindow->Render();
  renderWindowInteractor->Start();

  return 0;
}
