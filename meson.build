project('raytracer', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3',
                     'cpp_std=c++23'])


includes = include_directories('include')

cfg_data = configuration_data()
cfg_data.set('version', meson.project_version())

sources = [
    'src/raytracer.cpp',
    'src/colors.cpp'
]

conf = configure_file(input: 'include/config.h.in', output: 'config.h', configuration: cfg_data)

c_args = ['-Wpedantic']
executable(
    'raytracer',
    sources,
    conf,
    include_directories: includes,
    c_args: c_args,
)
