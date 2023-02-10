@echo off
if not exist "C:\dev\vcpkg" (
    mkdir C:\dev
    cd C:\dev
    git clone https://github.com/Microsoft/vcpkg.git
)
cd C:\dev\vcpkg 
if not exist "vcpkg.exe" (
    call bootstrap-vcpkg.bat
)

set VCPKG_DEFAULT_TRIPLET=x64-windows
rem Add your library ports here.
vcpkg install --recurse doctest gtest opencv imgui opengl glfw3 glew freeglut fmt osg
