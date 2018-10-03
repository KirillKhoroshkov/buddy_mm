cmake_minimum_required(VERSION 3.12)
project(buddy_mm C)

set(CMAKE_C_STANDARD 11)

add_executable(buddy_mm tests/mm_test.c buddy_mm/buddy_mm.h properties/errors.c properties/errors.h properties/commands.h properties/commands.c properties/messages.h properties/settings.h parser/parser.h)