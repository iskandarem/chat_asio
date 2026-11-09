find_package(Python3 REQUIRED COMPONENTS Interpreter)

add_test(
	NAME chat_protocol_integration
	COMMAND
		${Python3_EXECUTABLE}
		${CMAKE_CURRENT_SOURCE_DIR}/chat_protocol_test.py
		--server $<TARGET_FILE:chat_server>
		--client $<TARGET_FILE:chat_client>
)

set_tests_properties(
	chat_protocol_integration
	PROPERTIES
		TIMEOUT 15
)
