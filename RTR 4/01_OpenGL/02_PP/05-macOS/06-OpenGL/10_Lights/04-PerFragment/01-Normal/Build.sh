mkdir -p OGL.app/contents/MacOS
clang -Wno-deprecated-declarations -o OGL.app/contents/MacOS/OGL OGL.mm -framework Cocoa -framework QuartzCore -framework OpenGL
