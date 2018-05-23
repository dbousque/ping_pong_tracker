from distutils.core import setup, Extension

# define the extension module
sources = ['list.c', 'ping_pong_wrapper.c', 'ping_pong.c']
include_dirs = ['.', '/usr/local/lib/python3.6/site-packages/numpy/core/include']
ping_pong = Extension('ping_pong', sources=sources, include_dirs=include_dirs)

# run the setup
setup(ext_modules=[ping_pong])
