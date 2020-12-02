file(GLOB HEADERS "include/eosio/jsonrpc_plugin/*.hpp")
add_library( jsonrpc_plugin
             jsonrpc_plugin.cpp
             jsonrpc.cpp
             ${HEADERS} )

target_link_libraries( jsonrpc_plugin appbase fc http_plugin )
target_include_directories( jsonrpc_plugin PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )
