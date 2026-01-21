# OPENGL TEMPLATE

An OpenGL 4.6 Core Profile (backwards compatible) project template using GLAD, GLFW 3.3.8, stb_image and raymath (raylib math module) under mingw64

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

To enable OpenGL debug messages you must run the make command as follows:
`make GL_DEBUG=enabled`, though if the `GLAD` header doesn't provide `GL_KHR_debug`
extension then it wouldn't work (a warning will be emitted).
If the extension is present but the driver doesn't support it then it wouldn't
neither (a warning will also be emitted).
The provided `GLAD` header and src files in the repo do have the
extension enabled.

Even though the template is designed to work on Windows with the following tooling:
* GNU/Makefile
* GNU/Compiler Collection (GCC)

__A unix shell is required__, for example Make defaults to
`bash` if `git` is installed.
So if your system default shell is `CMD` or `Powershell` when running
`make` it will mostly fail to build.

At some point support for other build systems like CMake is planned, but makefile would
always be available.

## Switching / Upgrading GLAD

If you have installed the `glad cli` (read more at [GLAD GitHub repo](https://github.com/Dav1dde/glad/tree/master)), 
by running the following command you could easily switch the provided OpenGL version
and profile to the one you desire and/or add extensions you may need.

```sh
glad --profile <core|compatibility> --out-path ./thirdparty/ --api "gl=<version>" --generator c --extensions "<extensions>"
```

For example:
```sh
glad --profile core --out-path ./thirdparty/ --api "gl=3.3" --generator c --extensions "GL_KHR_debug"
```
is the one used for generating the same configuration as provided in the repo.

__NOTE: you must provide `--extensions ""` if you don't want any extensions, 
if it's missing by default all available extensions would be enabled.__

### BRANCHES

- **master**
  Actively mantained. Targets OpenGL 4.6 Core Profile.

- **gl-3.3**
  Stable OpenGL 3.3 Core Profile version.
  May or may not receive new dependencies but not code
  refactors/fixes.

- **archive/xxx**
  Are the oldest branches of the template, they
  are still avilable but don't receive new updates.