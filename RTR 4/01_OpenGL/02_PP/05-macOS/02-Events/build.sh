mkdir -p Window.app/contents/MacOS
clang -o Window.app/contents/MacOS/Window Window.m -framework Cocoa
