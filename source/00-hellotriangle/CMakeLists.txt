add_executable(00-hellotriangle
    main.cpp
)

set_target_properties(00-hellotriangle
    PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/media"
)

SetupSample(00-hellotriangle)

Enable_Cpp11(00-hellotriangle)
AddCompilerFlags(00-hellotriangle)

SetLinkerSubsystem(00-hellotriangle)