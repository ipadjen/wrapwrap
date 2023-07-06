cmake_minimum_required(VERSION 3.1)

project(wrapwrap)

set(CMAKE_CXX_FLAGS "-O2")
set(CMAKE_BUILD_TYPE "Release")

# Boost
if (WIN32)
    FIND_PACKAGE(Boost)
    if (Boost_FOUND)
        INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
        ADD_DEFINITIONS( "-DHAS_BOOST" )
        set(Boost_USE_STATIC_LIBS   ON)
        set(Boost_ARCHITECTURE "-x64")
    endif()
    ADD_DEFINITIONS("/EHsc")
endif (WIN32)

# CGAL
if (WIN32)
    include_directories(${CMAKE_SOURCE_DIR}/thirdparty/CGAL/include)
    set(CGAL_USE_INCLUDED_HEADERS ON)
else()
    find_package(CGAL REQUIRED QUIET COMPONENTS)
    if (CGAL_FOUND)
        if (CGAL_VERSION VERSION_GREATER_EQUAL "5.0")
            message(STATUS "Found CGAL version equal or greater than 5.0.")
            include_directories(${CMAKE_SOURCE_DIR}/thirdparty/CGAL/include)
            set(CGAL_USE_INCLUDED_HEADERS ON)
        else()
            message(FATAL_ERROR "Found CGAL version ${CGAL_VERSION} which is not supported!"
                "Please use CGAL version 5")
            return()
        endif ()
    endif ()
endif ()

# Creating entries for target: wrapwrap
FILE(GLOB SRC_FILES "*.cpp")
add_executable(wrapwrap ${SRC_FILES})
set_target_properties(
        wrapwrap
        PROPERTIES CXX_STANDARD 17
)

target_link_libraries(wrapwrap
        ${CGAL_LIBRARIES}
        ${CGAL_3RD_PARTY_LIBRARIES}
        )

install(TARGETS wrapwrap DESTINATION bin)
