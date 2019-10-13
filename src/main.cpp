// std
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
// vtk
#include <vtkSmartPointer.h>
// boost
#include <boost/tokenizer.hpp>
//
#include "vtkFmriConnectionDisplay.h"
typedef boost::tokenizer<boost::escaped_list_separator<char>> tokenizer;
typedef std::vector<std::vector<double>> matrix_d;
int main( int argc, char** argv )
{
  std::string fileName = "D:/ccode/fmriconnectiondisplay/test/fmri_correlation.csv";
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
  return 0;
}
