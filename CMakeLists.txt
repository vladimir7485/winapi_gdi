cmake_minimum_required(VERSION 2.8) # Проверка версии CMake.
									# Если версия установленой программы
									# старее указаной, произайдёт аварийный выход.
project(winapi_gdi)
ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)
add_executable(winapi_gdi WIN32 Source.cpp)		# Создает исполняемый файл с именем main
									# из исходника main.cpp