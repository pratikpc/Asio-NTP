project(Asio-NTP)

find_package(asio CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} STATIC)
add_library(pc::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# Add compile definitions to export
target_compile_definitions(${PROJECT_NAME}
PRIVATE
#	Boost Asio/Beast Coroutine TS support
	BOOST_ASIO_HAS_CO_AWAIT
	BOOST_ASIO_HAS_STD_COROUTINE)

target_include_directories(${PROJECT_NAME}
# A PUBLIC Header can be accessed outside
PUBLIC
$<INSTALL_INTERFACE:include>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
# A Private Header File cannot be accessed by
# Any packages that uses it
)

file(GLOB_RECURSE LIBRARY_CXX_SOURCES src/*.cxx)
file(GLOB_RECURSE LIBRARY_HXX_SOURCES include/*.hxx)

target_sources(${PROJECT_NAME}
PRIVATE
${LIBRARY_CXX_SOURCES}
${LIBRARY_HXX_SOURCES}
 )


if (MSVC)
target_compile_options(${PROJECT_NAME} PRIVATE /Zc:__cplusplus)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
target_compile_options(${PROJECT_NAME} PRIVATE -fcoroutines-ts)
endif()


# add_subdirectory(tests)

target_link_libraries(${PROJECT_NAME}
 PRIVATE
 asio asio::asio
)
