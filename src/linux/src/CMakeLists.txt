add_subdirectory (modules)
add_subdirectory (arch)

find_package (MySQL)
if (MYSQL_FOUND)
    message (STATUS "MySQL found at: ${MYSQL_INCLUDE_DIR}, ${MYSQL_LIBRARIES}")
else (MYSQL_FOUND)
#    message (SEND_ERROR "Couldn't find MySQL include files and/or library")
endif (MYSQL_FOUND)

include_directories (${PROJECT_SOURCE_DIR}/include)
add_executable (wise-nrfd wise-nrfd.cxx filelog.cxx wise_ipc.cxx wise_client_handler.cxx wiseNrfAckQueue.cxx jsoncpp.cpp)
target_link_libraries (wise-nrfd nrf24l01 gpio spi ${CMAKE_THREAD_LIBS_INIT})

add_executable (wise-dbd wise_mysql.cxx filelog.cxx wise_ipc.cxx wise-dbd.cxx wiseCacheDB.cxx)
target_link_libraries (wise-dbd ${MYSQL_LIBRARIES})
include_directories (wise-dbd PUBLIC ${MYSQL_INCLUDE_DIR})
