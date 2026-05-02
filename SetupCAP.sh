#!/bin/bash

####################################################################################################
#
# Source this script to define CAP environment variables used in the build and running of CAP6.0 with root.
# This script assumes the following environment variables are already defined
#
# The build and run CAP, you need to have the following tools
#
#   cmake    :  (cmake3) available (free) from: https://cmake.org/download/
#   make     :  indigenous to all LINUX, UNIX, and MacOS platforms
#   root     :  available from the CERN root distribution or from various sources
#   PYTHIA8  :  available from https://pythia.org
#   FASTJET  :  available from https://fastjet.fr
#
# To build and run the code, you will need a version root that works on your machine. Given root uses
# dynamic load libraries, it is sufficient (usually) to get a recent binary of root for your
# platform. Alternatively, you can also use a version built from the source. Note: EG libraries
# are no longer required with CAP.
#
# The cmake code assumes root is located at $ROOTSYS and
# that the $LD_LIBRARY_PATH, $SHLIB_PATH, $PYTHONPATH,
#
# To build the code after download
#    cd  CAP             : i.e., whereever you have located your download version of CAP
#    rm  -r -f build     : to start with a clean slate
#    source SetupCAP.sh  : source this file to configure the environment variables.
#    cd build            : swith to the build dir
#    cmake ../src        : to configure the build with cmake
#    make -j# install    : to build the code. # is a number corresponding to the number of
#                        : of CPU cores dedicated to the build -- more cores means a faster build
#
# To run the code after building, you need to have the following two environment variables defined in
# your shell profile/
#    HistosInputPath     : Desired location of input histogram files, calibration files, etc
#    HistosOutputPath    : Desired location of output histogram files, calibration files, etc
#
#    Example: In your .zshrc (MAC) or .zprofile scrit, insert
#             export HistosInputPath="/Volume/PathForHistosInputPath"
#             export HistosOutputPath="/Volume/PathForHistosOutputPath"
#    Note: It is preferable to define HistosInputPath and HistosOutputPath at the shell level
#          rather than in this script - since one should not have to edit this script to run CAP...
#
# See the src/Macros subdirectory for a list of instructions and commands to run high level
# analyses.
#
#
####################################################################################################
echo " "
echo " "
echo "========================================================================================"
echo " SetupCAP starting!"
echo "========================================================================================"
echo " "
echo "========================================================================================"
echo " CAP DEPENDENCIES"
echo "========================================================================================"
echo " The current version of CAP requires the prior installation of PYTHIA and FASTJET "
echo " The location of the include files and lib files are flexible but must be passed to "
echo " cmake  in order to complete the build of CAP. The locations are passed with the following"
echo " environment variables - which should be defined in your shell profile."
echo " "
echo "========================================================================================"
echo " BUILD"
echo "========================================================================================"
echo " To build CAP, execute the following commands"
echo " "
echo " cd build"
echo " cmake ../src"
echo " make -j8 install"
echo " "
echo " CAP includes several modules. Make sure you read the output of cmake and make to check for"
echo " for errors. Usually, errors indicate an incomplete or corrupted version of the code or "
echo " of the absence of external dependencies - PYTHIA, FASTJET, etc"
echo " "
echo "========================================================================================"
echo " JOB CONFIGURATION"
echo "========================================================================================"
echo " "
echo " CAP jobs are controlled by an initialization file (Name format: XXXX.ini). This "
echo " initialization file controls the workflow and parameters of each of the "
echo " tasks and subtasks of a particular analysis. "
echo " Initialization files are nominally stored and organized in \$CAP_PROJECTS_PATH"
echo " but you can change this behavior by changing the nominal definition of this variable"
echo " "
echo "========================================================================================"
echo " RUNNING"
echo "========================================================================================"
echo " "
echo " As of CAP8.0, the building of CAP produces an executable named CAP by default."
echo " To run CAP, call this program with the top level taks you wish to execute and "
echo " the ini file containing the specifications of the project. "
echo " Example:"
echo " ./CAP RunAna Glauber/glauber.ini"
echo " "
echo " You can of course customize RunAna  or create new versions suiting your needs."
echo " "
echo "========================================================================================"
echo " HELP"
echo "========================================================================================"
echo " "
echo " Contact the authors of CAP if you have questions..."
echo " "
echo " "
echo " "
export CAP_ROOT_PATH=`pwd`
export CAP_SRC_PATH="$CAP_ROOT_PATH/src/"
export CAP_BIN_PATH="$CAP_ROOT_PATH/bin/"
export CAP_LIB_PATH="$CAP_ROOT_PATH/lib/"
export CAP_GRID_PATH="$CAP_ROOT_PATH/Grid/"
export CAP_GRID_PATH_WSU="$CAP_ROOT_PATH/Grid/WSU/"
export CAP_GRID_PATH_ROM="$CAP_ROOT_PATH/Grid/ROM/"
export CAP_MACROS_PATH="$CAP_SRC_PATH/Macros/"
export PATH="$CAP_BIN_PATH:$PATH"
export DYLD_LIBRARY_PATH="$CAP_LIB_PATH:$DYLD_LIBRARY_PATH"
export LD_LIBRARY_PATH="$CAP_LIB_PATH:$LD_LIBRARY_PATH"
export CAP_PROJECTS_PATH="$CAP_ROOT_PATH/projects/"
export CAP_DATABASE_PATH="$CAP_ROOT_PATH/DB/"
export CAP_DATA_IMPORT_PATH="$CAP_USER_DATA_IMPORT_PATH"
export CAP_DATA_EXPORT_PATH="$CAP_USER_DATA_EXPORT_PATH"
export CAP_CALIB_IMPORT_PATH="$CAP_USER_DATA_IMPORT_PATH"
export CAP_CALIB_EXPORT_PATH="$CAP_USER_DATA_EXPORT_PATH"
export CAP_HISTOS_IMPORT_PATH="$CAP_USER_HISTO_IMPORT_PATH"
export CAP_HISTOS_EXPORT_PATH="$CAP_USER_HISTO_EXPORT_PATH"

export CAP_PYTHIA8_PATH="$CAP_ROOT_PATH/pythia8312/"
export CAP_PYTHIA_ROOT_DIR="$CAP_PYTHIA8_PATH"
export CAP_PYTHIA8_INCLUDE_PATH="$CAP_PYTHIA8_PATH/include/"
export CAP_PYTHIA8_LIB_PATH="$CAP_PYTHIA8_PATH/lib/"
export CAP_FASTJET_PATH="$CAP_ROOT_PATH/fastjet-3.4.3/"
export CAP_FASTJET_INCLUDE_PATH="$CAP_FASTJET_PATH/include/"
export CAP_FASTJET_LIB_PATH="/usr/local/lib/"


echo "========================================================================================"
echo " Defined environment variables"
echo "========================================================================================"
echo " "
echo " CAP_USER_HISTO_IMPORT_PATH.................................: " $CAP_USER_HISTO_IMPORT_PATH
echo " CAP_USER_HISTO_EXPORT_PATH.................................: " $CAP_USER_HISTO_EXPORT_PATH
echo " CAP_USER_DATA_IMPORT_PATH..................................: " $CAP_USER_DATA_IMPORT_PATH
echo " CAP_USER_DATA_EXPORT_PATH..................................: " $CAP_USER_DATA_EXPORT_PATH
echo " CAP_ROOT_PATH..............................................: " $CAP_ROOT_PATH
echo " CAP_SRC_PATH...(source)....................................: " $CAP_SRC_PATH
echo " CAP_BIN_PATH...(binary)....................................: " $CAP_BIN_PATH
echo " CAP_LIB_PATH...(libs)......................................: " $CAP_LIB_PATH
echo " CAP_GRID_PATH...(GRID).....................................: " $CAP_GRID_PATH
echo " CAP_GRID_PATH_WSU...(WSU)..................................: " $CAP_GRID_PATH_WSU
echo " CAP_GRID_PATH_ROM...(WSU)..................................: " $CAP_GRID_PATH_ROM
echo " CAP Database   (CAP_DATABASE_PATH).........................: " $CAP_DATABASE_PATH
echo " CAP_PROJECTS_PATH..........................................: " $CAP_PROJECTS_PATH
echo " CAP_GRID_PATH..............................................: " $CAP_GRID_PATH
echo " CAP_MACROS_PATH............................................: " $CAP_MACROS_PATH
echo " CAP_DATA_IMPORT_PATH.......................................: " $CAP_DATA_IMPORT_PATH
echo " CAP_DATA_EXPORT_PATH.......................................: " $CAP_DATA_EXPORT_PATH
echo " CAP_HISTOS_IMPORT_PATH.....................................: " $CAP_HISTOS_IMPORT_PATH
echo " CAP_HISTOS_EXPORT_PATH.....................................: " $CAP_HISTOS_EXPORT_PATH
echo " CAP_CALIB_IMPORT_PATH......................................: " $CAP_CALIB_IMPORT_PATH
echo " CAP_CALIB_EXPORT_PATH......................................: " $CAP_CALIB_EXPORT_PATH
echo " CAP_PYTHIA8_PATH...........................................: " $CAP_PYTHIA8_PATH
echo " CAP_PYTHIA8_INCLUDE_PATH...................................: " $CAP_PYTHIA8_INCLUDE_PATH
echo " CAP_PYTHIA8_LIB_PATH.......................................: " $CAP_PYTHIA8_LIB_PATH
echo " CAP_FASTJET_PATH...........................................: " $CAP_FASTJET_PATH
echo " CAP_FASTJET_INCLUDE_PATH...................................: " $CAP_FASTJET_INCLUDE_PATH
echo " CAP_FASTJET_LIB_PATH.......................................: " $CAP_FASTJET_LIB_PATH
echo " "
echo "========================================================================================"
echo " Setup_CAP completed!"
echo "========================================================================================"
