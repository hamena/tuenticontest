clear all
close all
clc

A=imread('05-headache');
if ndims(A)>2
    A=rgb2gray(A);
end

B = binarize(A);

figure, imshow(B);


