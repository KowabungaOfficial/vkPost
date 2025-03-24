subdir('shader')
subdir('reshade')

vkPost_src = [
    'buffer.cpp',
    'command_buffer.cpp',
    'config.cpp',
    'descriptor_set.cpp',
    'effect_cas.cpp',
    'effect.cpp',
    'effect_deband.cpp',
    'effect_dls.cpp',
    'effect_fxaa.cpp',
    'effect_lut.cpp',
    'effect_reshade.cpp',
    'effect_simple.cpp',
    'effect_smaa.cpp',
    'effect_transfer.cpp',
    'fake_swapchain.cpp',
    'format.cpp',
    'framebuffer.cpp',
    'graphics_pipeline.cpp',
    'image.cpp',
    'image_view.cpp',
    'keyboard_input.cpp',
    'logger.cpp',
    'logical_swapchain.cpp',
    'lut_cube.cpp',
    'memory.cpp',
    'post.cpp',
    'renderpass.cpp',
    'reshade_uniforms.cpp',
    'sampler.cpp',
    'shader.cpp',
    'stb_image.c',
    'stb_image_resize.c',
    'util.cpp',
    'vkdispatch.cpp',
]

x11_dep = dependency('x11')
wayland_dep = dependency('wayland-client', required: false)
xkbcommon_dep = dependency('xkbcommon', required: false)

conf_paths = configuration_data()
conf_paths.set_quoted('SYSCONFDIR', join_paths(get_option('prefix'), get_option('sysconfdir')))
conf_paths.set_quoted('DATADIR', join_paths(get_option('prefix'), get_option('datadir')))

configure_file(output: 'config_paths.hpp',
               configuration : conf_paths)

keyboard_input_x11_lib = static_library('keyboard_input_x11', 'keyboard_input_x11.cpp')

vkPost_deps = [x11_dep, reshade_dep]
vkPost_defines = []

if wayland_dep.found() and xkbcommon_dep.found()
    vkPost_src += ['keyboard_input_wayland.cpp']
    vkPost_deps += [wayland_dep, xkbcommon_dep]
    vkPost_defines += ['-DVKPOST_WAYLAND=1']
else
    vkPost_defines += ['-DVKPOST_WAYLAND=0']
endif

shared_library(meson.project_name().to_lower(), 
    vkPost_src, shader_include,
    link_with: [keyboard_input_x11_lib],
    include_directories : vkPost_include_path,
    dependencies : vkPost_deps,
    cpp_args : vkPost_defines,
    gnu_symbol_visibility: 'hidden',
    install : true,
    install_dir : lib_dir)
