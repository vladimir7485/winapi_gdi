cmake_minimum_required(VERSION 2.8) # �������� ������ CMake.
									# ���� ������ ������������ ���������
									# ������ ��������, ��������� ��������� �����.
project(winapi_gdi)
ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)

include_directories(.)

list(APPEND sources
		./winapi_gdi.cpp
		./FACTORY.cpp
		./CURVE.cpp
		./PARABOLA.cpp
		./GYPERBOLA.cpp
		./SINUS.cpp
		./TANGENT.cpp)

add_executable(winapi_gdi WIN32 ${sources})		# ������� ����������� ���� � ������ <name>
													# �� ��������� <name>.cpp