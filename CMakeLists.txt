cmake_minimum_required(VERSION 3.10)

project(echo_cancellation_demo LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(prebuilt_webrtc ${CMAKE_CURRENT_SOURCE_DIR}/win_webrtc)

# include directories
include_directories(${prebuilt_webrtc}/include/webrtc-audio-processing-1)
include_directories(${prebuilt_webrtc}/include/)

# executable
link_directories(${prebuilt_webrtc}/lib)
add_executable(echo_cancellation_demo main.cc)
target_link_libraries(echo_cancellation_demo PRIVATE webrtc-audio-processing-1.lib)
target_compile_definitions(echo_cancellation_demo PRIVATE WEBRTC_WIN NOMINMAX RTC_ENABLE_WIN_WGC)
set_target_properties(echo_cancellation_demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
# copy a dll next to executalbe
add_custom_command(TARGET echo_cancellation_demo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${prebuilt_webrtc}/bin/webrtc-audio-processing-1-3.dll
        $<TARGET_FILE_DIR:echo_cancellation_demo>
)
