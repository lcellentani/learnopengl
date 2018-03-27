add_executable(01-shaders
    main.cpp
)

set_target_properties(01-shaders
    PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/media"
)

SetupSample(01-shaders)

Enable_Cpp11(01-shaders)
AddCompilerFlags(01-shaders)

SetLinkerSubsystem(01-shaders)