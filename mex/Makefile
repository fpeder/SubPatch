# Makefile
CC='gcc-4.4'
CXX='g++-4.4'
CFLAGS='-O3'

MEX=mex CC=$(CC) CXX=$(CXX) CFLAGS=$(CFLAGS)

all: mexImgToPvec2 mexNnImp

mexImgToPvec2: mexImgToPvec2.cpp
	$(MEX) $<

mexNnImp: mexNnImp.cpp mat.cpp
	$(MEX) $^ -lgomp

