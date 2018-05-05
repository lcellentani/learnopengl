add_executable(04-materials
    main.cpp
)

set_target_properties(04-materials
    PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/media"
)

SetupSample(04-materials)

Enable_Cpp11(04-materials)
AddCompilerFlags(04-materials)

SetLinkerSubsystem(04-materials)