cmake_minimum_required(VERSION 3.10)
project(mcts_oxo)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加所有源文件
set(SOURCES
    mcts_oxo.cpp
)

# 添加所有头文件
set(HEADERS
    IState.h
    MSALoopTimer.h
    OXOState.h
    TreeNodeT.h
    ofxMSAmcts.h
)

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# 如果项目依赖 openFrameworks，需要添加以下配置
# find_package(OpenFrameworks REQUIRED)
# target_link_libraries(${PROJECT_NAME} PRIVATE OpenFrameworks)