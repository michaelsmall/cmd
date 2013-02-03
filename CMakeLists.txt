CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)
PROJECT(cmd)

ENABLE_TESTING()
ADD_DEFINITIONS(-std=c++0x -Wall -Wextra)

INCLUDE_DIRECTORIES(include)

#ADD_LIBRARY(cmd src/cmd.cpp)

ADD_EXECUTABLE(testcmd test/testcmd.cpp)
#TARGET_LINK_LIBRARIES(testcmd cmd)

ADD_TEST(Test-All testcmd)

