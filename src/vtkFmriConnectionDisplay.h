#ifndef __VTK_FMRI_CONNECTION_DISPLAY_H__
#define __VTK_FMRI_CONNECTION_DISPLAY_H__
#pragma once
#include <vtkPolyDataAlgorithm.h>

class vtkFmriConnectionDisplay : public vtkPolyDataAlgorithm
{
public:
  static vtkFmriConnectionDisplay *New();
  vtkTypeMacro(vtkFmriConnectionDisplay, vtkPolyDataAlgorithm);
  virtual void PrintSelf(ostream &os, vtkIndent indent);

  typedef std::vector<std::vector<double>> CorrelationMatrix;
  vtkSetMacro(Matrix, CorrelationMatrix*);
  vtkGetMacro(Matrix, CorrelationMatrix*);

protected:
  vtkFmriConnectionDisplay();
  virtual ~vtkFmriConnectionDisplay() override;

  virtual int RequestData(vtkInformation* request,
                          vtkInformationVector** inputVector,
                          vtkInformationVector* outputVector) override;

  CorrelationMatrix *Matrix;
private:
  vtkFmriConnectionDisplay(const vtkFmriConnectionDisplay&) = delete;
  void operator=(const vtkFmriConnectionDisplay&) = delete;
  static const int TOP;
  static const double C;
  static const double K;
};

#endif //!__VTK_FMRI_CONNECTION_DISPLAY_H__