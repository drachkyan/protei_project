cmake_minimum_required(VERSION 3.14)

include(FetchContent)

project(myApp)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)

FetchContent_Declare(spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.16.0)
FetchContent_MakeAvailable(spdlog)

#FetchContent_Declare(googletest
#        GIT_REPOSITORY https://github.com/google/googletest.git
#        GIT_TAG v1.16.0)
#FetchContent_MakeAvailable(googletest)

FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3)
FetchContent_MakeAvailable(json)

find_package(PkgConfig REQUIRED)
pkg_check_modules(URING REQUIRED liburing)

set(MY_VECTOR
        myvector/src/MyVector.cpp
        myvector/src/VectorFactory.cpp
)

# ===============
set(CLIENT_SOURCES
        client/src/Network/AppSettings.cpp
        client/src/Model/Menu.cpp
        client/src/Test/ConnectionTest.cpp
        client/src/Test/ResourceTest.cpp
        client/src/utils/utils.cpp
        client/src/Network/NetworkAddress.cpp
        client/src/Network/NetworkClient.cpp
        client/src/Model/MenuItem.cpp
        client/include/Model/MenuItem.h
        client/include/Model/MenuItems/AddVectorItem.h
        client/include/Model/MenuItems/PrintVectorItem.h
        client/include/Model/MenuItems/QuitItem.h
        client/include/Model/MenuItems/SendVectorItem.h
)

# ================
set(SERVER_SOURCES
        server/src/network/Server.cpp
        server/src/utils/utils.cpp
        server/src/model/ServerHandlers.cpp
)

# =================
add_executable(myApp_client client/src/client_main.cpp ${CLIENT_SOURCES} ${MY_VECTOR})
target_include_directories(myApp_client PRIVATE client/include myvector/include)
target_link_libraries(myApp_client PRIVATE spdlog::spdlog)
target_link_libraries(myApp_client PRIVATE nlohmann_json::nlohmann_json)

# =============
add_executable(myApp_server server/src/server_main.cpp ${SERVER_SOURCES} ${MY_VECTOR})
#target_compile_options(myApp_server PRIVATE -fsanitize=address -g)
#target_link_options(myApp_server PRIVATE -fsanitize=address)
target_include_directories(myApp_server PRIVATE server/include myvector/include)
target_link_libraries(myApp_server PRIVATE spdlog::spdlog ${URING_LIBRARIES})
target_include_directories(myApp_server PRIVATE ${URING_INCLUDE_DIRS})
target_link_libraries(myApp_server PRIVATE nlohmann_json::nlohmann_json)

# =================
