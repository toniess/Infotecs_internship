cmake_minimum_required(VERSION 3.22.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Internship)

# Перечислите имена директорий с проектами
set(PROJECTS
    Program_1
    Program_2
)

# Собираем каждый проект
foreach(PROJECT ${PROJECTS})
    add_subdirectory(${PROJECT})
endforeach()

# Создаем цель для запуска обоих программ
add_custom_target(RunAllPrograms
    COMMAND $<TARGET_FILE:Program_1>
    COMMAND $<TARGET_FILE:Program_2>
    DEPENDS ${PROJECTS}
)

