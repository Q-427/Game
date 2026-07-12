cmake_minimum_required(VERSION 3.20)

if(WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE AND EXISTS "D:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake")
    set(CMAKE_TOOLCHAIN_FILE "D:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()

project(WindLeafRunner LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(WindLeafRunner
    App/main.cpp
    App/GameApplication.cpp
    Model/CollisionDetector.cpp
    Model/GameModel.cpp
    Model/Leaf.cpp
    Model/LeafManager.cpp
    Model/Player.cpp
    ViewModel/GameViewModel.cpp
    View/InputHandler.cpp
    View/GameRenderer.cpp
    View/HUDRenderer.cpp
)

find_package(SFML 3 CONFIG REQUIRED COMPONENTS Graphics Window System)

target_link_libraries(WindLeafRunner
    PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
)

if(MSVC)
    target_compile_options(WindLeafRunner PRIVATE /utf-8)
endif()
