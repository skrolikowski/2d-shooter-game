include(FetchContent)

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/releases/download/v1.15.2/googletest-1.15.2.tar.gz
)

# Keep gtest's runtime settings consistent with the rest of the build.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

add_executable(formation_test "FormationTest.cpp")
target_link_libraries(formation_test PRIVATE src GTest::gtest_main)
gtest_discover_tests(formation_test)