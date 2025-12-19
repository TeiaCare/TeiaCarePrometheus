set(TARGET_NAME teiacare_prometheus_client)
add_library(${TARGET_NAME})
add_library(teiacare::prometheus_client ALIAS ${TARGET_NAME})

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/version.cpp.in
    ${CMAKE_CURRENT_SOURCE_DIR}/src/version.cpp
)

set(TARGET_HEADERS
    include/teiacare/prometheus_client/base_metric_family.hpp
    include/teiacare/prometheus_client/base_metric_serializer.hpp
    include/teiacare/prometheus_client/base_metric.hpp
    include/teiacare/prometheus_client/counter.hpp
    include/teiacare/prometheus_client/gauge.hpp
    include/teiacare/prometheus_client/histogram.hpp
    include/teiacare/prometheus_client/labels.hpp
    include/teiacare/prometheus_client/metric_family.hpp
    include/teiacare/prometheus_client/metric_serializer.hpp
    include/teiacare/prometheus_client/metric_type.hpp
    include/teiacare/prometheus_client/registry.hpp
    include/teiacare/prometheus_client/summary.hpp
    include/teiacare/prometheus_client/version.hpp
)

set(TARGET_SOURCES
    src/base_metric.cpp
    src/counter.cpp
    src/gauge.cpp
    src/histogram.cpp
    src/labels.cpp
    src/metric_serializer.cpp
    src/registry.cpp
    src/summary.cpp
    src/version.cpp
)

target_compile_features(${TARGET_NAME} PUBLIC cxx_std_20)
target_sources(${TARGET_NAME} PUBLIC ${TARGET_HEADERS} PRIVATE ${TARGET_SOURCES})
target_include_directories(${TARGET_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:CMAKE_INSTALL_INCLUDEDIR>
)
set_target_properties(${TARGET_NAME} PROPERTIES VERSION ${${PROJECT_NAME}_VERSION} SOVERSION ${${PROJECT_NAME}_VERSION_MAJOR})
install(TARGETS ${TARGET_NAME})
install(DIRECTORY include DESTINATION .)

if(TC_ENABLE_WARNINGS_ERROR)
    add_warnings(${TARGET_NAME})
    add_warnings_as_errors(${TARGET_NAME})
endif()

#################################################################
# Unit Tests
if(TC_ENABLE_UNIT_TESTS)
	set(TARGET_NAME_UNIT_TEST ${TARGET_NAME}_unit_tests)
    setup_unit_tests(${TARGET_NAME_UNIT_TEST}
        tests/main.cpp
        tests/test_labels.cpp
        tests/test_counter.cpp
        tests/test_gauge.cpp
        tests/test_histogram.cpp
        tests/test_summary.cpp
        tests/test_registry.cpp
        tests/test_metric_serializer.cpp
    )

    if(TC_ENABLE_WARNINGS_ERROR)
        add_warnings(${TARGET_NAME_UNIT_TEST})
        add_warnings_as_errors(${TARGET_NAME_UNIT_TEST})
    endif()

    if(TC_ENABLE_UNIT_TESTS_COVERAGE)
        include(coverage)
        add_coverage(${TARGET_NAME})
    endif()
endif()

#################################################################
# Examples
if(TC_ENABLE_EXAMPLES)
    add_example(example_counter ${TARGET_NAME})
endif()
