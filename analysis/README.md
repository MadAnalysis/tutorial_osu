# TTbar Analysis

This folder includes details about how to run the analysis. For simplicity, we will only use the header file, but you 
will get the same results if you use both the header and source file together. `analysis.h` includes the
cut-flow and histogram definitions that we will use in this tutorial. `tuto_osu_truth.ma5` is the truth level
[SFS](https://arxiv.org/abs/2006.09387) card and `tuto_osu_detector.ma5` is the [SFS](https://arxiv.org/abs/2006.09387)
card, which includes smearing and tagging functionality for the detector simulation.

In order to run truth level analysis please execute following commands
```shell
cd /PATH/TO/madanalysis5-2.0.4_beta
./bin/ma5 -Re tuto_osu_truth analysis /PATH/TO/tutorial_osu/analysis/tuto_osu_truth.ma5
cp /PATH/TO/tutorial_osu/analysis/analysis.h tuto_osu_truth/Build/SampleAnalyzer/User/Analyzer/.
rm tuto_osu_truth/Build/SampleAnalyzer/User/Analyzer/analysis.cpp
cd tuto_osu_truth/Build
source setup.sh --with-fastjet
make all
```
now we need to add the sample list to the `Input` folder:
```shell
echo "/PATH/TO/SAMPLE/my_sample.hepmc.gz" > ../Input/sample.list
```
and execute the analysis:
```shell
./MadAnalysis5job ../Input/sample.list
```
Outputs of the analysis will be saved in `tutorial_osu/Output/SAF/sample` folder.

The steps to run detector level simulation is exactly the same, but the only difference is instead of using 
`tuto_osu_truth` we need to use `tuto_osu_detector` i.e.
```shell
./bin/ma5 -Re tuto_osu_detector analysis /PATH/TO/tutorial_osu/analysis/tuto_osu_detector.ma5
```
This will produce following file structure
```shell
tuto_osu_detector
├── Build
│   ├── Lib
│   ├── Log
│   ├── Main
│   │   └── main.cpp
│   ├── Makefile
│   ├── SampleAnalyzer
│   │   ├── User
│   │   │   └── Analyzer
│   │   │       ├── analysis.cpp
│   │   │       ├── analysis.h
│   │   │       ├── analysisList.h
│   │   │       ├── efficiencies.h             # Auto-generated files for detector simulation
│   │   │       ├── new_smearer_reco.cpp       # Auto-generated files for detector simulation
│   │   │       ├── new_smearer_reco.h         # Auto-generated files for detector simulation
│   │   │       ├── new_tagger.cpp             # Auto-generated files for detector simulation
│   │   │       ├── new_tagger.h               # Auto-generated files for detector simulation
│   │   │       ├── reco.h                     # Auto-generated files for detector simulation
│   │   │       └── sigmas.h                   # Auto-generated files for detector simulation
│   │   └── newAnalyzer.py
│   ├── setup.csh
│   └── setup.sh
├── Input
└── Output
    ├── DVI
    ├── HTML
    ├── Histos
    ├── PDF
    └── SAF

14 directories, 15 files
```
Again the only files that we need to change to implement the analysis is `analysis.h` and `analysis.cpp`.