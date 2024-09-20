cmake_minimum_required(VERSION 3.18)


if(CMAKE_HOST_UNIX)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g3")
endif()


project(ArgumentParser
	VERSION 0.0.1
	DESCRIPTION "A library for cross-platform argument parsing"
	LANGUAGES C)

include(GNUInstallDirs)
include_directories(includes)
link_libraries(objectwrapper)

add_library(ow_argumentparser SHARED
	src/argument_parser.c)

set_target_properties(ow_argumentparser PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(ow_argumentparser PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})


enable_testing()

macro(TestCase name)
add_executable(${name} ${ARGN})
add_test(NAME ${name} COMMAND ./${name})
add_test(NAME memcheck_${name} COMMAND valgrind --error-exitcode=1 --leak-check=full ./${name})
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
