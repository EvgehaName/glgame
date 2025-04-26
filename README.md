# How to build and setup 

## Build:

You can use any IDE you like because our project is configured to use CMake.
However, we tested building the project in Qt Creator, Visual Studio 2022 with a plugin installed to support Qt and Visual Code on Linux. We have not tested other IDEs.

## Libraries:
All required libraries are already in repository as plain files or as submodules.

## Setup:

- Clone the repository (you should not download it as an archive)
   - If you are using Git console, here's the command:
    - `git clone (url) --recursive`
   - If you are using Git client:
    - You can just push `Open in Desktop` if you are using Github Desktop.
    - Then type this command in the Git console `git submodule update --init --recursive`
    - If this command fails for any reason, use `git submodule sync` and then retry the above command.
    - Then type this command in the console 'cd vcpkg; .\bootstrap-vcpkg.bat'.
- Now you can build the engine!