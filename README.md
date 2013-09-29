## About

A collection of small sample programs for systems that include Intel Xeon Phi
(MIC architecture) coprocessors.

## Prerequisites

* Intel C/C++ Compiler (ICC)
* [Intel Manycore Platform Software Stack (MPSS)](http://software.intel.com/de-de/articles/intel-manycore-platform-software-stack-mpss)

Programs will run on the host processor if no Xeon Phi is installed.

## Build

```
git clone https://github.com/aprell/mic-intro.git

cd mic-intro

./build.sh --help

$ build.sh
  Build for hybrid execution (offload mode)

$ build.sh host
  Build for execution on host processor

$ build.sh mic
  Build for native execution on Xeon Phi

./build.sh
```
