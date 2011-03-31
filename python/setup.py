from setuptools import setup, Extension

_texcaller = Extension('_texcaller',
                       sources = ['texcaller_wrap.cxx'])

setup(name = 'texcaller',
      version = '0',
      ext_modules = [_texcaller],
      py_modules = ['texcaller'])
