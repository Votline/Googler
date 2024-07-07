cmake_minimum_required(VERSION 3.10)
project(Googler)

# Найти пакет FLTK
find_package(FLTK REQUIRED)

# Добавить исполняемый файл
add_executable(Googler main.cpp)

# Включить заголовочные файлы FLTK
target_include_directories(Googler PRIVATE ${FLTK_INCLUDE_DIRS})

# Линковка с библиотеками FLTK
target_link_libraries(Googler PRIVATE ${FLTK_LIBRARIES})

# Установка кросс-компилятора для Windows (MinGW)
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_C_COMPILER i686-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER i686-w64-mingw32-g++)

# Указать кросс-платформенные флаги компиляции
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mwindows")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows")

# Указать путь к заголовочным файлам FLTK
include_directories(/usr/include/FL)

# Указать путь к библиотекам FLTK
link_directories(/usr/lib)
