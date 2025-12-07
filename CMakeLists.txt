cmake_minimum_required(VERSION 3.16)
project(qt_travel_crud VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
# Убрали AUTOUIC, так как нет UI файлов
# set(CMAKE_AUTOUIC ON)

# Поиск Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Поиск PostgreSQL
find_package(PostgreSQL REQUIRED)

# Включаем директории
include_directories(${PostgreSQL_INCLUDE_DIRS})

# Исходные файлы (только .cpp файлы)
set(SOURCES
    main.cpp
    mainwindow.cpp
    database.cpp
)

# Создаем исполняемый файл
add_executable(qt_travel_crud ${SOURCES})

# Подключаем библиотеки
target_link_libraries(qt_travel_crud
    Qt6::Core
    Qt6::Widgets
    ${PostgreSQL_LIBRARIES}
)