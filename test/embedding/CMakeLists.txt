add_executable(Embedding embed.cpp) 


target_link_libraries(  Embedding             # To
                        Object                  # What
                        Parser
						Value
						Evaluator
						EnvironmentHolder
						EvaluatorActions
						ValueReference
                        )            