project(Plumber)

set(Plumber_SRC_LIST 
    ${PROJECT_SOURCE_DIR}/main.cpp
	${PROJECT_SOURCE_DIR}/Plumber.h
	${PROJECT_SOURCE_DIR}/Position.h
)

add_executable(Plumber ${Plumber_SRC_LIST})
