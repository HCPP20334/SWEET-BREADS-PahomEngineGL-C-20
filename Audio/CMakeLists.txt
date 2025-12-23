cmake_minimum_required( VERSION 3.23.0 )

# Use solution folders in Visual Studio.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project( Audio LANGUAGES C CXX )

set( INC_FILES
    Audio/Config.hpp
    Audio/Device.hpp
    Audio/Listener.hpp
    Audio/Sound.hpp
    Audio/Vector.hpp
	Audio/Waveform.hpp
)

set( SRC_FILES
    src/Device.cpp
    src/Listener.cpp
    src/ListenerImpl.hpp
    src/ListenerImpl.cpp
    src/miniaudio.c
    src/miniaudio.h
    src/Sound.cpp
    src/SoundImpl.hpp
    src/SoundImpl.cpp
	src/Waveform.cpp
	src/WaveformImpl.hpp
	src/WaveformImpl.cpp
    src/stb_vorbis.c
)

set( ALL_FILES 
    ${SRC_FILES} 
    ${INC_FILES} 
)

add_library( Audio ${ALL_FILES})

set_target_properties( Audio
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)

target_include_directories( Audio
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

