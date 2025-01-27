cmake_minimum_required(VERSION 3.16)

# Название проекта
project(finalProject)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Подключение Google Test через FetchContent
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
FetchContent_MakeAvailable(googletest)

# Основной исполняемый файл
add_executable(finalProject
    main.cpp
    src/converter_json.cpp
    src/inverted_index.cpp
    src/search_server.cpp
)

# Указываем пути для nlohmann/json
target_include_directories(finalProject PRIVATE ${CMAKE_SOURCE_DIR}/external/nlohmann)

# Тесты для InvertedIndex
add_executable(test_inverted_index
    tests/test_main.cpp
    tests/test_inverted_index.cpp
    src/inverted_index.cpp
)
target_include_directories(test_inverted_index PRIVATE ${CMAKE_SOURCE_DIR}/external/nlohmann)
target_link_libraries(test_inverted_index gtest gtest_main)

# Тесты для SearchServer
add_executable(test_search_server
    tests/test_main.cpp
    tests/test_search_server.cpp
    src/search_server.cpp
    src/inverted_index.cpp
)
target_include_directories(test_search_server PRIVATE ${CMAKE_SOURCE_DIR}/external/nlohmann)
target_link_libraries(test_search_server gtest gtest_main)

# Копирование JSON-файлов в папку сборки
configure_file(${CMAKE_SOURCE_DIR}/config.json ${CMAKE_BINARY_DIR}/config.json COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/requests.json ${CMAKE_BINARY_DIR}/requests.json COPYONLY)