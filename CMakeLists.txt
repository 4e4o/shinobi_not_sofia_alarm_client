cmake_minimum_required(VERSION 3.5)

project(shinobi_not_sofia_alarm_client LANGUAGES CXX)

add_subdirectory(boost_base)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME})

file(GLOB_RECURSE PRJ_SRCS src/*.c src/*.cpp src/*.h src/*.hpp)

target_sources(${PROJECT_NAME} PRIVATE ${PRJ_SRCS})
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC src/)
target_link_libraries(${PROJECT_NAME} boost_base)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)
install(FILES "scripts/shinobi_not_sofia_alarm_client" DESTINATION /etc/systemd/system/)
