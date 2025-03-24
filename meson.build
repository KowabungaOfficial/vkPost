project('vkPost', ['c', 'cpp'],
    default_options: ['c_std=c11', 'cpp_std=c++2a'],
    version: '0.4.6',
    license: 'Zlib'
)

cpp = meson.get_compiler('cpp')

# Define include directories for vkPost (only 'src' for now, adjust as needed)
vkPost_include_path = include_directories('src')

lib_dir = get_option('libdir')
ld_lib_dir_vkpost = join_paths(get_option('prefix'), lib_dir)  # Default: e.g., /usr/lib

if get_option('append_libdir_vkpost')
    lib_dir = join_paths(lib_dir, 'vkpost')
    ld_lib_dir_vkpost = join_paths(ld_lib_dir_vkpost, 'vkpost')  # e.g., /usr/lib/vkpost
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
