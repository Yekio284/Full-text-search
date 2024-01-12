set(target_name_test test_searcher)

add_executable(${target_name_test})

include(CompileOptions)
set_compile_options(${target_name_test})

target_sources(
    ${target_name_test}
    PRIVATE
    test.cpp
)

target_link_libraries(
    ${target_name_test}
    PRIVATE
    gtest
    src
    pugixml
)

add_test(${target_name_test} ${CMAKE_BINARY_DIR}/bin/${target_name_test})