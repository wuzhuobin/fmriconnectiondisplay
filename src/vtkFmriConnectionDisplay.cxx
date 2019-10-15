#include "vtkFmriConnectionDisplay.h"
// vtk
#include <vtkObjectFactory.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkLineSource.h>
#include <vtkSphereSource.h>
#include <vtkTubeFilter.h>
#include <vtkAppendPolyData.h>
// std
#include <algorithm>
#include <numeric>
#include <functional>
//
#include "mni_power_264.h"
vtkStandardNewMacro(vtkFmriConnectionDisplay);

const int vtkFmriConnectionDisplay::TOP = 15;
const double vtkFmriConnectionDisplay::C = 7;
const double vtkFmriConnectionDisplay::K = 1;
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
  std::partial_sort(sum, sum + TOP, sum + this->Matrix->size(),
    [](const Sum &lhs, const Sum &rhs) { return lhs.second > rhs.second; 
  });
  std::vector<vtkSmartPointer<vtkSphereSource>> spheres;
  vtkSmartPointer<vtkAppendPolyData> append = 
    vtkSmartPointer<vtkAppendPolyData>::New();
  std::vector<vtkSmartPointer<vtkLineSource>> lines;
  std::vector<vtkSmartPointer<vtkTubeFilter>> tubes;
  for(std::size_t i = 0; i < TOP; ++i) {
    const int * const center = mni_power_264[sum[i].first];
    const double radius = sum[i].second / 264 * C;
    spheres.push_back(vtkSmartPointer<vtkSphereSource>::New());
    spheres.back()->SetCenter(center[1], center[2], center[3]);
    spheres.back()->SetRadius(radius);
    append->AddInputConnection(spheres.back()->GetOutputPort());
    for (std::size_t j = 0; j < TOP; ++j) {
      const int * const point2  = mni_power_264[sum[j].first];
      lines.push_back(vtkSmartPointer<vtkLineSource>::New());
      lines.back()->SetPoint1(center[1], center[2], center[3]);
      lines.back()->SetPoint2(point2[1], point2[2], point2[3]);
      tubes.push_back(vtkSmartPointer<vtkTubeFilter>::New());
      tubes.back()->SetInputConnection(lines.back()->GetOutputPort());
      tubes.back()->SetRadius((*this->Matrix)[i][j] * K);
      append->AddInputConnection(tubes.back()->GetOutputPort());
    }
  }
  append->Update();
  output->ShallowCopy(append->GetOutput());
  // for(std::size_t i = 0; i < 15; ++i) {
  //   cout << sum[i].second << '\t';
  // }
  // cout << '\n';


  delete[] sum;
  return 1;
}