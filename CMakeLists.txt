cmake_minimum_required (VERSION 3.28.1)


project (chat_server LANGUAGES CXX)

include(CTest)

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(chat_server)

if(BUILD_TESTING)
	add_subdirectory(tests)
endif()

