from distutils.core import setup, Extension

extra_compile_args = []
# extra_compile_args.append('-Werror')

module1 = Extension('pyfastbti',
                    define_macros = [('MAJOR_VERSION', '1'),
                                     ('MINOR_VERSION', '0')],
                    include_dirs = [],
                    libraries = [],
                    library_dirs = [],
                    sources = ['pyfastbti.c'],
                    extra_compile_args = extra_compile_args)

setup (name = 'PyFastBTI',
       version = '1.0',
       description = 'Functions for encoding/decoding BTI files written in C.',
       author = 'LagoLunatic',
       author_email = '',
       url = 'https://github.com/LagoLunatic',
       long_description = '''
Python module written in C for encoding/decoding BTI image files with better performance than native Python code.
''',
       ext_modules = [module1])
