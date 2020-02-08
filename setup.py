from distutils.core import setup, Extension

module1 = Extension('pyfastbti',
                    define_macros = [('MAJOR_VERSION', '1'),
                                     ('MINOR_VERSION', '0')],
                    include_dirs = ['C:/Python36-64/include'],
                    libraries = [],
                    library_dirs = ['C:/Python36-64/libs'],
                    sources = ['pyfastbti.c'])

setup (name = 'Python Fast BTI',
       version = '1.0',
       description = 'Functions for encoding/decoding BTI files written in C.',
       author = 'LagoLunatic',
       author_email = '',
       url = 'https://github.com/LagoLunatic',
       long_description = '''
Python module written in C for encoding/decoding BTI image files with better performance than native Python code.
''',
       ext_modules = [module1])
