# Run with: python3 build.py
import os
import platform

# (1)==================== COMMON CONFIGURATION OPTIONS ======================= #
COMPILER="g++ -std=c++17"   # The compiler we want to use 
SOURCE="./src/*.cpp"        # Where the source code lives
EXECUTABLE="engine"        # Name of the final executable
# ======================= COMMON CONFIGURATION OPTIONS ======================= #

# (2)=================== Platform specific configuration ===================== #
# For each platform we need to set the following items
ARGUMENTS=""            # Arguments needed for our program (Add others as you see fit)
INCLUDE_DIR=""          # Which directories do we want to include.
LIBRARIES=""            # What libraries do we want to include

if platform.system()=="Linux":
    ARGUMENTS="-D LINUX" # -D is a #define sent to preprocessor
    INCLUDE_DIR="-I ./include/ -I./include/glm/"
    LIBRARIES="-lSDL2 -ldl"
elif platform.system()=="Darwin":
    ARGUMENTS="-D MAC" # -D is a #define sent to the preprocessor.
    INCLUDE_DIR = "-I ./include/ -I/opt/homebrew/include/SDL2 -I./include/glm/"
    LIBRARIES = "-L/opt/homebrew/lib -lSDL2"
elif platform.system()=="Windows":
    ARGUMENTS="-D MINGW -static-libgcc -static-libstdc++" 
    INCLUDE_DIR="-I./include/ -I./include/glm/"
    EXECUTABLE="engine.exe"
    LIBRARIES="-lmingw32 -lSDL2main -lSDL2 -mwindows -mconsole"
# (2)=================== Platform specific configuration ===================== #

# (3)====================== Building the Executable ========================== #
# Build a string of our compile commands that we run in the terminal
compileString=COMPILER+" "+ARGUMENTS+" -o "+EXECUTABLE+" "+" "+INCLUDE_DIR+" "+SOURCE+" "+LIBRARIES
# Print out the compile string
# This is the command you can type
print("============v (Command running on terminal) v===========================")
print("Compilng on: "+platform.system())
print(compileString)
print("========================================================================")
# Run our command 
# Here I am using an exit_code so you can
# also compile & run in one step as
# python3 build.py && ./engine
# If compilation failes, ./engine will not run.
exit_code = os.system(compileString)
exit(0 if exit_code==0 else 1)
# ========================= Building the Executable ========================== #


