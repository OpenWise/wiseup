add_subdirectory (modules)
add_subdirectory (arch)

find_package (MySQL)
if (MYSQL_FOUND)
    message (STATUS "MySQL found at: ${MYSQL_INCLUDE_DIR}, ${MYSQL_LIBRARIES}")
else (MYSQL_FOUND)
#    message (SEND_ERROR "Couldn't find MySQL include files and/or library")
endif (MYSQL_FOUND)

include_directories (${PROJECT_SOURCE_DIR}/include)
add_executable (wise-nrfd wise-nrfd.cxx filelog.cxx wise_ipc.cxx wise_client_handler.cxx wiseNrfAckQueue.cxx jsoncpp.cpp wise_mysql.cxx wiseEventMng.cxx wiseCacheDB.cxx wiseDBMng.cxx)
target_link_libraries (wise-nrfd ${MYSQL_LIBRARIES} nrf24l01 gpio spi ${CMAKE_THREAD_LIBS_INIT})
include_directories (wise-nrfd PUBLIC ${MYSQL_INCLUDE_DIR})

add_executable (wise-eventd wise-eventd.cxx wise_mysql.cxx filelog.cxx wise_ipc.cxx wiseEventMng.cxx wiseCacheDB.cxx)
target_link_libraries (wise-eventd ${MYSQL_LIBRARIES})
include_directories (wise-eventd PUBLIC ${MYSQL_INCLUDE_DIR})
