add_executable(05-lightingmaps
    main.cpp
)

set_target_properties(05-lightingmaps
    PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/media"
)

SetupSample(05-lightingmaps)

Enable_Cpp11(05-lightingmaps)
AddCompilerFlags(05-lightingmaps)

SetLinkerSubsystem(05-lightingmaps)