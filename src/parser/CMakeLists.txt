set(BISON_OUTPUT ${PARSER_OUTPUT}/parser.cpp)
set(FLEX_OUTPUT ${PARSER_OUTPUT}/scanner.cpp)

bison_target(Parser parser.y ${BISON_OUTPUT} COMPILE_FLAGS -v)

if(WIN32)
    set(FLEX_COMPILE_FLAGS --wincompat)
else()
    set(FLEX_COMPILE_FLAGS "")
endif()

flex_target(Scanner scanner.l ${FLEX_OUTPUT} COMPILE_FLAGS ${FLEX_COMPILE_FLAGS})
add_flex_bison_dependency(Scanner Parser)

add_library(Parser STATIC ${PARSER_OUTPUT}/scanner.cpp
                          ${PARSER_OUTPUT}/parser.cpp)

target_link_libraries(  Parser
                        Object 
						AST
)