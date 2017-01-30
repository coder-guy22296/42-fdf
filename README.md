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
