# Analysis results
The analysis results have been prepared via the [`ma5-expert`](https://github.com/MadAnalysis/ma5_expert) plugin 
(follow the instructions in the readme file to install and run). `truth` folder contains the MadAnalysis output
files within `tuto_osu_truth/Output/SAF/sample/analysis_0` and PDF files prepared with `ma5-expert` plugin. 
Similarly `detector` folder contains results from `tuto_osu_detector/Output/SAF/sample/analysis_0` folder. 

**Note:** Your detector simulation results may vary; inorder to fix the random seed, you can use the following command
`set main.random_seed = <value>` in SFS card.

`histXX.pdf` files in this folder are prepared by combining truth and detector level histograms from this analysis.

The results are prepared with 1.6 pb cross-section and 10 fb$^{-1}$ integrated luminosity.