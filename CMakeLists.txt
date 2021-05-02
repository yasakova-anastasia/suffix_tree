cmake_minimum_required( VERSION 3.15 )

if(WIN32)
    option(gtest_force_shared_crt "" TRUE)
endif(WIN32)

include( "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Configure.cmake" )

############################### GTest ###############################
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/gtest")
include_directories("${CMAKE_SOURCE_DIR}/3rdparty/gtest/googletest/include")
if( WIN32 )
    include_directories("${CMAKE_SOURCE_DIR}/3rdparty/gtest-mpi/win")
else( WIN32 )
    include_directories("${CMAKE_SOURCE_DIR}/3rdparty/gtest-mpi/linux")
endif( WIN32 )


############################## CPPLINT ##############################
find_package(Python3 REQUIRED)
enable_testing()
if( NOT WIN32 )
    include(cmake/Cpplint.cmake)
    cpplint_add_subdirectory(src)
    cpplint_add_subdirectory(include)
endif( NOT WIN32 )

############################## Modules ##############################
include_directories(3rdparty/unapproved)

get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)

set(ProjectId "${ProjectId}")
project( ${ProjectId} )
message( STATUS "-- " ${ProjectId} )

set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include)
include_directories(${COMMON_INCLUDES})

file(GLOB ALL_SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp
		    ${PROJECT_SOURCE_DIR}/include/*.h
		    ${PROJECT_SOURCE_DIR}/test/*.cpp)

set(PACK_LIB "${ProjectId}_lib")
add_library( ${PACK_LIB} ${ALL_SOURCE_FILES})

add_executable( ${ProjectId} ${ALL_SOURCE_FILES})

target_link_libraries(${ProjectId} ${PACK_LIB})

enable_testing()

target_link_libraries(${ProjectId} gtest gtest_main)

add_test(NAME ${ProjectId} COMMAND ${ProjectId})

if( UNIX )
        foreach (SOURCE_FILE ${ALL_SOURCE_FILES})
            string(FIND ${SOURCE_FILE} ${PROJECT_BINARY_DIR} PROJECT_TRDPARTY_DIR_FOUND)
            if (NOT ${PROJECT_TRDPARTY_DIR_FOUND} EQUAL -1)
                list(REMOVE_ITEM ALL_SOURCE_FILES ${SOURCE_FILE})
            endif ()
        endforeach ()

        find_program(CPPCHECK cppcheck)
        add_custom_target(
                "${ProjectId}_cppcheck" ALL
                COMMAND ${CPPCHECK}
                --enable=warning,performance,portability,information,missingInclude
                --language=c++
                --std=c++11
                --error-exitcode=1
                --template="[{severity}][{id}] {message} {callstack} \(On {file}:{line}\)"
                --verbose
                --quiet
                ${ALL_SOURCE_FILES}
        )
    endif( UNIX )

SET(ARGS_FOR_CHECK_COUNT_TESTS "")
foreach (FILE_ELEM ${ALL_SOURCE_FILES})
    set(ARGS_FOR_CHECK_COUNT_TESTS "${ARGS_FOR_CHECK_COUNT_TESTS} ${FILE_ELEM}")
endforeach ()
