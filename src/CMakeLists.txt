add_subdirectory(parser)
add_subdirectory(utilities)

add_executable(AlphaDI main.cpp interpreter/Interpreter.cpp) 

target_link_libraries(  AlphaDI             # To
                        Object                  # What
                        Parser
						Value
						Evaluator
						EnvironmentHolder
						EvaluatorActions
						ValueReference
						MetaParser
						MetaUnparser
                    )                
