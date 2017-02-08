cmake_minimum_required(VERSION 2.8) # Проверка версии CMake.
									# Если версия установленой программы
									# старее указаной, произойдёт аварийный выход.
project(winapi_gdi)
ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)

include_directories(.)

list(APPEND sources
		./winapi_gdi.cpp
		./FACTORY.cpp
		./CURVE.cpp
		./PARABOLA.cpp
		./GYPERBOLA.cpp)

add_executable(winapi_gdi WIN32 ${sources})		# Создает исполняемый файл с именем <name>
													# из исходника <name>.cpp