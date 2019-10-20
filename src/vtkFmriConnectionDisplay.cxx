#include "vtkFmriConnectionDisplay.h"
// vtk
#include <vtkObjectFactory.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkLineSource.h>
#include <vtkSphereSource.h>
#include <vtkTubeFilter.h>
#include <vtkAppendPolyData.h>
#include <vtkUnsignedCharArray.h>
#include <vtkPointData.h>
// std
#include <algorithm>
#include <numeric>
#include <functional>
//
#include "mni_power_264.h"
#include "mni_power_264_color.h"
vtkStandardNewMacro(vtkFmriConnectionDisplay);

struct XY{
  int x;
  int y;
  double value;
};
typedef std::vector<XY> XYS;
XYS xys;

const int vtkFmriConnectionDisplay::DIM = 264;
vtkFmriConnectionDisplay::vtkFmriConnectionDisplay() {
  this->Matrix = nullptr;
  this->SphereRadius = 1;
  this->UseTube = false;
  this->TubeRadius = 1;
  this->Percentile = 0.1;

  this->SetNumberOfInputPorts(0);
  this->SetNumberOfOutputPorts(2);
}

vtkFmriConnectionDisplay::~vtkFmriConnectionDisplay() {

}

void vtkFmriConnectionDisplay::PrintSelf(ostream & os, vtkIndent indent) {
  this->Superclass::PrintSelf(os, indent);
  if(this->Matrix != nullptr) {
    os << indent << "Matrix dimension: " << this->Matrix->size() << 'x' << this->Matrix->front().size() << '\n';
    // for(CorrelationMatrix::value_type & vec: *this->Matrix) {
    //   os << indent.GetNextIndent();
    //   os << vec.size();
    //   for(CorrelationMatrix::value_type::value_type & v: vec) {
    //     os << v;
    //   }
    //   os << '\n';
    // }
  }
  os << indent << "SphereRadius: " << this->SphereRadius << '\n';
  os << indent << "UseTube: " << this->UseTube << '\n';
  os << indent << "TubeRadius: " << this->TubeRadius << '\n';
  os << indent << "Percentile: " << this->Percentile << '\n';
}

int vtkFmriConnectionDisplay::RequestData(
  vtkInformation *vtkNotUsed(request),
  vtkInformationVector **vtkNotUsed(inputVector),
  vtkInformationVector *outputVector) {
  
  if(!this->Matrix) {
    vtkErrorMacro("The matrix is null.");
    return 0;
  }
  vtkInformation *outInfo1 = outputVector->GetInformationObject(0);
  vtkPolyData *output1 = vtkPolyData::SafeDownCast(
    outInfo1->Get(vtkDataObject::DATA_OBJECT())
  );
  vtkInformation *outInfo2 = outputVector->GetInformationObject(1);
  vtkPolyData *output2 = vtkPolyData::SafeDownCast(
    outInfo2->Get(vtkDataObject::DATA_OBJECT())
  );
  // double *sum = new double[this->Matrix->size()];
  // std::size_t i = 0;
  // typedef std::pair<std::size_t, double> Sum;
  // Sum *sum = new Sum[this->Matrix->size()];
  // for(const CorrelationMatrix::value_type &vec: *this->Matrix) {
  //   sum[i] = std::make_pair(i, std::accumulate(vec.cbegin(), vec.cend(), 0.0));
  //   // cout << sum[i].second << '\t';
  //   ++i;
  // }
  // // cout << '\n';
  // std::partial_sort(sum, sum + TOP, sum + this->Matrix->size(),
  //   [](const Sum &lhs, const Sum &rhs) { return lhs.second > rhs.second; 
  // });
  vtkSmartPointer<vtkAppendPolyData> append1 = 
    vtkSmartPointer<vtkAppendPolyData>::New();
  std::vector<vtkSmartPointer<vtkSphereSource>> spheres;
  spheres.resize(DIM);
  for(std::size_t i = 0; i < DIM; ++i) {
    const unsigned char * const color = mni_power_264_color[i];
    const int * const center = mni_power_264[i];
    // const double radius = sum[i].second / 264 * C;
    spheres[i] = vtkSmartPointer<vtkSphereSource>::New();
    spheres[i]->SetCenter(center[1], center[2], center[3]);
    spheres[i]->SetRadius(this->SphereRadius);
    spheres[i]->Update();
    vtkSmartPointer<vtkUnsignedCharArray> colorArray =
      vtkSmartPointer<vtkUnsignedCharArray>::New();
    colorArray->SetName("Color");
    colorArray->SetNumberOfComponents(3);
    colorArray->SetNumberOfTuples(spheres[i]->GetOutput()->GetNumberOfPoints());
    unsigned char *pointer = colorArray->WritePointer(0, 3 * spheres[i]->GetOutput()->GetNumberOfPoints());
    for(std::size_t j = 0; j < colorArray->GetNumberOfTuples(); ++j) {
      memcpy(pointer + 3 * j, color, 3);
    }
    spheres[i]->GetOutput()->GetPointData()->SetScalars(colorArray);
    append1->AddInputData(spheres[i]->GetOutput());
  }
  append1->Update();
  output1->ShallowCopy(append1->GetOutput());
  output1->GetPointData()->SetActiveScalars("Color");

  for(std::size_t i = 0; i < DIM - 1; i++) {
    for(std::size_t j = i + 1; j < DIM; j++) {
      xys.push_back({static_cast<int>(i), static_cast<int>(j), (*this->Matrix)[i][j]});
    }
  }
  std::size_t topNPercent = static_cast<std::size_t>(ceil(xys.size() * this->Percentile));
  std::partial_sort(xys.begin(), xys.begin() + topNPercent, xys.end(),
    [](const XY &first, const XY &second) { return first.value > second.value; });

  vtkSmartPointer<vtkAppendPolyData> append2 =
    vtkSmartPointer<vtkAppendPolyData>::New();
  std::vector<vtkSmartPointer<vtkLineSource>> lines;
  std::vector<vtkSmartPointer<vtkTubeFilter>> tubes;
  XYS::const_iterator end = xys.cbegin() + topNPercent;
  for(XYS::const_iterator cit = xys.cbegin(); cit != end; ++cit) {
    const int *const point1 = mni_power_264[cit->x];
    const int *const point2 = mni_power_264[cit->y];
    lines.push_back(vtkSmartPointer<vtkLineSource>::New());
    lines.back()->SetPoint1(point1[1], point1[2], point1[3]);
    lines.back()->SetPoint2(point2[1], point2[2], point2[3]);
    if(this->UseTube) {
      tubes.push_back(vtkSmartPointer<vtkTubeFilter>::New());
      tubes.back()->SetInputConnection(lines.back()->GetOutputPort());
      tubes.back()->SetRadius(this->TubeRadius);
      append2->AddInputConnection(tubes.back()->GetOutputPort());
    }
    else {
      append2->AddInputConnection(lines.back()->GetOutputPort());
    }
  }
  // for(std::size_t i = 0; i < TOP; ++i) {
  //   const int * const center = mni_power_264[sum[i].first];
  //   const double radius = sum[i].second / 264 * C;
  //   spheres.push_back(vtkSmartPointer<vtkSphereSource>::New());
  //   spheres.back()->SetCenter(center[1], center[2], center[3]);
  //   spheres.back()->SetRadius(radius);
  //   append1->AddInputConnection(spheres.back()->GetOutputPort());
  //   for (std::size_t j = 0; j < TOP; ++j) {
  //     const int * const point2  = mni_power_264[sum[j].first];
  //     lines.push_back(vtkSmartPointer<vtkLineSource>::New());
  //     lines.back()->SetPoint1(center[1], center[2], center[3]);
  //     lines.back()->SetPoint2(point2[1], point2[2], point2[3]);
  //     tubes.push_back(vtkSmartPointer<vtkTubeFilter>::New());
  //     tubes.back()->SetInputConnection(lines.back()->GetOutputPort());
  //     tubes.back()->SetRadius((*this->Matrix)[i][j] * K);
  //     append2->AddInputConnection(tubes.back()->GetOutputPort());
  //   }
  // }
  append2->Update();
  output2->ShallowCopy(append2->GetOutput());
  // for(std::size_t i = 0; i < 15; ++i) {
  //   cout << sum[i].second << '\t';
  // }
  // cout << '\n';


  // delete[] sum;
  return 1;
}