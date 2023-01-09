mkdir -p OGL.app/contents/MacOS
clang -Wno-deprecated-declarations -o OGL.app/contents/MacOS/OGL OGL.m -framework Cocoa -framework QuartzCore -framework OpenGL
