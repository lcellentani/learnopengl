add_executable(02-textures
    main.cpp
)

set_target_properties(02-textures
    PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/media"
)

SetupSample(02-textures)

Enable_Cpp11(02-textures)
AddCompilerFlags(02-textures)

SetLinkerSubsystem(02-textures)