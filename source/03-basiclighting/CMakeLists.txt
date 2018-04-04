add_executable(03-basiclighting
    main.cpp
)

set_target_properties(03-basiclighting
    PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/media"
)

SetupSample(03-basiclighting)

Enable_Cpp11(03-basiclighting)
AddCompilerFlags(03-basiclighting)

SetLinkerSubsystem(03-basiclighting)