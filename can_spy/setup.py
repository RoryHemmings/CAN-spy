from setuptools import setup, find_packages

setup(
    name="CAN-spy",
    version="0.1",
    packages=find_packages(),
    include_package_data=True,
    install_requires=[],
    entry_points={
        "console_scripts": [
            "can-spy=scripts.repl:main",
        ],
    },
    long_description=open("README.md").read(),
    long_description_content_type='text/markdown'
)
