add_subdirectory(parser)
add_subdirectory(utilities)

add_executable(Interpreter main.cpp) 

target_link_libraries(  Interpreter             # To
                        Object                  # What
                        Parser
						Value
                        )                