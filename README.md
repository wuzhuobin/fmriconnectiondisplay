# FMRI Connection Display
## Usage
The following information is print out of the program's help.
```
This program is to find the top percentile correlation connection in fmri.
Rendering 264 brain regions in spheres and the top percentile in lines or tubes. 

Examples:
./fmriconnectiondisplay -c "YourCSVFile.csv"
Description:
  -h [ --help ]                         Show help.
  -c [ --csv ] arg                      Set csv input path.
  -o [ --output ] arg                   Set output path.
  --sphere-radius arg (=1)              Set the radius(mm) of spheres.
  -t [ --tube ] [=arg(=1)]              Using tube instead of line to render 
                                        the connections.
  --tube-radius arg (=1)                Set the radius of the tubes, please go 
                                        with "-t". 
  -P [ --percentile ] arg (=0.10000000000000001)
                                        Set the top percentile, between 0 to 1.
  --opacity arg (=0.5)                  Set opacity(0-1) of the brain.
  --axex [=arg(=1)]                     Show the reference axes.
  --debug [=arg(=1)]                    Debug in interative way.
  -a [ --anterior ] arg (=fmri-connection-anterior)
                                        Set the anterior output path without 
                                        extension name(since only png is 
                                        supported).
  -p [ --posterior ] arg (=fmri-connection-posterior)
                                        Set the posterior output path without 
                                        extension name(since only png is 
                                        supported).
  -i [ --inferior ] arg (=fmri-connection-inferior)
                                        Set the inferior ouptut path without 
                                        extension name(since only png is 
                                        supported).
  -s [ --superior ] arg (=fmri-connection-superior)
                                        Set the superior ouptut path without 
                                        extension name(since only png is 
                                        supported).
  -L [ --left-left-brain ] arg (=fmri-connection-left-LB)
                                        Set the left view with left brain 
                                        visible ouptut path without extension 
                                        name(since only png is supported).
  -l [ --left-right-brain ] arg (=fmri-connection-left-RB)
                                        Set the left view with right brain 
                                        visible ouptut path without extension 
                                        name(since only png is supported).
  -r [ --right-left-brain ] arg (=fmri-connection-right-LB)
                                        Set the right view with left brain 
                                        visible ouptut path without extension 
                                        name(since only png is supported).
  -R [ --right-right-brain ] arg (=fmri-connection-right-RB)
                                        Set the right view with right brain 
                                        visible ouptut path without extension 
                                        name(since only png is supported).
```