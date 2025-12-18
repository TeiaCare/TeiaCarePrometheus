function(add_example EXAMPLE_TARGET TARGET_NAME)
	# find_package(spdlog REQUIRED)
	add_executable(${EXAMPLE_TARGET} examples/${EXAMPLE_TARGET}.cpp)
	target_link_libraries(${EXAMPLE_TARGET} PRIVATE ${TARGET_NAME}) #PRIVATE spdlog::spdlog
	target_compile_features(${EXAMPLE_TARGET} PRIVATE cxx_std_20)
    install(TARGETS ${EXAMPLE_TARGET} DESTINATION examples)
endfunction()
