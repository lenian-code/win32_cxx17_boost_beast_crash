
# Building with Boost Beast fails for Win32 Release Config on VS2017 15.9.24

## Steps to build the project

Open your Visual Studio 2017 Command Prompt. 
Navigate to this folder containing the main.cpp. 
And then enter the following command:

`
cl /EHsc /MD /std:c++17 /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_WINDOWS" /I D:\downloads/boost_1_73_0 main.cpp /link /LIBPATH:D:\downloads/boost_1_73_0/stage/lib
`

This line assumes you have boost 1.73.0 at `D:/downloads/`. 
Change the command prompt code if your boost is installed in another folder.
## Run after build finishes
When the build is done enter `main` into the command prompt.
When main is executed it crashes with `exception code 80000003`
