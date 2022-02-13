cls

del OGL.exe

del OGL.obj

del OGL.res

cl.exe Window.c /c /EHsc OGL.cpp

rc.exe Window.rc

link.exe Window.obj Window.res user32.lib gdi32.lib /SUBSYSTEM:WINDOWS

Window.exe
