# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 3.19

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF
SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2021.1.1\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2021.1.1\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Local_documents\VS\ProjetC\projet_c_v6.0\clion

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Local_documents\VS\ProjetC\projet_c_v6.0\clion\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles\two048.dir\depend.make

# Include the progress variables for this target.
include CMakeFiles\two048.dir\progress.make

# Include the compile flags for this target's objects.
include CMakeFiles\two048.dir\flags.make

CMakeFiles\two048.dir\C_\Local_documents\VS\ProjetC\projet_c_v6.0\tests\two048.c.obj: CMakeFiles\two048.dir\flags.make
CMakeFiles\two048.dir\C_\Local_documents\VS\ProjetC\projet_c_v6.0\tests\two048.c.obj: C:\Local_documents\VS\ProjetC\projet_c_v6.0\tests\two048.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Local_documents\VS\ProjetC\projet_c_v6.0\clion\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/two048.dir/C_/Local_documents/VS/ProjetC/projet_c_v6.0/tests/two048.c.obj"
	C:\PROGRA~2\MIB055~1\2019\ENTERP~1\VC\Tools\MSVC\1428~1.293\bin\Hostx64\x64\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\two048.dir\C_\Local_documents\VS\ProjetC\projet_c_v6.0\tests\two048.c.obj /FdCMakeFiles\two048.dir\ /FS -c C:\Local_documents\VS\ProjetC\projet_c_v6.0\tests\two048.c
<<

CMakeFiles\two048.dir\C_\Local_documents\VS\ProjetC\projet_c_v6.0\tests\two048.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/two048.dir/C_/Local_documents/VS/ProjetC/projet_c_v6.0/tests/two048.c.i"
	C:\PROGRA~2\MIB055~1\2019\ENTERP~1\VC\Tools\MSVC\1428~1.293\bin\Hostx64\x64\cl.exe > CMakeFiles\two048.dir\C_\Local_documents\VS\ProjetC\projet_c_v6.0\tests\two048.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Local_documents\VS\ProjetC\projet_c_v6.0\tests\two048.c
<<

CMakeFiles\two048.dir\C_\Local_documents\VS\ProjetC\projet_c_v6.0\tests\two048.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/two048.dir/C_/Local_documents/VS/ProjetC/projet_c_v6.0/tests/two048.c.s"
	C:\PROGRA~2\MIB055~1\2019\ENTERP~1\VC\Tools\MSVC\1428~1.293\bin\Hostx64\x64\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\two048.dir\C_\Local_documents\VS\ProjetC\projet_c_v6.0\tests\two048.c.s /c C:\Local_documents\VS\ProjetC\projet_c_v6.0\tests\two048.c
<<

# Object files for target two048
two048_OBJECTS = \
"CMakeFiles\two048.dir\C_\Local_documents\VS\ProjetC\projet_c_v6.0\tests\two048.c.obj"

# External object files for target two048
two048_EXTERNAL_OBJECTS =

two048.exe: CMakeFiles\two048.dir\C_\Local_documents\VS\ProjetC\projet_c_v6.0\tests\two048.c.obj
two048.exe: CMakeFiles\two048.dir\build.make
two048.exe: C:\Local_documents\VS\ProjetC\projet_c_v6.0\_win\eibase64.lib
two048.exe: C:\Local_documents\VS\ProjetC\SDL2_windows\lib\x64\SDL2.lib
two048.exe: C:\Local_documents\VS\ProjetC\SDL2_windows\lib\x64\SDL2_gfx.lib
two048.exe: C:\Local_documents\VS\ProjetC\SDL2_windows\lib\x64\SDL2_ttf.lib
two048.exe: C:\Local_documents\VS\ProjetC\SDL2_windows\lib\x64\FreeImage.lib
two048.exe: CMakeFiles\two048.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Local_documents\VS\ProjetC\projet_c_v6.0\clion\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable two048.exe"
	"C:\Program Files\JetBrains\CLion 2021.1.1\bin\cmake\win\bin\cmake.exe" -E vs_link_exe --intdir=CMakeFiles\two048.dir --rc=C:\PROGRA~2\WI3CF2~1\10\bin\100183~1.0\x64\rc.exe --mt=C:\PROGRA~2\WI3CF2~1\10\bin\100183~1.0\x64\mt.exe --manifests -- C:\PROGRA~2\MIB055~1\2019\ENTERP~1\VC\Tools\MSVC\1428~1.293\bin\Hostx64\x64\link.exe /nologo @CMakeFiles\two048.dir\objects1.rsp @<<
 /out:two048.exe /implib:two048.lib /pdb:C:\Local_documents\VS\ProjetC\projet_c_v6.0\clion\cmake-build-debug\two048.pdb /version:0.0 /machine:x64 /debug /INCREMENTAL /subsystem:console  ei.lib C:\Local_documents\VS\ProjetC\projet_c_v6.0\_win\eibase64.lib C:\Local_documents\VS\ProjetC\SDL2_windows\lib\x64\SDL2.lib C:\Local_documents\VS\ProjetC\SDL2_windows\lib\x64\SDL2_gfx.lib C:\Local_documents\VS\ProjetC\SDL2_windows\lib\x64\SDL2_ttf.lib C:\Local_documents\VS\ProjetC\SDL2_windows\lib\x64\FreeImage.lib kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib 
<<

# Rule to build all files generated by this target.
CMakeFiles\two048.dir\build: two048.exe

.PHONY : CMakeFiles\two048.dir\build

CMakeFiles\two048.dir\clean:
	$(CMAKE_COMMAND) -P CMakeFiles\two048.dir\cmake_clean.cmake
.PHONY : CMakeFiles\two048.dir\clean

CMakeFiles\two048.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" C:\Local_documents\VS\ProjetC\projet_c_v6.0\clion C:\Local_documents\VS\ProjetC\projet_c_v6.0\clion C:\Local_documents\VS\ProjetC\projet_c_v6.0\clion\cmake-build-debug C:\Local_documents\VS\ProjetC\projet_c_v6.0\clion\cmake-build-debug C:\Local_documents\VS\ProjetC\projet_c_v6.0\clion\cmake-build-debug\CMakeFiles\two048.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles\two048.dir\depend

