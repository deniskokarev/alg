cmake_minimum_required(VERSION 3.8)
project(yalg)

set(CMAKE_CXX_STANDARD 11)

# place libs into lib and executables into bin directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(include)

# place all our code into one static library
add_library(yalg
	STATIC
	src/par.cpp
	src/prime.cpp
	src/prefix.cpp
)

# optionally build doxygen docs
set(DOXYGEN_EXECUTABLE "") # hmm, something's wrong with FindDoxygen module
find_package(Doxygen)
if (DOXYGEN_FOUND)
    # need to render Doxyfile during the build    
    configure_file(${CMAKE_SOURCE_DIR}/doc/Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )
else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)

# googletest mantra
ADD_SUBDIRECTORY(extern/googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# all tests
add_executable(segtree_test test/segtree_test.cpp include/segtree.hpp)
target_link_libraries(segtree_test gtest gtest_main)

add_executable(binomial_test test/binomial_test.cpp include/binomial.hpp)
target_link_libraries(binomial_test gtest gtest_main)

add_executable(par_test test/par_test.cpp include/par.hpp)
target_link_libraries(par_test yalg gtest gtest_main)

add_executable(heap_test test/heap_test.cpp include/heap.hpp)
target_link_libraries(heap_test gtest gtest_main)

add_executable(ilog_test test/ilog_test.cpp include/ilog.hpp)
target_link_libraries(ilog_test gtest gtest_main)

add_executable(mat_test test/mat_test.cpp include/mat.hpp include/hperm.hpp)
target_link_libraries(mat_test gtest gtest_main)

add_executable(prime_test test/prime_test.cpp include/prime.hpp)
target_link_libraries(prime_test yalg gtest gtest_main)

add_executable(prefix_test test/prefix_test.cpp include/prefix.hpp)
target_link_libraries(prefix_test yalg gtest gtest_main)

add_executable(nth_element_test test/nth_element_test.cpp include/nth_element.hpp)
target_link_libraries(nth_element_test yalg gtest gtest_main)

add_test(NAME segtree_test COMMAND segtree_test)
add_test(NAME binomial_test COMMAND binomial_test)
add_test(NAME par_test COMMAND par_test)
add_test(NAME heap_test COMMAND heap_test)
add_test(NAME ilog_test COMMAND ilog_test)
add_test(NAME mat_test COMMAND mat_test)
add_test(NAME prime_test COMMAND prime_test)
add_test(NAME prefix_test COMMAND prefix_test)
add_test(NAME nth_element_test COMMAND nth_element_test)

# explicit tests <- exe build dependency allows running 'ctest' right away
add_test(NAME building_all_tests
  COMMAND "${CMAKE_COMMAND}" --build "${CMAKE_BINARY_DIR}" -- -j)
set_tests_properties(building_all_tests PROPERTIES FIXTURES_SETUP bld)
set_tests_properties(
  segtree_test
  binomial_test
  par_test
  heap_test
  ilog_test
  mat_test
  prime_test
  prefix_test
  nth_element_test
  PROPERTIES FIXTURES_REQUIRED bld
)
