project('vkPost', ['c', 'cpp'],
    default_options: ['c_std=c11', 'cpp_std=c++2a'],
    version: '0.4.6',
    license: 'Zlib'
)

cpp = meson.get_compiler('cpp')

# Define include directories for vkPost
vkPost_include_path = include_directories('src')

lib_dir = get_option('libdir')
ld_lib_dir_vkpost = join_paths(get_option('prefix'), lib_dir)  # Default: e.g., /usr/lib

if get_option('append_libdir_vkpost')
    lib_dir = join_paths(lib_dir, 'vkpost')
    ld_lib_dir_vkpost = join_paths(ld_lib_dir_vkpost, 'vkpost')  # e.g., /usr/lib/vkpost
endif

# Dependencies for Wayland and X11
dep_wayland_client = dependency('wayland-client', required: false)
dep_xkbcommon = dependency('xkbcommon', required: false)
dep_x11 = dependency('x11')

# Define compilation flags for Wayland support
if dep_wayland_client.found() and dep_xkbcommon.found()
    add_project_arguments('-DVKPOST_WAYLAND=1', language: 'cpp')
else
    add_project_arguments('-DVKPOST_WAYLAND=0', language: 'cpp')
endif

# Process subdirectories based on options
if get_option('with_so')
    subdir('src')
else
    if get_option('with_json')
        warning('with_json is enabled but with_so is disabled; JSON will be installed without a library.')
    endif
endif

if get_option('with_json')
    subdir('config')
endif
