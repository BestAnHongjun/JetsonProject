cmake_minimum_required(VERSION 2.6)
project(project_modules)

# Global Config
set (GLOBAL_EXAMPLE "ON")

# Timer Module
set (TIMER_SWITCH   "ON")
set (TIMER_EXAMPLE  "ON")
if (${TIMER_SWITCH} STREQUAL "ON")
    add_subdirectory(timer)
endif()
