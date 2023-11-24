project(
    'hexagon-city',
    'cpp',
    version : '0.1',
    default_options : [
        'warning_level=3',
        'cpp_std=c++20',
        'default_library=static'
    ])

deps = [
    dependency('penrose'),
]

src = [
    'src/Main.cpp',

    # Rendering
    'src/Rendering/GridDrawableProvider.cpp',

    # Systems
    'src/Systems/CitySimulationSystem.cpp',
    'src/Systems/DebugCameraSystem.cpp',
    'src/Systems/GameUISystem.cpp',
    'src/Systems/GridBuildingsSystem.cpp',
    'src/Systems/GridGenerationSystem.cpp',
    'src/Systems/SelectionSystem.cpp',

    # Others
    'src/RandomGenerator.cpp',
    'src/Raycaster.cpp',
]

executable('hexagon_city', src, dependencies : deps)
