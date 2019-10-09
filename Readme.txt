Step
A. Calculation
1. Read 264x264 matrix (/test/fmri_correlation.csv)
2. Sum along row
3. Select top 15 highest row id
4. Get top15 id xyz coordinate from header file (/resources/mni_power_264.h)

B. Visualization
1. Visualize brain stl (split left and right brain)
2. Visualize sphere with radius of summed value/264*C , where C is a constant, suggest C=5 (you can try tune C to have better outlook)
3. Visualize line source of top15 id with line thickness v[id1, id2]*K, where K is a constant, suggest K=3 (you can try tune K to have better outlook)

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