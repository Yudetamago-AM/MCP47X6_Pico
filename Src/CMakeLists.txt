add_library(MCP47X6 INTERFACE)

target_sources(MCP47X6 INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/MCP47X6.cpp
)

target_link_libraries(MCP47X6
        INTERFACE
        pico_stdlib
        hardware_i2c
)