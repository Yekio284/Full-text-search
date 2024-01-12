set(target_name app)

add_executable(${target_name})

include(CompileOptions)
set_compile_options(${target_name})

target_sources(
    ${target_name}
    PRIVATE
    main_searcher/main.cpp
)

set(target_name src)

add_library(${target_name} STATIC
    searcher/searcher.cpp
    searcher/searcher.hpp
    indexer/indexer.cpp
    indexer/indexer.hpp
    parser/parser.cpp
    parser/parser.hpp
)

target_link_libraries(
    app
    PRIVATE
    src
    CLI11
    rapidcsv
    pugixml
)

set_compile_options(${target_name})

target_include_directories(
    ${target_name}
    PUBLIC
    searcher
    indexer
    parser
    ${CMAKE_CURRENT_LIST_DIR}
)