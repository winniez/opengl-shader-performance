CSCI 5239 14 Spring
HW 4
Xinying Zeng

How to play with it?
Press down/ left/ right arrow key to rotate the perspective. 
Press 'o' to increase number of objects displayed.
Press 'm' to switch shaders.
Press 'd' / 'D' to increase/ decrease size of world.

In this homework I use 7 shader programs to test the performance of different ways of do things in shader. The 8 shader programs are derived from the brick, Mandelbrot and color shader. The "fps=num" shows frames per second.

My first change is to use int instead of float in "int Brick" and "int Mandelbrot" shader, e.g. vec3 to ivec2 etc. 
The performance change is trivial in this case. fps barely changes.

The second change is to substitude bulit-in functions with the functions I implemented. I explicit define mix, pow, max, normalize, reflect, fract function in "self defined brick" and "self defind func mandelbrot".
The performance does drop in this case, fps is about 3 frames fewer.

While the most obvious factor to performance I observed so far is the number of complex objects. I am using OSX 10.9 with Intel Iris graphics card. With brick shader, fps is 60 when I only have sephere and bunny. After adding dianosaur and armadillo, fps drops to around 30. When a building object is displayed, fps drops to around 26.  

The bunny has 35,947 vertices. 
The building has 41,923 vertices.

It took me around 8 hours to finish the homework.
