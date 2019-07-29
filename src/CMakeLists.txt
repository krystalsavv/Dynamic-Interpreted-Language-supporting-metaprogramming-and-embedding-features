add_subdirectory(parser)
add_subdirectory(utilities)
add_subdirectory(interpreter)

add_executable(InterpreterExec main.cpp) 

target_link_libraries(  InterpreterExec             # To
                        Object                  # What
                        Parser
						Value
						Evaluator
						EnvironmentHolder
						EvaluatorActions
						ValueReference
						Interpreter
                        )                