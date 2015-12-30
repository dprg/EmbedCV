#!/bin/sh


#
# Example from webpage: http://golem5.org/embedcv/example_seg.html
#

echo "Remember to press the 'q' key and advance to the next image"


echo
echo "Display the original image"
echo "	cat parkpath.jpg | ./jpg2ppm | display -"
cat parkpath.jpg | ./jpg2ppm | display -

echo
echo "Display the chroma image channels"
echo "	cat parkpath.jpg | ./jpg2ppm | ./ppm2chroma -s 8 | display -"
cat parkpath.jpg | ./jpg2ppm | ./ppm2chroma -s 8 | display -

echo
echo "First try at chroma image segmentation (key subimage too small)"
echo "	cat parkpath.jpg | ./jpg2ppm | ./ppm2seg -x 320 -y 480 -p 16 | display -"
cat parkpath.jpg | ./jpg2ppm | ./ppm2seg -x 320 -y 480 -p 16 | display -

echo
echo "Second try at chroma image segmentation (key subimage big enough)"
echo "	cat parkpath.jpg | ./jpg2ppm | ./ppm2seg -x 320 -y 480 -p 128 | display -"
cat parkpath.jpg | ./jpg2ppm | ./ppm2seg -x 320 -y 480 -p 128 | display -

echo
echo "Luminance image segmentation (threshold too low)"
echo "	cat parkpath.jpg | ./jpg2ppm | ./ppm2seg -x 320 -y 480 -p 128 -l | display -"
cat parkpath.jpg | ./jpg2ppm | ./ppm2seg -x 320 -y 480 -p 128 -l | display -

echo
echo "Luminance image segmentation (threshold much higher)"
echo "	cat parkpath.jpg | ./jpg2ppm | ./ppm2seg -x 320 -y 480 -p 128 -l -t 20 | display -"
cat parkpath.jpg | ./jpg2ppm | ./ppm2seg -x 320 -y 480 -p 128 -l -t 20 | display -

echo
echo "Use morphological opening to clean image segmentation"
echo "	cat parkpath.jpg | ./jpg2ppm | ./ppm2seg -x 320 -y 480 -p 128 | ./ppm2morph -g -o 1 | display -"
cat parkpath.jpg | ./jpg2ppm | ./ppm2seg -x 320 -y 480 -p 128 | ./ppm2morph -g -o 1 | display -

echo
echo "Use more morphological opening to clean image segmentation"
echo "	cat parkpath.jpg | ./jpg2ppm | ./ppm2seg -x 320 -y 480 -p 128 | ./ppm2morph -g -o 3 | display -"
cat parkpath.jpg | ./jpg2ppm | ./ppm2seg -x 320 -y 480 -p 128 | ./ppm2morph -g -o 3 | display -

echo
echo "Use morphological opening and closing to clean image segmentation"
echo "	cat parkpath.jpg | ./jpg2ppm | ./ppm2seg -x 320 -y 480 -p 128 | ./ppm2morph -g -o 3 | ./ppm2morph -g -c 3 | display -"
cat parkpath.jpg | ./jpg2ppm | ./ppm2seg -x 320 -y 480 -p 128 | ./ppm2morph -g -o 3 | ./ppm2morph -g -c 3 | display -

