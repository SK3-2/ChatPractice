# 요구 CMake 최소 버전
CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

# 프로젝트 이름 및 버전
# PROJECT ("test_for_chat_CMAKELIST")
SET (CMAKE_PROJECT_NAME_OF_SERVER SERVER)
SET (CMAKE_PROJECT_NAME_OF_CLIENT CLIENT)
SET (PROJECT_VERSION_MAJOR 0)
SET (PROJECT_VERSION_MINOR 1)

# 빌드 형상(Configuration) 및 주절주절 Makefile 생성 여부
#SET (CMAKE_BUILD_TYPE Debug)
SET (CMAKE_VERBOSE_MAKEFILE true)

# 빌드 대상 바이너리 파일명 및 소스 파일 목록
SET (OUTPUT_ELF_SERVER
				"${CMAKE_PROJECT_NAME_OF_SERVER}-${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.out"
				)
SET (OUTPUT_ELF_CLIENT
				"${CMAKE_PROJECT_NAME_OF_CLIENT}-${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.out"
				)
SET ( SRC_FILES_SERVER
				ClientManager.cpp
				EventManager.cpp
				main.cpp
				CreateServer.cpp
				Server.h
				ClientSession.cpp
				Message.cpp
				)
SET ( SRC_FILES_CLIENT
				Client.c
				readline.c
				)

SET (CMAKE_SYSTEM_NAME Linux)
SET (CMAKE_SYSTEM_PROCESSOR arm)
# 공통 컴파일러
SET (CMAKE_C_COMPILER "arm-linux-gnueabi-gcc")
SET (CMAKE_CXX_COMPILER "arm-linux-gnueabi-g++")

#공통 헤더 파일 Include 디렉토리 (-I)
INCLUDE_DIRECTORIES(./)

#공통 컴파일 옵션, 링크 옵션
#ADD_COMPILE_OPTIONS(-g -Wall)
#SET( CMAKE_EXE_LINKER_FLAGS "-static -Wl,--gc-sections")

#공통 링크 라이브러리 (-l)
#LINK_LIBRARIES()

#공통 링크 라이브러리 디렉토리 (-L)
#LINK_DIRECTORIES ()

SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BUILD_TYPE})

ADD_EXECUTABLE(${OUTPUT_ELF_SERVER} ${SRC_FILES_SERVER})
ADD_EXECUTABLE(${OUTPUT_ELF_CLIENT} ${SRC_FILES_CLIENT})
			
