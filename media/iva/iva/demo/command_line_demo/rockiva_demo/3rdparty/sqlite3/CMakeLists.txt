cmake_minimum_required(VERSION 3.10)

project(sqlite3)

include_directories(./include)

add_library(sqlite3 STATIC
        src/sqlite3.c)