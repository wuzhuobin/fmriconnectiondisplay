# Step
A. Calculation

  1. Read 264x264 matrix (/test/fmri_correlation.csv)
  2. Reserve top10% highest value of upper-right half，只保留X部分的最大10%

  |1xxxx|
  | 1xxx|
  |  1xx|
  |   1x|
  |    1|

B. Visualization

  1. Visualize brain stl (split left and right brain)
  2. Visualize all spheres with same size and different colors(color will be provided in later stage)
  3. Visualize top10% line source with same line thickness

C. Export

  1. Export as png in different views of angle
		- fmri-connection-anterior.png
		- fmri-connection-posterior.png
		- fmri-connection-inferior.png
		- fmri-connection-superior.png
		- fmri-connection-left-LB.png 			#left view with left brain visible
		- fmri-connection-left-RB.png			#left view with right brain visible
		- fmri-connection-right-LB.png			#right view with left brain visible
		- fmri-connection-right-RB.png			#right view with right brain visible

D. IO

fmriConnection --csv ${CSV_PATH} --output {OUTPUT_DIR}