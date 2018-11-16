-------------------------------INTRODUCE--------------------------------------------
basic C++ game framework

------------------------------HOW TO USE THIS CODE PROPERLY-------------------------

Framework contains all the basic properties we need to create our game
How to create a game: create a child class inherited from Game class and just work around two main functions: CreateGame() and UpdateGame()  


--------------HOW TO SETUP YOUR PROJECT TO USE FRAMEWORK CODE -------------------------

#Go to project -> properties -> C/C++ -> Additional Include Directories ->
and add $(SolutionDir)framework\include\ (this is the macro for the path ..\..\framework\include\)

If Framework is in the same solution with your project, you can go to References under your project name -> add Framework 
or you can use the way below here

If not, Go to project -> properties -> linker -> general -> addtional library directories and add the macro for the folder containing framework.lib
						or add this in VC++ directories -> library directories
Next, Go to project -> properties -> linker -> input -> addtional dependencies and add framework.lib


------------------------------THINGS TO DO (OWNERs' WORK)-----------------------------
