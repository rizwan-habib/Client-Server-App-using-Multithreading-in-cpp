cmake_minimum_required(VERSION 3.10)
project(YourProjectName)

# Set C++ standard to C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add executable for the server
add_executable(server server.cpp header.h db.h)

# Add executable for the test code
add_executable(tests test.cpp header.h db.h)

# Find and link against the SQLite library
find_library(SQLITE3_LIBRARY sqlite3)
target_link_libraries(server ${SQLITE3_LIBRARY})
target_link_libraries(tests ${SQLITE3_LIBRARY})

# Link against the Google Test library, pthread, and dl
find_package(Threads REQUIRED)
target_link_libraries(server Threads::Threads dl)
target_link_libraries(tests gtest gtest_main Threads::Threads)

# Set the output directory for the built binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set the installation directory for the built binaries
install(TARGETS server DESTINATION bin)

# Include packaging instructions
set(CPACK_PACKAGE_NAME "ChatServer")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_GENERATOR "ZIP")
include(CPack)
