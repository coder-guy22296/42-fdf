# 42-fdf
I built a Graphics library and used it to build this 3d wireframe renderer, that can load multiple models, translate and rotate them independently in 3d space. I used perspective projection to make the objects look more realistic. You can also move the camera around in 3d space. The only thing I was given was a function to put a single pixel on the screen and hooks for user input events

# Demo Instructions (Mac OSX El Capitan)
- open Terminal and run commands below
- git clone https://github.com/coder-guy22296/42-fdf
- git submodule init
- git submodule update
- make && ./fdf my42.fdf 42.fdf
- if that last one doesn't work try this one:
- make re && ./fdf my42.fdf 42.fdf


# Controls (all movement controls currently move along the x y and z axis)
- Move Camera: W,A,S,D,R and F Keys
- Rotate Camera: click and drag the mouse
- Move Selected object: arrow keys and page up/page down
- Rotate Selected object: NUMPAD 4 to NUMPAD 9
- Change Selected object: NUMPAD 1
