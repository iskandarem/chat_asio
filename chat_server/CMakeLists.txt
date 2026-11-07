
find_package(boost_program_options CONFIG REQUIRED)
find_package(boost_asio CONFIG REQUIRED)

add_library(chat_server_lib
	chat.hpp 
	mem_func_comp_handler.hpp
)

target_link_libraries(chat_server_lib
	PUBLIC
		Boost::program_options
		Boost::asio
)

target_include_directories(chat_server_lib
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(chat_server
	main.cpp
)

target_link_libraries(chat_server
	PRIVATE
		chat_server_lib
)
