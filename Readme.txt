Step
A. Calculation
1. Read 264x264 matrix (/test/fmri_correlation.csv)
2. Sum along row
3. Select top 15 highest row id
4. Get top15 id xyz coordinate from header file (/resources/mni_power_264.h)

B. Visualization
1. Visualize brain stl
2. Visualize sphere with radius of summed value/264*C , where C is a constant, suggest C=10 (you can try tune C to have better outlook)
3. Visualize line source of top15 id with line thickness v[id1, id2]*K, where K is a constant, suggest K=5 (you can try tune K to have better outlook)

C. Export
1. Export as png in different views of angle
