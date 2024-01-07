cmake_minimum_required(VERSION 3.26)
project(css432_tftp)

set(CMAKE_CXX_STANDARD 17)
enable_testing()

configure_file(.github/workflows/TestTftp.sh TestTftp.sh COPYONLY)
configure_file(.github/workflows/input.txt input.txt COPYONLY)
configure_file(.github/workflows/check_memory_leak.sh check_memory_leak.sh COPYONLY)

add_executable(tftp-client TftpClient.cpp)
add_executable(tftp-server TftpServer.cpp)
