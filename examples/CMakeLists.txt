add_executable(wincpp_test)
set_target_properties(wincpp_test PROPERTIES OUTPUT_NAME "test")
target_sources(wincpp_test PRIVATE "test.cpp" "threads.cpp")
target_link_libraries(wincpp_test PRIVATE wincpp)

add_executable(wincpp_threads)
set_target_properties(wincpp_threads PROPERTIES OUTPUT_NAME "threads")
target_sources(wincpp_threads PRIVATE "threads.cpp")
target_link_libraries(wincpp_threads PRIVATE wincpp)