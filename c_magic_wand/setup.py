from distutils.core import setup, Extension

# define the extension module
sources = ['list.c', 'ping_pong_wrapper.c', 'ping_pong.c']
ping_pong = Extension('ping_pong', sources=sources, include_dirs=["."])

# run the setup
setup(ext_modules=[ping_pong])
