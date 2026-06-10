cmake_minimum_required(VERSION 3.10)
project(MyGameProject)

set(CMAKE_CXX_STANDARD 17)

# 寻找系统中的 SFML 库
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

# 生成可执行文件，目标名称为 MyGame
add_executable(MyGame ${SRC_FILES}
    src/Map.cpp
    src/Main.cpp
    src/Player.cpp
    src/Game.cpp
    src/Entity.cpp
    src/Enemy.cpp
    src/World.cpp
    src/EnemyFactory.cpp
    src/FOVEngine.cpp
    src/FogManager.cpp)

# 为目标 MyGame 链接头文件搜索路径
target_include_directories(MyGame PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 为目标 MyGame 链接 SFML 的库文件
target_link_libraries(MyGame PRIVATE sfml-graphics sfml-window sfml-system)