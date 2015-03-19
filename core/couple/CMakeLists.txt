
file(GLOB subs */CMakeLists.txt)
foreach(sub ${subs})
    get_filename_component(sub ${sub} DIRECTORY)
    get_filename_component(sub ${sub} NAME)
    add_subdirectory(${sub})
endforeach()
