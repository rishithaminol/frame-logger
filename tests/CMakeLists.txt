include_directories(../)

add_executable(timestamping timestamping.c ../timeutils.c)

# target_link_libraries(timestamping json-c)

add_test(NAME timestamping_test COMMAND timestamping)
