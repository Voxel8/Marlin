cmake_minimum_required (VERSION 3.1)

project (registerer)

##################################
# Download and install GoogleTest

include(ExternalProject)
ExternalProject_Add(gtest
  URL https://googletest.googlecode.com/files/gtest-1.7.0.zip
  # Comment above line, and uncomment line below to use subversion.
  # SVN_REPOSITORY http://googletest.googlecode.com/svn/trunk/ 
  # Uncomment line below to freeze a revision (here the one for 1.7.0)
  # SVN_REVISION -r700

  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
  INSTALL_COMMAND ""
)
ExternalProject_Get_Property(gtest source_dir binary_dir)

add_definitions(-DUNIT_TEST)

################
# Define a test
add_executable(cartridge_test cartridge_test.cc)

######################################
# Configure the test to use GoogleTest
#
# If used often, could be made a macro.

add_dependencies(cartridge_test gtest)
include_directories(${source_dir}/include)
target_link_libraries(cartridge_test ${binary_dir}/libgtest.a)
target_link_libraries(cartridge_test ${binary_dir}/libgtest_main.a)

#########################cartridge_test#########
# Just make the test runnable with
#   $ make test

enable_testing()
add_test(NAME    cartridge_test 
         COMMAND cartridge_test)