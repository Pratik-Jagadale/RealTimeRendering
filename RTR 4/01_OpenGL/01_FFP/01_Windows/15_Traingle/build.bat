cls

del OGL.exe

del OGL.obj

del OGL.res

cl.exe OGL.cpp /c /EHsc OGL.cpp

rc.exe OGL.rc

link.exe OGL.obj OGL.res user32.lib gdi32.lib /SUBSYSTEM:WINDOWS

OGL.exe
