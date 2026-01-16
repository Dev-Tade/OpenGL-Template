# OPENGL TEMPLATE
An OpenGL 3.3 Core Profile project template using GLAD, GLFW 3.3.8, stb_image and raymath (raylib math module) under mingw64

[GLFW Download](https://www.glfw.org/download.html)

[GLAD Download](https://glad.dav1d.de/)

[stb GitHub repo](https://github.com/nothings/stb/)

[raylib Homepage](https://www.raylib.com)

## Build System
Currently using a handmade makefile that can be configured at `Config.mk`, root
makefile builds all the `USER_SRC` code by globbing it.

Thirdparty dependencies under `THIRDPARTY_DIR` are built once,
and don't clean by default on `make clean`. If you wish to clean
it anyways use `make clean CLEAN_THIRDPARTY=yes`

Even though the template is designed to work on Windows with the following tooling:
* GNU/Makefile
* GNU/Compiler Collection (GCC)

__A unix shell is required__, for example Makefile defaults to
`bash` if `git` is installed.
So if your system default shell is `CMD` or `Powershell` when running
`make` it mostly wouldn't build.

At some point support for other build systems like CMake is planned, but makefile would
always be available.

### OLDER BRANCHES
All effort has been moved into mantaining `master` branch,
older branches are still avilable under `archive/` but don't
receive new updates.