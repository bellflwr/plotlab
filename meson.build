project(
  'plotlab',
  'cpp',
  version : '0.1',
  meson_version : '>= 1.3.0',
  default_options : ['warning_level=3', 'cpp_std=c++17'],
)

src = [
  'src/directives.cpp',
  'src/main.cpp',
  'src/plotlist.cpp',
  'src/plotrender/plotrender.cpp',
  'src/plotrender/shapes.cpp',
  'src/editor/plot_editor.cpp',
  'src/editor/finder.cpp',
  'src/point.cpp',
  'src/pointlist.cpp',
  'src/project.cpp',

  'vendor/imgui-sfml/src/imgui-SFML.cpp'
]

sfml_dep = declare_dependency(dependencies: [
  dependency('sfml-system', required: true),
   dependency('sfml-window', required: true),
   dependency('sfml-graphics', required: true),]
)



imgui_proj = subproject('imgui-docking')
imgui_dep = imgui_proj.get_variable('imgui_dep')


dependencies = [
  dependency('nlohmann_json', required: true),
  dependency('gl', required: true),
  sfml_dep,
  imgui_dep,
]

incdir = include_directories('include', 'vendor/imgui-sfml/include')

exe = executable(
  'plotlab',
  src,
  install : true,
  dependencies : dependencies,
  include_directories : incdir
)

test('basic', exe)
