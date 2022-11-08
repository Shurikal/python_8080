from setuptools import Extension, setup

setup(
    ext_modules=[
        Extension(
            name="_i8080",  # as it would be imported
                               # may include packages/namespaces separated by `.`

            sources=["src/i8080/_i8080.c"], # all sources are compiled into a single binary file
            include_dirs=["src/i8080"], # include directories
        ),
    ]
)
