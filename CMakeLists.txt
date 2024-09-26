cmake_minimum_required(VERSION 3.18)

# Custom variables:
# * OW_PREFIX => Prefix to ObjectWrapper library's installation directory


project(ArgumentParser
	VERSION 0.0.1
	DESCRIPTION "A library for cross-platform argument parsing"
	LANGUAGES C)


function(END_NO_SLASH p result)
	if(${p} MATCHES ".*/$")
		string(LENGTH ${p} p_len)
		math(EXPR p_len "${p_len} - 1")
		string(SUBSTRING ${p} 0 ${p_len} p)
	endif()
	set(${result} ${p} PARENT_SCOPE)
endfunction()

function(MAKE_LIBRARY_VARS prefix lib result_include result_lib)
	if(WIN32)
		set(lib_suffix ".lib")
	endif()

	if(prefix)
		set(${result_include} "${prefix}/include" PARENT_SCOPE)
		set(${result_lib} "${prefix}/lib/${lib}${lib_suffix}" PARENT_SCOPE)
	else()
		set(${result_include} "" PARENT_SCOPE)
		set(${result_lib} "${lib}${lib_suffix}" PARENT_SCOPE)
	endif()
endfunction()

# Ensure correct path format
END_NO_SLASH(${OW_PREFIX} OW_PREFIX)
MAKE_LIBRARY_VARS(${OW_PREFIX} "objectwrapper" OW_INCLUDE OW_LIB)
message("${OW_LIB}")

if(CMAKE_HOST_UNIX)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g3")
endif()


set(CMAKE_C_STANDARD 11)
include(GNUInstallDirs)
include_directories(includes ${OW_INCLUDE})
link_libraries(${OW_LIB})

add_library(ow_argumentparser SHARED
	src/argument_parser.c)

set_target_properties(ow_argumentparser PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(ow_argumentparser PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})


enable_testing()

macro(TestCase name)
	add_executable(${name} ${ARGN})
	add_test(NAME ${name} COMMAND ./${name})
	if(NOT WIN32)
		add_test(NAME memcheck_${name} COMMAND valgrind --error-exitcode=1 --leak-check=full ./${name})
	endif()
endmacro()

TestCase(test_argument_component
	tests/unit_test/argument_component.c
	src/argument_parser.c)

TestCase(test_argument_parser
	tests/full_test/tester.c
	src/argument_parser.c)



# Add installation steps
install(TARGETS ow_argumentparser
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

install(DIRECTORY includes/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
