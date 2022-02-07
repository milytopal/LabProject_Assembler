cmake_minimum_required(VERSION 3.21)
project(Project_maman14 C)

set(CMAKE_C_STANDARD 90)

add_definitions(-Wall -pedantic)

add_executable(Project_maman14 main.c dataStructs.h
        dataLinkedList.h dataLinkedList.c
        parseCodeToFile.c parseCodeToFile.h utils.c utils.h firstPass.c firstPass.h secondPass.c secondPass.h extrnVariables.h)
