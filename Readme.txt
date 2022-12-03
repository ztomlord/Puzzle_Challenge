cpp opencv windows install from scratch notes (TL 2022)
-download latest source code from github for opencv
-put in directory location where you are going to leave it
-run cmake (configure for vs x64 whichever version u want)
-(select the base source code directory and that directory/build), it will ask if u want to create a build directory say yes
-after configuring once, find the opencv_world flag and check it
-configure again and then generate
-open up the build folder and there should be a vs solution in there, open it.
-in solution explorer under CMakeTargets right click ALL_BUILD and build, then after complete right click INSTALL and build.  You will need to do this once in debug mode and once in release mode to fully build the libraries
-open environment variables and add the path to the build/bin that you just created

--to add opencv to a cpp project in visual studio:
- add include and library directories (for include make sure u take the include inside the build/install folder)
- in the linker additional dependancies, add opencv_world
