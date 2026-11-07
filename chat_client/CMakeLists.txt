find_package(boost_asio CONFIG REQUIRED)

add_library(chat_client_lib
	client.hpp
)

target_link_libraries(chat_client_lib
	PUBLIC
		Boost::asio
)

target_link_directories(chat_client_lib
	PUBLIC
		${CMAKE_CURRENT_DIR}
)

add_executable(chat_client
	main.cpp
)
	
target_link_libraries(chat_client
	PRIVATE
		chat_client_lib
)
