project('raytracer', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3',
                     'cpp_std=c++23'])

glfw_dep = dependency('glfw3')
glew_dep = dependency('glew')

includes = include_directories('include')
renderer_includes = include_directories('renderer')

cfg_data = configuration_data()
cfg_data.set('version', meson.project_version())

sources = [
    'src/raytracer.cpp',
    'src/colors.cpp',
    'src/rays.cpp',
    'src/threadpool.cpp'
]

render_sources = [
    'renderer/renderer.cpp'
]

renderer_lib = library('renderer',
    render_sources,
    dependencies: [glfw_dep, glew_dep],
    include_directories: [renderer_includes,includes]
)


conf = configure_file(input: 'include/config.h.in', output: 'config.h', configuration: cfg_data)

c_args = ['-Wpedantic']
executable(
    'raytracer',
    sources,
    conf,
    link_with: renderer_lib,
    dependencies: [glfw_dep, glew_dep],
    include_directories: [renderer_includes,includes],
    c_args: c_args,
)
