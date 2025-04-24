include_directories(../)

add_executable(timestamping timestamping.c ../timeutils.c)
add_executable(packet_chain_count packet_chain_count.c
                ../packet_chain.c
                ../logging.c
                ../processor.c
                ../timeutils.c
                ../ethertype.c)
add_executable(packet_chain_speed packet_chain_speed.c
                ../packet_chain.c
                ../logging.c
                ../processor.c
                ../timeutils.c
                ../ethertype.c)

target_link_libraries(packet_chain_count json-c pcap pthread)
target_link_libraries(packet_chain_speed json-c pcap pthread)

configure_file(packet_chain_speed.sh.in packet_chain_speed.sh @ONLY)
configure_file(memleak_test.sh.in memleak_test.sh @ONLY)

add_test(NAME timespec_iso8601_test COMMAND timestamping)

# Check packet_chain_t can accurately calculate the number of packets
add_test(NAME Packet_Count_Test_ssh.pcap
         COMMAND ${CMAKE_CURRENT_BINARY_DIR}/packet_chain_count ./captures/ssh.pcap 42
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_test(NAME Packet_Count_Test_random_capture_microsec.pcap
         COMMAND ${CMAKE_CURRENT_BINARY_DIR}/packet_chain_count ./captures/random_capture_microsec.pcap 475
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_test(NAME packet_chain_speed_ssh.pcap
         COMMAND ${CMAKE_CURRENT_BINARY_DIR}/packet_chain_speed.sh captures/ssh.pcap 42)
add_test(NAME packet_chain_speed_random_capture_microsec.pcap
         COMMAND ${CMAKE_CURRENT_BINARY_DIR}/packet_chain_speed.sh captures/random_capture_microsec.pcap 475)

# Check memory safety
add_test(NAME packet_chain_speed_memsafe_ssh.pcap
         COMMAND ${CMAKE_CURRENT_BINARY_DIR}/memleak_test.sh captures/ssh.pcap 42)
add_test(NAME packet_chain_speed_memsafe_random_capture_microsec.pcap
         COMMAND ${CMAKE_CURRENT_BINARY_DIR}/memleak_test.sh captures/random_capture_microsec.pcap 475)
