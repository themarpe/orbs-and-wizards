# Adapted to CMake build system 17/11/2022
cmake_minimum_required(VERSION 3.21)

set(TARGET_NAME OrbsAndWizards)
project(${TARGET_NAME} C CXX)

# Specify that shared libs aren't built by default
option(BUILD_SHARED_LIBS "Build dependencies as a shared libraries" OFF)

# Append cmake directory to module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/")

# Add external dependencies
add_subdirectory(external)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Installation Directory" FORCE)
endif()

add_executable(${TARGET_NAME} WIN32
    src/Engine/Camera.cpp
    src/Engine/Camera.h
    src/Engine/Enums.h
    src/Engine/Game.cpp
    src/Engine/Game.h
    src/Engine/GameEngine.cpp
    src/Engine/GameEngine.h
    src/Engine/Image.cpp
    src/Engine/Image.h
    src/Engine/Level.cpp
    src/Engine/Level.h
    src/Engine/LevelCreator.cpp
    src/Engine/LevelCreator.h
    src/Engine/SpriteSheet.cpp
    src/Engine/SpriteSheet.h
    src/Engine/States/GameOverState.cpp
    src/Engine/States/GameOverState.h
    src/Engine/States/GameState.cpp
    src/Engine/States/GameState.h
    src/Engine/States/GameWonState.cpp
    src/Engine/States/GameWonState.h
    src/Engine/States/LevelCreatorSelector.cpp
    src/Engine/States/LevelCreatorSelector.h
    src/Engine/States/LevelCreatorState.cpp
    src/Engine/States/LevelCreatorState.h
    src/Engine/States/MainMenuState.cpp
    src/Engine/States/MainMenuState.h
    src/Engine/States/PauseState.cpp
    src/Engine/States/PauseState.h
    src/Engine/States/State.h
    src/Engine/UI.cpp
    src/Engine/UI.h
    src/Engine/Vector2D.cpp
    src/Engine/Vector2D.h
    src/Objects/Block.cpp
    src/Objects/Block.h
    src/Objects/Door.cpp
    src/Objects/Door.h
    src/Objects/Entity.cpp
    src/Objects/Entity.h
    src/Objects/HittableEnemy.cpp
    src/Objects/HittableEnemy.h
    src/Objects/Orb.cpp
    src/Objects/Orb.h
    src/Objects/Player.cpp
    src/Objects/Player.h
    src/Objects/Potion.cpp
    src/Objects/Potion.h
    src/Objects/Rune.cpp
    src/Objects/Rune.h
    src/Objects/RuneKey.cpp
    src/Objects/RuneKey.h
    src/Objects/Skeleton.cpp
    src/Objects/Skeleton.h
    src/Objects/SparkleAnimation.cpp
    src/Objects/SparkleAnimation.h
    src/Objects/Turret.cpp
    src/Objects/Turret.h
    src/main.cpp
)

# Specify C++11 and disables extensions
set_property(TARGET ${TARGET_CORE_NAME} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${TARGET_CORE_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET ${TARGET_CORE_NAME} PROPERTY CXX_EXTENSIONS OFF)

# Add include headers
set(header_dirs src/Engine src/Engine/States src/Objects)
target_include_directories(${TARGET_NAME} PRIVATE ${header_dirs})

# Add clangformat
include(ClangFormat)
target_clangformat_setup(${TARGET_NAME} "${header_dirs}")

# Link to libraries
target_link_libraries(OrbsAndWizards PRIVATE SDL2-static SDL2_ttf SDL2_image SDL2main tinyfiledialogs rapidjson)

# Set rpath
set_target_properties(${TARGET_NAME} PROPERTIES INSTALL_RPATH "\${ORIGIN}")

# Copy over required DLLs (Windows)
if(WIN32 AND ${BUILD_SHARED_LIBS})
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND
        ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:${TARGET_NAME}> $<TARGET_FILE_DIR:${TARGET_NAME}>
        COMMAND_EXPAND_LISTS
    )
endif()

# Installs executable and dependencies (except system ones)
install(TARGETS ${TARGET_NAME}
    RUNTIME_DEPENDENCIES
        PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-" "ld-linux" "libz" "libc" "libpthread" "libgcc" "libm" "libstdc\\+\\+" "librt" "libdl"
        POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
    RUNTIME DESTINATION ${TARGET_NAME}
    LIBRARY DESTINATION ${TARGET_NAME}
)
# Installs resources and assets
install(DIRECTORY res Assets Levels CustomLevels Replays DESTINATION ${TARGET_NAME})