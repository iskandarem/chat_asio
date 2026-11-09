find_package(boost_asio CONFIG REQUIRED)

add_library(chat_client_lib
	client.hpp
	client.cpp
)

target_link_libraries(chat_client_lib
	PUBLIC
		Boost::asio
)

target_include_directories(chat_client_lib
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(chat_client
	main.cpp
)
	
target_link_libraries(chat_client
	PRIVATE
		chat_client_lib
)
