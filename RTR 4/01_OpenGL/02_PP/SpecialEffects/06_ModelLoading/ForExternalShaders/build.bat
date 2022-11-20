cls

del OGLMain.exe

del OGLMain.obj

del OGLMain.res

cl.exe /c /EHsc /I C:\glew\include OGLMain.cpp GLLog.cpp GLShaders.cpp ./MESH_LOADING/mesh.cpp

rc.exe OGLMain.rc

link.exe OGLMain.obj GLLog.obj GLShaders.obj mesh.obj OGLMain.res /LIBPATH:C:\glew\lib\Release\x64 user32.lib gdi32.lib /SUBSYSTEM:WINDOWS

OGLMain.exe
