set(
  differ_src
    "${CMAKE_CURRENT_SOURCE_DIR}/DiffConsumer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/differ.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/DifferenceEngine.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/DiffLog.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/FunctionDiffConsumer.cpp"
)

add_executable(differ ${differ_src})

# Get llvm.
llvm_map_components_to_libnames(llvm_libs core irreader)
target_link_libraries(differ ${llvm_libs})
