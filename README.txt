CS184 Project 1: Phong shader

Partner 1: Kyle Boss cs184-ck
Partner 2: Jingyi Li cs184-ec
Code runs on OS X
cs184-ck will have the code (compiled and ran on OS X) and cs184-ec will submit the readme. 

COMMAND LINE ARGUMENTS
-ka r g b
-kd r g b
-ks r g b
Specifies ambient, diffuse, and specular color coefficients in RGB (floats between 0 and 1) respectively.

-sp v
Sets specular coefficient.

-pl x y z r g b
-dl x y z r g b
Adds a point or directional light of RGB at (X, Y, Z).

EXTRA CREDIT FEATURES

-t
Turns on the ‘toon’ flag, which implements cel shading by creating rough pixel thresholds.

-s
Turns on the ‘save’ flag, which saves image to current directory as img.bmp. Always put this flag at the end of the command line.
