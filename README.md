# Tutorial for Oklahoma State University HEP Group

This tutorial focuses on a $t \bar{t}$ analysis with resolved and boosted signal regions. With the resolved signal region, 
we will go through the main functionality of the MadAnalysis 5 workspace structure and learn how to prepare cut-flows
and histograms. With the boosted signal region, we will investigate the jet substructure interface in MadAnalysis 5. 
This will also show how to add multiple signal regions to our analysis.

Please ensure you satisfy all the requirements below before starting this tutorial.

**PS:** Please install MadAnalysis as shown below before the tutorial session and make sure that you have generated at
least 10K semileptonic, showered and hadronized $t \bar{t}$ events using the materials in `sample` folder.

## Outline

* [Structure of this repository](#structure-of-this-repository)
* [Installation](#installation)
* [First steps](#first-steps)
* [More information](#more-information)

## Structure of this repository

* `analysis` folder includes the codes for the analysis and information about how to run this tutorial.
* `sample` folder includes information for event generation to be used in this tutorial.
* `results` folder includes the results of this tutorial. 

## Installation

This tutorial is based on MadAnalysis 5 `v2.0.4_beta` or above. Please install the tarball in 
[this link](https://github.com/MadAnalysis/madanalysis5/releases/tag/v2.0.4_beta). Note that the 
substructure module is not available in any other version below. Please ensure you satisfy all the 
requirements listed in the [this README file](https://github.com/MadAnalysis/madanalysis5/tree/v2.0.4_beta#requirements).

In this tutorial, we will mainly use [FastJet](http://fastjet.fr/), 
[fj-contrib](https://fastjet.hepforge.org/contrib/) and 
[HEPTopTagger](https://github.com/MadAnalysis/HEPTopTagger) libraries, so we need to make sure that 
these tools are correctly installed. In order to ensure that, type the following steps in the command
line after installing the tarball in your machine.

```shell
wget https://github.com/MadAnalysis/madanalysis5/archive/refs/tags/v2.0.4_beta.tar.gz
tar -xvf v2.0.4_beta.tar.gz
cd madanalysis5-2.0.4_beta
./bin/ma5
```
When started, MadAnalysis will automatically compile the software in the background, given that all the requirements 
are satisfied. You will likely see the following screen;
```shell
$ ./bin/ma5
MA5:
MA5: *************************************************************
MA5: *                                                           *
MA5: *        W E L C O M E  to  M A D A N A L Y S I S  5        *
MA5: *                         ______  ______                    *
MA5: *                 /'\_/`\/\  __ \/\  ___\                   *
MA5: *                /\      \ \ \_\ \ \ \__/                   *
MA5: *                \ \ \__\ \ \  __ \ \___``\                 *
MA5: *                 \ \ \_/\ \ \ \/\ \/\ \_\ \                *
MA5: *                  \ \_\\ \_\ \_\ \_\ \____/                *
MA5: *                   \/_/ \/_/\/_/\/_/\/___/                 *
MA5: *                                                           *
MA5: *   MA5 release : 2.0.4                        2022/07/18   *
MA5: *                                                           *
MA5: *         Comput. Phys. Commun. 184 (2013) 222-256          *
MA5: *             Eur. Phys. J. C74 (2014) 3103                 *
MA5: *                                                           *
MA5: *   The MadAnalysis Development Team - Please visit us at   *
MA5: *              https://github.com/MadAnalysis               *
MA5: *                                                           *
MA5: *              Type 'help' for in-line help.                *
MA5: *                                                           *
MA5: *************************************************************
MA5: Platform: Darwin 21.5.0 [MAC/OSX mode]
MA5: Reading user settings ...
MA5: Checking mandatory packages:
MA5:      - Python                   [OK]
MA5:      - GNU GCC g++              [OK]
MA5:      - GNU Make                 [OK]
MA5: Checking optional packages devoted to data processing:
MA5:      - Zlib                     [DISABLED]
MA5:      - FastJet                  [DISABLED]
MA5:      - FastJet Contrib          [DISABLED]
MA5:      - HEPTopTagger             [DISABLED]
MA5:      - Root                     [DISABLED]
MA5:      - Delphes                  [DISABLED]
MA5:      - Delphes-MA5tune          [DISABLED]
MA5: Checking the MadAnalysis 5 core library:
MA5:   => First use of MadAnalysis (or the library is missing).
MA5:
MA5:    **********************************************************
MA5:                 Building SampleAnalyzer libraries
MA5:    **********************************************************
MA5:    How many cores for the compiling? default = max = 10
   Answer:
```
Note that some details such as platform and number of cores can be different in your machine. 
Choose a number of cores and let MadAnalysis 5 compile the software. As can be seen, the libraries 
that we are interested in are disabled.
```shell
MA5:      - Zlib                     [DISABLED]
MA5:      - FastJet                  [DISABLED]
MA5:      - FastJet Contrib          [DISABLED]
MA5:      - HEPTopTagger             [DISABLED]
```
where `zlib` will be required to untar event files with `.gz` extensions. After the compilation, we can use the 
following commands to install these packages.
```shell
install zlib
install fastjet
install HEPTopTagger
```
Note that after each command, MadAnalysis will automatically install, compile and restart the interface.
Once these tools are installed, you will see that the greeting message is modified as follows;
```shell
MA5:      - Zlib                     [OK]
MA5:      - FastJet                  [OK]
MA5:      - FastJet Contrib          [OK]
MA5:      - HEPTopTagger             [OK]
```
**Note:** If you have global installations for FastJet, MadAnalysis will automatically detect them. However,
if they are not compiled with respect to MadAnalysis' specifications, the interface will fail to compile
MadAnalysis 5. Any global third party software can be vetoed through 
[this file](https://github.com/MadAnalysis/madanalysis5/blob/v2.0.4_beta/madanalysis/input/installation_options.dat)
and the proper installation can be done through MadAnalysis.

[back to top](#outline)

## First steps

Once the above installation is complete one can create an expert mode workspace with the following command
```shell
./bin/ma5 -Re <folder name> <analysis name> <SFS card>
```
where folder name is the name of the workspace, followed by the analysis name and the 
[SFS](https://arxiv.org/abs/2006.09387) card, which includes the configuration for jet clustering and 
detector simulation. For instance, lets create a simple [SFS](https://arxiv.org/abs/2006.09387) card; open 
a file named `tuto_osu.ma5`
```shell
echo "set main.fastsim.package = fastjet" > tuto_osu.ma5
```
This will tell MadAnalysis to use FastJet interface with default configuration. So if we type
```shell
./bin/ma5 -Re tuto_osu analysis tuto_osu.ma5
```
where `R` indicates reco-mode and `e` indicates the expert mode, MadAnalysis will create a folder named 
`tuto_uso` with the following structure
```shell
tuto_osu
├── Build
│   ├── Lib
│   ├── Log
│   ├── Main
│   │   └── main.cpp                 # Includes main structure of the analysis execution
│   ├── Makefile                     # Compiles the analysis
│   ├── SampleAnalyzer
│   │   ├── User
│   │   │   └── Analyzer
│   │   │       ├── analysis.cpp     # Analysis source file to be modified
│   │   │       ├── analysis.h       # Analysis header file to be modified
│   │   │       └── analysisList.h
│   │   └── newAnalyzer.py           # Creates new analyses
│   ├── setup.csh
│   └── setup.sh                     # Setup MadAnalysis environment
├── Input                            # Location for input sample list
└── Output
    ├── DVI
    ├── HTML
    ├── Histos
    ├── PDF
    └── SAF                          # Output will be saved here

14 directories, 8 files
```
Note that folder and analysis name can be anything you want, it is not restricted.

[back to top](#outline)

## More information

For more information regarding MadAnalysis see the following links:

* [List of publications (including manual) on MadAnalysis 5](https://github.com/MadAnalysis/madanalysis5/tree/main#credits)
* Reference cards can be found in [`doc`](https://github.com/MadAnalysis/madanalysis5/tree/v2.0.4_beta/doc) folder.
* See [`doc/README.md`](https://github.com/MadAnalysis/madanalysis5/tree/v2.0.4_beta/doc#readme) 
  for details on specific modules.
* [Tutorials](https://madanalysis.irmp.ucl.ac.be/wiki/tutorials)
* [Talks](http://madanalysis.irmp.ucl.ac.be/wiki/Talks)
* Report your [issues](https://github.com/MadAnalysis/madanalysis5/issues/new/choose) or start a 
  [discussion](https://github.com/MadAnalysis/madanalysis5/discussions).
