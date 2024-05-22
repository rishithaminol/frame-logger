include_directories(../)

add_executable(timestamping timestamping.c ../timeutils.c)
add_executable(packet_chain_count packet_chain_count.c
                ../packet_chain.c
                ../logging.c
                ../processor.c
                ../timeutils.c
                ../ethertype.c)

target_link_libraries(packet_chain_count json-c pcap pthread)

add_test(NAME timestamping_test COMMAND timestamping)
add_test(NAME Packet_Count_Test_ssh.pcap
         COMMAND packet_chain_count ./captures/ssh.pcap 42
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_test(NAME Packet_Count_Test_random_capture_microsec.pcap
         COMMAND packet_chain_count ./captures/random_capture_microsec.pcap 475
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
