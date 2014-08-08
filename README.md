# SUBPATCH

SubPatch is a program to compute the Approximate Nearest-Neighbor
field between image pairs. It is mainly written in Matlab with some C
parts. SubPatch requires FLANN library in order to work. This is a
freely avaialbe and implements algorithms for approximate
nearest-neighbor search. At the moment SubPatch supports only
GNU/Linux platform but can be easly ported to others.

## Install

### Requirements
    
#### FLANN

First of all you need to compile the lib
https://github.com/mariusmuja/flann. The process should be
straightforward, just enable `BUILD_MATLAB_BINDINGS`. Once the
compilation is done copy the files `flann_search.m` and
`nearest_neighbor.mex*` in a visible matlab path.

### Compile

Since SubPatch comes with some C parts, you need to compile these in
order to use it. So you must have `mex`, `gcc` and `make`. To compile
go to mex direcotry and launch make.

## Usage

Watch `demo.m` to see an example of execution.

