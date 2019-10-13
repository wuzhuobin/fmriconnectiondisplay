#include "vtkFmriConnectionDisplay.h"
// vtk
#include <vtkObjectFactory.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkLineSource.h>
// std
#include <algorithm>
#include <numeric>
#include <functional>
vtkStandardNewMacro(vtkFmriConnectionDisplay);


vtkFmriConnectionDisplay::vtkFmriConnectionDisplay() {
  this->Matrix = nullptr;
  this->SetNumberOfInputPorts(0);
}

vtkFmriConnectionDisplay::~vtkFmriConnectionDisplay() {

}

void vtkFmriConnectionDisplay::PrintSelf(ostream & os, vtkIndent indent) {
  this->Superclass::PrintSelf(os, indent);
  if(this->Matrix != nullptr) {
    os << indent << "Matrix is " << this->Matrix->size() << 'x' << this->Matrix->front().size() << '\n';
    // for(CorrelationMatrix::value_type & vec: *this->Matrix) {
    //   os << indent.GetNextIndent();
    //   os << vec.size();
    //   for(CorrelationMatrix::value_type::value_type & v: vec) {
    //     os << v;
    //   }
    //   os << '\n';
    // }
  }
}

int vtkFmriConnectionDisplay::RequestData(
  vtkInformation *vtkNotUsed(request),
  vtkInformationVector **vtkNotUsed(inputVector),
  vtkInformationVector *outputVector) {
  
  if(!this->Matrix) {
    vtkErrorMacro("The matrix is null.");
    return 0;
  }
  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  vtkPolyData *output = vtkPolyData::SafeDownCast(
    outInfo->Get(vtkDataObject::DATA_OBJECT())
  );
  // double *sum = new double[this->Matrix->size()];
  std::size_t i = 0;
  typedef std::pair<std::size_t, double> Sum;
  Sum *sum = new Sum[this->Matrix->size()];
  for(const CorrelationMatrix::value_type &vec: *this->Matrix) {
    sum[i] = std::make_pair(i, std::accumulate(vec.cbegin(), vec.cend(), 0.0));
    // cout << sum[i].second << '\t';
    ++i;
  }
  // cout << '\n';
  std::partial_sort(sum, sum + 15, sum + this->Matrix->size(),
    [](const Sum &lhs, const Sum &rhs) { return lhs.second > rhs.second; 
  });
  // for(std::size_t i = 0; i < 15; ++i) {
  //   cout << sum[i].second << '\t';
  // }
  // cout << '\n';


  delete[] sum;
  return 1;
}