# Find GTest package
find_package(GTest REQUIRED)

# Add test executable
add_executable(${PROJECT_NAME}_tests main_test.cpp)

# Link with GTest
target_link_libraries(${PROJECT_NAME}_tests PRIVATE
    GTest::GTest
    GTest::Main
)

# Include directories
target_include_directories(${PROJECT_NAME}_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Add test
add_test(NAME ${PROJECT_NAME}_tests COMMAND ${PROJECT_NAME}_tests) 