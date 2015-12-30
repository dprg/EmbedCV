#!/bin/sh


#
# Example from webpage: http://golem5.org/embedcv/example_obj.html
#

echo "Remember to press the 'q' key and advance to the next image"


echo
echo "Display the original image"
echo "	cat pooltable.jpg | ./jpg2ppm | display -"
cat pooltable.jpg | ./jpg2ppm | display -

echo
echo "Detect Sobel edges"
echo "	cat pooltable.jpg | ./jpg2ppm | ./ppm2edge | display -"
cat pooltable.jpg | ./jpg2ppm | ./ppm2edge | display -

echo
echo "Blur away noisy artifacts before detecting edges"
echo "	cat pooltable.jpg | ./jpg2ppm | ./ppm2blur -r 10 | ./ppm2edge | display -"
cat pooltable.jpg | ./jpg2ppm | ./ppm2blur -r 10 | ./ppm2edge | display -

echo
echo "Show box feature image"
echo "	cat pooltable.jpg | ./jpg2ppm | ./ppm2fbox | display -"
cat pooltable.jpg | ./jpg2ppm | ./ppm2fbox | display -

echo
echo "Show box feature blob boundaries (oops, blobs are too large)"
echo "	cat pooltable.jpg | ./jpg2ppm | ./ppm2fbox | ./ppm2morph -r -d 1 | ./ppm2morph -g -d 1 | display -"
cat pooltable.jpg | ./jpg2ppm | ./ppm2fbox | ./ppm2morph -r -d 1 | ./ppm2morph -g -d 1 | display -

echo
echo "Show box feature blob boundaries (good, blobs shrunk with thresholding)"
echo "	cat pooltable.jpg | ./jpg2ppm | ./ppm2fbox -s 13 | ./ppm2morph -r -d 1 | ./ppm2morph -g -d 1 | display -"
cat pooltable.jpg | ./jpg2ppm | ./ppm2fbox -s 13 | ./ppm2morph -r -d 1 | ./ppm2morph -g -d 1 | display -

echo
echo "Look for objects (oops, found too many)"
echo "	cat pooltable.jpg | ./jpg2ppm | ./ppm2fbox -s 13 | ./ppm2morph -r -d 1 | ./ppm2morph -g -d 1 | ./ppm2tbox | display -"
cat pooltable.jpg | ./jpg2ppm | ./ppm2fbox -s 13 | ./ppm2morph -r -d 1 | ./ppm2morph -g -d 1 | ./ppm2tbox | display -

echo
echo "Look for objects (works pretty well)"
echo "	cat pooltable.jpg | ./jpg2ppm | ./ppm2fbox -s 13 | ./ppm2morph -r -d 1 | ./ppm2morph -g -d 1 | ./ppm2tbox -t 50 -r 50 | display -"
cat pooltable.jpg | ./jpg2ppm | ./ppm2fbox -s 13 | ./ppm2morph -r -d 1 | ./ppm2morph -g -d 1 | ./ppm2tbox -t 50 -r 50 | display -

echo
echo "Display original image"
echo "	cat rocks.jpg | ./jpg2ppm | display -"
cat rocks.jpg | ./jpg2ppm | display -

echo
echo "Look for objects"
echo "	cat rocks.jpg | ./jpg2ppm | ./ppm2fbox -s 13 | ./ppm2morph -r -d 1 | ./ppm2morph -g -d 1 | ./ppm2tbox -t 100 -r 50 | display -"
cat rocks.jpg | ./jpg2ppm | ./ppm2fbox -s 13 | ./ppm2morph -r -d 1 | ./ppm2morph -g -d 1 | ./ppm2tbox -t 100 -r 50 | display -

