#ifndef __VTK_FMRI_CONNECTION_DISPLAY_H__
#define __VTK_FMRI_CONNECTION_DISPLAY_H__
#pragma once
#include <vtkPolyDataAlgorithm.h>
class vtkLineSource;
class vtkTubeFilter;

class vtkFmriConnectionDisplay : public vtkPolyDataAlgorithm
{
public:
  static vtkFmriConnectionDisplay *New();
  vtkTypeMacro(vtkFmriConnectionDisplay, vtkPolyDataAlgorithm);
  virtual void PrintSelf(ostream &os, vtkIndent indent);

  typedef std::vector<std::vector<double>> CorrelationMatrix;
  vtkSetMacro(Matrix, CorrelationMatrix*);
  vtkGetMacro(Matrix, CorrelationMatrix*);

  vtkSetMacro(SphereRadius, double);
  vtkGetMacro(SphereRadius, double);


  vtkBooleanMacro(UseTube, bool);
  vtkSetMacro(UseTube, bool);
  vtkGetMacro(UseTube, bool);


  vtkSetMacro(TubeRadius, double);
  vtkGetMacro(TubeRadius, double);

  vtkSetClampMacro(Percentile, double, 0, 1);
  vtkGetMacro(Percentile, double);

protected:
  vtkFmriConnectionDisplay();
  virtual ~vtkFmriConnectionDisplay() override;

  virtual int RequestData(vtkInformation* request,
                          vtkInformationVector** inputVector,
                          vtkInformationVector* outputVector) override;

  CorrelationMatrix *Matrix;
  double SphereRadius;
  bool UseTube;
  double TubeRadius;
  double Percentile;
  std::vector<vtkLineSource*> Lines;
  std::vector<vtkTubeFilter*> Tubes;
private:
  vtkFmriConnectionDisplay(const vtkFmriConnectionDisplay&) = delete;
  void operator=(const vtkFmriConnectionDisplay&) = delete;
  // static const int DIM;
};

#endif //!__VTK_FMRI_CONNECTION_DISPLAY_H__