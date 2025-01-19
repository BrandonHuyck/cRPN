
project(crpn)

cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O0") 
set(CMAKE_BUILD_TYPE Debug)

include_directories("src")
include_directories("test")

# locate gtest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# lexer (0.0.2) unit tests
add_executable(token_tests tests/token_tests.cpp src/token.cpp)
target_link_libraries(token_tests ${GTEST_LIBRARIES} pthread)

add_executable(lexer_tests tests/lexer_tests.cpp src/token.cpp
  src/crpn_exception.cpp src/lexer.cpp)
target_link_libraries(lexer_tests ${GTEST_LIBRARIES} pthread)

# simple parser (0.0.3) unit tests
# add_executable(simple_parser_tests tests/simple_parser_tests.cpp
#   src/token.cpp src/crpn_exception.cpp src/lexer.cpp src/simple_parser.cpp)
# target_link_libraries(simple_parser_tests ${GTEST_LIBRARIES} pthread)

# ast parser (0.0.4) unit tests
# add_executable(ast_parser_tests tests/ast_parser_tests.cpp
#   src/token.cpp src/crpn_exception.cpp src/lexer.cpp src/ast_parser.cpp)
# target_link_libraries(ast_parser_tests ${GTEST_LIBRARIES} pthread)

# add_executable(parser_syntax_tests tests/parser_syntax_tests.cpp
#   src/token.cpp src/crpn_exception.cpp src/lexer.cpp src/ast_parser.cpp)
# target_link_libraries(parser_syntax_tests ${GTEST_LIBRARIES} pthread)

# semantic_checker (0.0.5) unit tests
# add_executable(semantic_checker_tests tests/semantic_checker_tests.cpp
#   src/token.cpp src/crpn_exception.cpp src/lexer.cpp src/ast_parser.cpp
#   src/symbol_table.cpp src/semantic_checker.cpp)
# target_link_libraries(semantic_checker_tests ${GTEST_LIBRARIES} pthread)

# vm (0.0.6) unit tests
# add_executable(vm_tests tests/vm_tests.cpp src/crpn_exception.cpp
#   src/vm_instr.cpp src/vm.cpp)
# target_link_libraries(vm_tests ${GTEST_LIBRARIES} pthread)

# code_generator (0.0.7) unit tests
# add_executable(code_generator_tests tests/code_generator_tests.cpp
#   src/token.cpp src/crpn_exception.cpp src/lexer.cpp src/ast_parser.cpp
#   src/vm.cpp src/vm_instr.cpp src/var_table.cpp src/code_generator)
# target_link_libraries(code_generator_tests ${GTEST_LIBRARIES} pthread)

# create crpn target
add_executable(crpn src/token.cpp src/crpn_exception.cpp src/lexer.cpp
  # src/simple_parser.cpp
  # src/ast_parser.cpp src/print_visitor.cpp
  # src/symbol_table.cpp src/semantic_checker.cpp
  # src/vm_instr.cpp src/vm.cpp
  # src/var_table.cpp src/code_generator.cpp
  src/crpn.cpp)
