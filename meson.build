project(
    'hexagon-city',
     'cpp',
     version: '0.1',
     default_options: [
         'warning_level=3',
         'cpp_std=c++20'
     ])

deps = [
    dependency('penrose'),
]

src = [
    'src/DebugCameraSystem.cpp',
    'src/GameUISystem.cpp',
    'src/GridDrawableProvider.cpp',
    'src/GridGenerationSystem.cpp',
    'src/Raycaster.cpp',
    'src/SelectionSystem.cpp',
    'src/Main.cpp',
]

executable('hexagon_city', src, dependencies: deps)
