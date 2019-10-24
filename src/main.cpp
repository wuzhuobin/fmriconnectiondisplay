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
// #include <vtkPolyDataWriter.h>
// boost
#include <boost/tokenizer.hpp>
#include <boost/program_options.hpp>
// std
#include <iostream>
//
#include "vtkFmriConnectionDisplay.h"
int execute(const boost::program_options::variables_map &map);
int main (int argc, char *argv[]) {
  int ret = 0;
  namespace bpo = boost::program_options;
  bpo::options_description desc(
    "This program is to find the top percentile correlation connection in fmri.\n"
    "Rendering 264 brain regions in spheres and the top percentile in lines or tubes. \n\n"
    "Examples:\n" +
    std::string(argv[0]) + " -c \"YourCSVFile.csv\"\n"
    "Description"
  );
  desc.add_options()
    ("help,h", "Show help.")
    ("csv,c", bpo::value<std::string>()->required(), "Set csv input path.")
    ("output,o", bpo::value<std::string>()->default_value(""), "Set output path.")
    ("sphere-radius", bpo::value<double>()->default_value(1.0), "Set the radius(mm) of spheres.")
    ("tube,t", bpo::value<bool>()->implicit_value(true), "Using tube instead of line to render the connections.")
    ("tube-radius", bpo::value<double>()->default_value(1.0), "Set the radius of the tubes, please go with \"-t\". ")
    ("percentile,P", bpo::value<double>()->default_value(0.1), "Set the top percentile, between 0 to 1.")
    ("opacity", bpo::value<double>()->default_value(0.5), "Set opacity(0-1) of the brain.")
    ("axex", bpo::value<bool>()->implicit_value(true), "Show the reference axes.")
    ("debug", bpo::value<bool>()->implicit_value(true), "Debug in interative way.")
    ("anterior,a", bpo::value<std::string>()->default_value("fmri-connection-anterior"), "Set the anterior output path without extension name(since only png is supported).")
    ("posterior,p", bpo::value<std::string>()->default_value("fmri-connection-posterior"), "Set the posterior output path without extension name(since only png is supported).")
    ("inferior,i", bpo::value<std::string>()->default_value("fmri-connection-inferior"), "Set the inferior ouptut path without extension name(since only png is supported).")
    ("superior,s", bpo::value<std::string>()->default_value("fmri-connection-superior"), "Set the superior ouptut path without extension name(since only png is supported).")
    ("left-left-brain,L", bpo::value<std::string>()->default_value("fmri-connection-left-LB"),  "Set the left view with left brain visible ouptut path without extension name(since only png is supported).")
    ("left-right-brain,l", bpo::value<std::string>()->default_value("fmri-connection-left-RB"), "Set the left view with right brain visible ouptut path without extension name(since only png is supported).")
    ("right-left-brain,r", bpo::value<std::string>()->default_value("fmri-connection-right-LB"), "Set the right view with left brain visible ouptut path without extension name(since only png is supported).")
    ("right-right-brain,R", bpo::value<std::string>()->default_value("fmri-connection-right-RB"), "Set the right view with right brain visible ouptut path without extension name(since only png is supported).");
  try {
    bpo::variables_map variables_map;
    bpo::command_line_parser command_line_parser(argc, argv);
    command_line_parser.options(desc).style(
      bpo::command_line_style::allow_dash_for_short |
      bpo::command_line_style::default_style
    );
    bpo::parsed_options parserd_options = command_line_parser.run();
    bpo::store(parserd_options, variables_map);
    bpo::notify(variables_map);
    ret = execute(variables_map);
  }
  catch (const bpo::error &error) {
    std::cerr << "Error: " << error.what() << "\n\n\n";
    std::cout << desc << '\n';
    ret = 1;
  }
  return ret;
}

typedef boost::tokenizer<boost::escaped_list_separator<char>> tokenizer;
typedef std::vector<std::vector<double>> matrix_d;
int execute(const boost::program_options::variables_map &map)
{
  // std::string fileName = "D:/ccode/fmriconnectiondisplay/test/fmri_correlation_1.csv";
  std::string filename = map["csv"].as<std::string>();
  std::string output = map["output"].as<std::string>();
  double sphere_radius = map["sphere-radius"].as<double>();
  bool tube = false;
  if(map.count("tube")) {
    tube = map["tube"].as<bool>();
  }
  double tube_radius = map["tube-radius"].as<double>();
  double percentile = map["percentile"].as<double>();
  double opacity = map["opacity"].as<double>();
  bool axes = false;
  if(map.count("axes")) {
    axes = map["axes"].as<bool>();
  }
  bool debug = false;
  if(map.count("debug")) {
    debug = map["debug"].as<bool>();
  }
  std::string anterior = output + map["anterior"].as<std::string>() + ".png";
  std::string posterior = output + map["posterior"].as<std::string>() + ".png";
  std::string inferior = output + map["inferior"].as<std::string>() + ".png";
  std::string superior = output + map["superior"].as<std::string>() + ".png";
  std::string left_left_brain = output + map["left-left-brain"].as<std::string>() + ".png";
  std::string left_right_brain = output + map["left-right-brain"].as<std::string>() + ".png";
  std::string right_left_brain = output + map["right-left-brain"].as<std::string>() + ".png";
  std::string right_right_brain = output + map["right-right-brain"].as<std::string>() + ".png";

  std::ifstream fin;
  fin.open(filename, std::ios::in);
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
  fmri_filter->SetSphereRadius(sphere_radius);
  fmri_filter->SetUseTube(tube);
  fmri_filter->SetTubeRadius(tube_radius);
  fmri_filter->SetPercentile(percentile);
  fmri_filter->Update();
  std::cout << *fmri_filter << '\n';
  // std::cin.get();

  vtkSmartPointer<vtkPolyDataMapper> sphereMapper =
      vtkSmartPointer<vtkPolyDataMapper>::New();
  sphereMapper->SetInputConnection(fmri_filter->GetOutputPort(0));

  // vtkSmartPointer<vtkPolyDataWriter> polydataWriter =
  //   vtkSmartPointer<vtkPolyDataWriter>::New();
  // polydataWriter->SetInputConnection(fmri_filter->GetOutputPort());
  // polydataWriter->SetFileName("sphere.vtk");
  // polydataWriter->Write();

  vtkSmartPointer<vtkActor> sphereActor =
      vtkSmartPointer<vtkActor>::New();
  sphereActor->SetMapper(sphereMapper);
  // sphereActor->GetProperty()->SetColor(1, 0, 0);

  vtkSmartPointer<vtkPolyDataMapper> tubeMapper =
      vtkSmartPointer<vtkPolyDataMapper>::New();
  tubeMapper->SetInputConnection(fmri_filter->GetOutputPort(1));

  vtkSmartPointer<vtkActor> tubeActor =
      vtkSmartPointer<vtkActor>::New();
  tubeActor->SetMapper(tubeMapper);
  // tubeActor->GetProperty()->SetColor(0.3, 0.3, 0.3);

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
  leftActor->GetProperty()->SetOpacity(opacity);
  leftActor->GetProperty()->SetColor(0.5, 0.5, 0.5);

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
  rightActor->GetProperty()->SetOpacity(opacity);
  rightActor->GetProperty()->SetColor(0.5, 0.5, 0.5);

  vtkSmartPointer<vtkAxesActor> axesActor =
    vtkSmartPointer<vtkAxesActor>::New();
  axesActor->SetTotalLength(100, 100, 100);
  axesActor->SetVisibility(axes);

  vtkSmartPointer<vtkRenderer> renderer =
      vtkSmartPointer<vtkRenderer>::New();
  renderer->AddActor(sphereActor);
  renderer->AddActor(tubeActor);
  renderer->AddActor(leftActor);
  renderer->AddActor(rightActor);
  renderer->AddActor(axesActor);
  renderer->UseDepthPeelingOn();
  renderer->SetOcclusionRatio(0);
  renderer->SetMaximumNumberOfPeels(0);

  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->SetOffScreenRendering(!debug);
  renderWindow->AddRenderer(renderer);
  renderWindow->SetSize(1024, 1024);
  renderWindow->SetMultiSamples(0);
  renderWindow->AlphaBitPlanesOn();
  renderWindow->Render();

  vtkSmartPointer<vtkWindowToImageFilter> windowToImageFIlter =
    vtkSmartPointer<vtkWindowToImageFilter>::New();
  windowToImageFIlter->SetInput(renderWindow);

  vtkSmartPointer<vtkPNGWriter> pngWriter =
    vtkSmartPointer<vtkPNGWriter>::New();
  pngWriter->SetInputConnection(windowToImageFIlter->GetOutputPort());

  renderer->GetActiveCamera()->SetPosition(0, 1, 0);
  renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
  renderer->GetActiveCamera()->SetViewUp(0, 0, 1);
  renderer->ResetCamera();
  renderer->ResetCameraClippingRange();
  renderWindow->Render();

  windowToImageFIlter->Modified();
  pngWriter->SetFileName(anterior.c_str());
  pngWriter->Write();


  renderer->GetActiveCamera()->SetPosition(0, -1, 0);
  renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
  renderer->GetActiveCamera()->SetViewUp(0, 0, 1);
  renderer->ResetCamera();
  renderer->ResetCameraClippingRange();
  renderWindow->Render();

  windowToImageFIlter->Modified();
  pngWriter->SetFileName(posterior.c_str());
  pngWriter->Write();


  renderer->GetActiveCamera()->SetPosition(0, 0, -1);
  renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
  renderer->GetActiveCamera()->SetViewUp(0, 1, 0);
  renderer->ResetCamera();
  renderer->ResetCameraClippingRange();
  renderWindow->Render();

  windowToImageFIlter->Modified();
  pngWriter->SetFileName(inferior.c_str());
  pngWriter->Write();


  renderer->GetActiveCamera()->SetPosition(0, 0, 1);
  renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
  renderer->GetActiveCamera()->SetViewUp(0, 1, 0);
  renderer->ResetCamera();
  renderer->ResetCameraClippingRange();
  renderWindow->Render();

  windowToImageFIlter->Modified();
  pngWriter->SetFileName(superior.c_str());
  pngWriter->Write();

  rightActor->SetVisibility(false);
  leftActor->SetVisibility(true);
  renderer->GetActiveCamera()->SetPosition(-1, 0, 0);
  renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
  renderer->GetActiveCamera()->SetViewUp(0, 0, 1);
  renderer->ResetCamera();
  renderer->ResetCameraClippingRange();
  renderWindow->Render();

  windowToImageFIlter->Modified();
  pngWriter->SetFileName(left_left_brain.c_str());
  pngWriter->Write();


  rightActor->SetVisibility(true);
  leftActor->SetVisibility(false);
  renderer->GetActiveCamera()->SetPosition(-1, 0, 0);
  renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
  renderer->GetActiveCamera()->SetViewUp(0, 0, 1);
  renderer->ResetCamera();
  renderer->ResetCameraClippingRange();
  renderWindow->Render();

  windowToImageFIlter->Modified();
  pngWriter->SetFileName(left_right_brain.c_str());
  pngWriter->Write();



  rightActor->SetVisibility(false);
  leftActor->SetVisibility(true);
  renderer->GetActiveCamera()->SetPosition(1, 0, 0);
  renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
  renderer->GetActiveCamera()->SetViewUp(0, 0, 1);
  renderer->ResetCamera();
  renderer->ResetCameraClippingRange();
  renderWindow->Render();

  windowToImageFIlter->Modified();
  pngWriter->SetFileName(right_left_brain.c_str());
  pngWriter->Write();



  rightActor->SetVisibility(true);
  leftActor->SetVisibility(false);
  renderer->GetActiveCamera()->SetPosition(1, 0, 0);
  renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
  renderer->GetActiveCamera()->SetViewUp(0, 0, 1);
  renderer->ResetCamera();
  renderer->ResetCameraClippingRange();
  renderWindow->Render();

  windowToImageFIlter->Modified();
  pngWriter->SetFileName(right_right_brain.c_str());
  pngWriter->Write();


  if(debug) {
    rightActor->SetVisibility(true);
    leftActor->SetVisibility(true);
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
        vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);

    renderWindowInteractor->Start();
  }

  return 0;
}
