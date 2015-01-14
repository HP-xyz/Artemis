project(artemis_server)
cmake_minimum_required(VERSION 2.8)

#link_directories(${BOOST_ROOT}/lib)

if (NOT WIN32)
	# We need this for unix machines since Qt internally uses
	# the pthread library (I think :D)
    find_package(Threads REQUIRED)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lrt -ldl")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lrt -ldl")
endif (NOT WIN32)

set (ARTEMIS_SERVER_SRCS
	${artemis_server_SOURCE_DIR}/src/Artemis_Server.cpp
	${artemis_server_SOURCE_DIR}/src/Artemis_Server_Connection.cpp
	${artemis_server_SOURCE_DIR}/src/Artemis_Request_Handler.cpp
	${artemis_server_SOURCE_DIR}/src/Artemis_Network_Sender_Connection.cpp
	)
set (ARTEMIS_SERVER_HDRS
	${artemis_server_SOURCE_DIR}/src/Artemis_Server.h
	${artemis_server_SOURCE_DIR}/src/Artemis_Server_Connection.h
	${artemis_server_SOURCE_DIR}/src/Artemis_Request_Handler.h
	${artemis_server_SOURCE_DIR}/src/Artemis_Network_Sender_Connection.h
	)

include_directories(
		${Boost_INCLUDE_DIRS}
		${PUGIXML_INCLUDE_DIR}
		${Tacktech_Common_INCLUDE_DIR}
		${LIBB64_INCLUDE_DIR}
		)

add_executable(Artemis_Server 
		main.cpp
		${ARTEMIS_SERVER_SRCS}
		${ARTEMIS_SERVER_HDRS}
		)
if(MINGW AND __COMPILER_GNU)
  target_link_libraries(Artemis_Server ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ws2_32 wsock32 pugixml libb64 Tacktech_Common)
else(MINGW AND __COMPILER_GNU)
	target_link_libraries(Artemis_Server
						${Boost_LIBRARIES}
						${CMAKE_THREAD_LIBS_INIT}
						pugixml
						libb64
						Tacktech_Common)
endif(MINGW AND __COMPILER_GNU)


