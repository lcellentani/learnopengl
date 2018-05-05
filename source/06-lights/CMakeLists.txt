add_executable(06-lights
    main.cpp
)

set_target_properties(06-lights
    PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/media"
)

SetupSample(06-lights)

Enable_Cpp11(06-lights)
AddCompilerFlags(06-lights)

SetLinkerSubsystem(06-lights)