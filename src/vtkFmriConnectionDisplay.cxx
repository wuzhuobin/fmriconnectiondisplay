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

// const int vtkFmriConnectionDisplay::DIM = 264;
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
  os << indent << "Number of lines: " << this->Lines.size() << '\n';
  os << indent << "Number of tubes: " << this->Tubes.size() << '\n';
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

  const std::size_t DIM = this->Matrix->size();
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

  XYS xys;
  for(std::size_t i = 0; i < DIM - 1; i++) {
    for(std::size_t j = i + 1; j < DIM; j++) {
      xys.push_back({static_cast<int>(i), static_cast<int>(j), (*this->Matrix)[i][j]});
    }
  }
  // std::cout << xys.size() << '\n';
  // std::cout << this->Percentile << '\n';
  std::size_t topNPercent = static_cast<std::size_t>(ceil(xys.size() * this->Percentile));
  std::partial_sort(xys.begin(), xys.begin() + topNPercent, xys.end(),
    [](const XY &first, const XY &second) { return first.value > second.value; });
  // std::for_each(xys.begin(), xys.begin() + topNPercent, [](const XY& xy){ std::cout << xy.value << '\n';});

  vtkSmartPointer<vtkAppendPolyData> append2 =
    vtkSmartPointer<vtkAppendPolyData>::New();
  XYS::const_iterator end = xys.cbegin() + topNPercent;
  for(XYS::const_iterator cit = xys.cbegin(); cit != end; ++cit) {
    const int *const point1 = mni_power_264[cit->x];
    const int *const point2 = mni_power_264[cit->y];
    this->Lines.push_back(vtkLineSource::New());
    this->Lines.back()->Register(this);
    this->Lines.back()->SetPoint1(point1[1], point1[2], point1[3]);
    this->Lines.back()->SetPoint2(point2[1], point2[2], point2[3]);
    if(this->UseTube) {
      this->Tubes.push_back(vtkTubeFilter::New());
      this->Tubes.back()->Register(this);
      this->Tubes.back()->SetInputConnection(Lines.back()->GetOutputPort());
      this->Tubes.back()->SetRadius(this->TubeRadius);
      append2->AddInputConnection(this->Tubes.back()->GetOutputPort());
    }
    else {
      append2->AddInputConnection(this->Lines.back()->GetOutputPort());
    }
  }
  append2->Update();
  output2->ShallowCopy(append2->GetOutput());
  return 1;
}