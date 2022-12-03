Jigsaw puzzles are created using this site: https://puzzle.telegnom.org/
I converted it to a jpg using: https://image.online-convert.com/convert/svg-to-jpg


Biggest *todo* is to convert the file location, name, and debug visuals flag to arguments rather than hardcoded in


cpp opencv windows install from scratch notes (TL 2022)
-download latest source code from github for opencv
-put in directory location where you are going to leave it
-run cmake (configure for vs x64 whichever version u want)
-(select the base source code directory and that directory/build), it will ask if u want to create a build directory say yes
-after configuring once, find the opencv_world flag and check it
-configure again and then generate
-open up the build folder and there should be a vs solution in there, open it.
-in solution explorer under CMakeTargets right click ALL_BUILD and build, then after complete right click INSTALL and build.  You will need to do this once in debug mode and once in release mode to fully build the libraries
-open environment variables and add the path to the build/install/... that you just created
	example: (C:\LocalDevelopment\Third_Party\Open_CV\opencv-4.x\opencv-4.x\build\install\x64\vc17\bin)

--to add opencv to a cpp project in visual studio:
- add include and lib directories (these should be in the build\install... folder)
	example:
	include dirs: (C:\LocalDevelopment\Third_Party\Open_CV\opencv-4.x\opencv-4.x\build\install\include)
	lib dirs: (C:\LocalDevelopment\Third_Party\Open_CV\opencv-4.x\opencv-4.x\build\install\x64\vc17\lib)
- in the linker additional dependancies, add opencv_world460.lib (version dependant) (specify the debug one in debug configuration if u want)
