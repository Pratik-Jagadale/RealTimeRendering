cls

del OGL.exe

del OGL.obj

del OGL.res

rc.exe OGL.rc

cl.exe /c /EHsc /I "C:\glew\include" -I "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.6\include" OGL.cpp

rc.exe OGL.rc

link.exe OGL.obj OGL.res /LIBPATH:C:\glew\lib\Release\x64 /LIBPATH:"C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.6\lib\x64" user32.lib gdi32.lib /SUBSYSTEM:WINDOWS
