% demo.m
clear all;
close all;
clc;

addpath('mex')

%----------------
a = imread('ar.jpg');
b = imread('br.jpg');
w = 8;
s = 1;
Niter = 3;

%---------------
nn = subPatch(a, b, w, s, Niter)

