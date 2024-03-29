#**************************************************************************************************
#
#   raylib makefile for Desktop platforms, Raspberry Pi and Web Assembly
#
#   Copyright (c) 2013-2023 Ramon Santamaria (@raysan5)
#
#   This software is provided "as-is", without any express or implied warranty. In no event
#   will the authors be held liable for any damages arising from the use of this software.
#
#   Permission is granted to anyone to use this software for any purpose, including commercial
#   applications, and to alter it and redistribute it freely, subject to the following restrictions:
#
#     1. The origin of this software must not be misrepresented; you must not claim that you
#     wrote the original software. If you use this software in a product, an acknowledgment
#     in the product documentation would be appreciated but is not required.
#
#     2. Altered source versions must be plainly marked as such, and must not be misrepresented
#     as being the original software.
#$(RAYLIB_LIB_PATH)/libraylib.a
#     3. This notice may not be removed or altered from any source distribution.
#
#**************************************************************************************************

.PHONY: all clean debug

# Define required environment variables
#------------------------------------------------------------------------------------------------
# Define target platform: PLATFORM_DESKTOP, PLATFORM_WEB, PLATFORM_DRM
PLATFORM              ?= PLATFORM_DESKTOP

# Define project variables
PROJECT_NAME          ?= astro-aegis
PROJECT_VERSION       ?= 1.0
PROJECT_BUILD_PATH    ?= .

RAYLIB_PATH           ?= ../raylib
RAYLIB_INCLUDE_PATH   ?= $(RAYLIB_PATH)/src
RAYLIB_LIB_PATH       ?= $(RAYLIB_PATH)/src

# Build mode for project: DEBUG or RELEASE
BUILD_MODE            ?= DEBUG

# PLATFORM_WEB: Default properties
BUILD_WEB_ASYNCIFY    ?= FALSE
BUILD_WEB_SHELL       ?= minshell.html
BUILD_WEB_HEAP_SIZE   ?= 400MB # large, but trying to stay under 512
BUILD_WEB_RESOURCES   ?= TRUE
BUILD_WEB_RESOURCES_PATH  ?= resources

# Determine PLATFORM_OS in case PLATFORM_DESKTOP selected
ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    # No uname.exe on MinGW!, but OS=Windows_NT on Windows!
    # ifeq ($(UNAME),Msys) -> Windows
    ifeq ($(OS),Windows_NT)
        PLATFORM_OS = WINDOWS
    else
        UNAMEOS = $(shell uname)
        ifeq ($(UNAMEOS),Linux)
            PLATFORM_OS = LINUX
        endif
        ifeq ($(UNAMEOS),FreeBSD)
            PLATFORM_OS = BSD
        endif
        ifeq ($(UNAMEOS),OpenBSD)
            PLATFORM_OS = BSD
        endif
        ifeq ($(UNAMEOS),NetBSD)
            PLATFORM_OS = BSD
        endif
        ifeq ($(UNAMEOS),DragonFly)
            PLATFORM_OS = BSD
        endif
        ifeq ($(UNAMEOS),Darwin)
            PLATFORM_OS = OSX
        endif
    endif
endif
ifeq ($(PLATFORM),PLATFORM_DRM)
    UNAMEOS = $(shell uname)
    ifeq ($(UNAMEOS),Linux)
        PLATFORM_OS = LINUX
    endif
endif

ifeq ($(PLATFORM),PLATFORM_WEB)
    # Emscripten required variables
    EMSDK_PATH         ?= C:/emsdk
    EMSCRIPTEN_PATH    ?= $(EMSDK_PATH)/upstream/emscripten
    CLANG_PATH          = $(EMSDK_PATH)/upstream/bin
    PYTHON_PATH         = $(EMSDK_PATH)/python/3.9.2-1_64bit
    NODE_PATH           = $(EMSDK_PATH)/node/14.15.5_64bit/bin
    export PATH         = $(EMSDK_PATH):$(EMSCRIPTEN_PATH):$(CLANG_PATH):$(NODE_PATH):$(PYTHON_PATH):$(shell printenv PATH)
endif

# Define default C compiler: CC
#------------------------------------------------------------------------------------------------
CC = cc

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),OSX)
        # OSX default compiler
        CC = clang
    endif
    ifeq ($(PLATFORM_OS),BSD)
        # FreeBSD, OpenBSD, NetBSD, DragonFly default compiler
        CC = clang
    endif
endif
ifeq ($(PLATFORM),PLATFORM_WEB)
    # HTML5 emscripten compiler
    # WARNING: To compile to HTML5, code must be redesigned
    # to use emscripten.h and emscripten_set_main_loop()
    CC = emcc
endif
ifeq ($(PLATFORM),PLATFORM_DRM)
    ifeq ($(USE_RPI_CROSS_COMPILER),TRUE)
        # Define RPI cross-compiler
        #CC = armv6j-hardfloat-linux-gnueabi-gcc
        CC = $(RPI_TOOLCHAIN)/bin/arm-linux-gnueabihf-gcc
    endif
endif


# Define compiler flags: CFLAGS
#------------------------------------------------------------------------------------------------
#  -O1                  defines optimization level
#  -g                   include debug information on compilation
#  -s                   strip unnecessary data from build
#  -Wall                turns on most, but not all, compiler warnings
#  -std=c99             defines C language mode (standard C from 1999 revision)
#  -std=gnu99           defines C language mode (GNU C from 1999 revision)
#  -Wno-missing-braces  ignore invalid warning (GCC bug 53119)
#  -Wno-unused-value    ignore unused return values of some functions (i.e. fread())
#  -D_DEFAULT_SOURCE    use with -std=c99 on Linux and PLATFORM_WEB, required for timespec
CFLAGS = -Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces -Wunused-result $(CUSTOM_CFLAGS)

ifeq ($(BUILD_MODE),DEBUG)
    CFLAGS += -g -D_DEBUG
    ## asan
    CFLAGS += -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment
else
    ifeq ($(PLATFORM),PLATFORM_WEB)
        ifeq ($(BUILD_WEB_ASYNCIFY),TRUE)
            CFLAGS += -O3
        else
            CFLAGS += -Os
        endif
    else
        ifeq ($(PLATFORM_OS),OSX)
            CFLAGS += -O2
        else
            CFLAGS += -s -O2
        endif
    endif
endif
ifeq ($(PLATFORM),PLATFORM_DRM)
    CFLAGS += -std=gnu99 -DEGL_NO_X11
endif

# Define include paths for required headers: INCLUDE_PATHS
#------------------------------------------------------------------------------------------------
INCLUDE_PATHS = -I. -I$(RAYLIB_PATH)/src -I$(RAYLIB_PATH)/src/external
INCLUDE_PATHS += -I./xxd_resources/
INCLUDE_PATHS += -I./xxd_resources/img/
INCLUDE_PATHS += -I./xxd_resources/mdl/
INCLUDE_PATHS += -I./xxd_resources/mus/
INCLUDE_PATHS += -I./xxd_resources/snd/
INCLUDE_PATHS += -I./xxd_resources/lic/

# Define additional directories containing required header files
ifeq ($(PLATFORM),PLATFORM_DRM)
    # DRM required libraries
    INCLUDE_PATHS += -I/usr/include/libdrm
endif
ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),BSD)
        # Consider -L$(RAYLIB_H_INSTALL_PATH)
        INCLUDE_PATHS += -I/usr/local/include
    endif
endif

# Define library paths containing required libs: LDFLAGS
#------------------------------------------------------------------------------------------------
LDFLAGS = -L.

ifeq ($(PLATFORM),PLATFORM_WEB)
	LDFLAGS += -L$(RAYLIB_LIB_PATH)
endif

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),WINDOWS)
        # resource file contains windows executable icon and properties
        LDFLAGS += $(RAYLIB_PATH)/src/raylib.rc.data
        # -Wl,--subsystem,windows hides the console window
        ifeq ($(BUILD_MODE), RELEASE)
            LDFLAGS += -Wl,--subsystem,windows
        endif
    endif
    ifeq ($(PLATFORM_OS),BSD)
        # Consider -L$(RAYLIB_INSTALL_PATH)
        LDFLAGS += -Lsrc -L/usr/local/lib
    endif
    ifeq ($(PLATFORM_OS),LINUX)
        # Reset everything.
        # Precedence: immediately local, installed version, raysan5 provided libs
        #LDFLAGS += -L$(RAYLIB_RELEASE_PATH)
    endif
endif
ifeq ($(PLATFORM),PLATFORM_WEB)
    # -Os                        # size optimization
    # -O2                        # optimization level 2, if used, also set --memory-init-file 0
    # -s USE_GLFW=3              # Use glfw3 library (context/input management)
    # -s ALLOW_MEMORY_GROWTH=1   # to allow memory resizing -> WARNING: Audio buffers could FAIL!
    # -s TOTAL_MEMORY=16777216   # to specify heap memory size (default = 16MB) (67108864 = 64MB)
    # -s USE_PTHREADS=1          # multithreading support
    # -s WASM=0                  # disable Web Assembly, emitted by default
    # -s ASYNCIFY                # lets synchronous C/C++ code interact with asynchronous JS
    # -s FORCE_FILESYSTEM=1      # force filesystem to load/save files data
    # -s ASSERTIONS=1            # enable runtime checks for common memory allocation errors (-O1 and above turn it off)
    # --profiling                # include information for code profiling
    # --memory-init-file 0       # to avoid an external memory initialization code file (.mem)
    # --preload-file resources   # specify a resources folder for data compilation
    # --source-map-base          # allow debugging in browser with source map
    LDFLAGS += -s USE_GLFW=3 -s TOTAL_MEMORY=$(BUILD_WEB_HEAP_SIZE) -s FORCE_FILESYSTEM=1 -sALLOW_MEMORY_GROWTH
    
    # Build using asyncify
    ifeq ($(BUILD_WEB_ASYNCIFY),TRUE)
        LDFLAGS += -s ASYNCIFY
    endif
    
    # Add resources building if required
    ifeq ($(BUILD_WEB_RESOURCES),TRUE)
        LDFLAGS += --preload-file $(BUILD_WEB_RESOURCES_PATH)
    endif
    
    # Add debug mode flags if required
    ifeq ($(BUILD_MODE),DEBUG)
        LDFLAGS += -s ASSERTIONS=1 --profiling
    endif

    # Define a custom shell .html and output extension
    LDFLAGS += --shell-file $(BUILD_WEB_SHELL)
    EXT = .html
endif

# Define libraries required on linking: LDLIBS
# NOTE: To link libraries (lib<name>.so or lib<name>.a), use -l<name>
#------------------------------------------------------------------------------------------------
ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),WINDOWS)
        # Libraries for Windows desktop compilation
        # NOTE: WinMM library required to set high-res timer resolution
        LDLIBS = -lraylib_$(PLATFORM) -lopengl32 -lgdi32 -lwinmm -lcomdlg32 -lole32
        # Required for physac examples
        LDLIBS += -static -lpthread
    endif
    ifeq ($(PLATFORM_OS),LINUX)
        # Libraries for Debian GNU/Linux desktop compiling
        # NOTE: Required packages: libegl1-mesa-dev
        LDLIBS = -lraylib_$(PLATFORM) -lGL -lm -lpthread -ldl -lrt

        # On Wayland windowing system, additional libraries requires
        ifeq ($(USE_WAYLAND_DISPLAY),TRUE)
            LDLIBS += -lwayland-client -lwayland-cursor -lwayland-egl -lxkbcommon
        else
            # On X11 requires also below libraries
            LDLIBS += -lX11
            # NOTE: It seems additional libraries are not required any more, latest GLFW just dlopen them
            #LDLIBS += -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor
        endif
        # Explicit link to libc
        ifeq ($(RAYLIB_LIBTYPE),SHARED)
            LDLIBS += -lc
        endif
    endif
    ifeq ($(PLATFORM_OS),OSX)
        # Libraries for OSX 10.9 desktop compiling
        # NOTE: Required packages: libopenal-dev libegl1-mesa-dev
        LDLIBS = -lraylib_$(PLATFORM) -framework OpenGL -framework Cocoa -framework IOKit -framework CoreAudio -framework CoreVideo
    endif
    ifeq ($(PLATFORM_OS),BSD)
        # Libraries for FreeBSD, OpenBSD, NetBSD, DragonFly desktop compiling
        # NOTE: Required packages: mesa-libs
        LDLIBS = -lraylib_$(PLATFORM) -lGL -lpthread -lm

        # On XWindow requires also below libraries
        LDLIBS += -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor
    endif
endif
ifeq ($(PLATFORM),PLATFORM_WEB)
    # Libraries for web (HTML5) compiling
    LDLIBS = -lraylib_$(PLATFORM)
endif
ifeq ($(PLATFORM),PLATFORM_DRM)
    # Libraries for DRM compiling
    # NOTE: Required packages: libasound2-dev (ALSA)
    LDLIBS = -lraylib_$(PLATFORM) -lGLESv2 -lEGL -lpthread -lrt -lm -lgbm -ldrm -ldl
endif


# Define source code files required
#------------------------------------------------------------------------------------------------
PROJECT_SOURCE_FILES  = main.c
PROJECT_SOURCE_FILES += xxd_resources/assets.c
PROJECT_SOURCE_FILES += resource.c
PROJECT_SOURCE_FILES += tool.c
PROJECT_SOURCE_FILES += game.c
PROJECT_SOURCE_FILES += entity_player.c
PROJECT_SOURCE_FILES += entity_globe.c
PROJECT_SOURCE_FILES += vector.c
PROJECT_SOURCE_FILES += entity_asteroid.c
PROJECT_SOURCE_FILES += level.c
PROJECT_SOURCE_FILES += weapon.c
PROJECT_SOURCE_FILES += entity_bullet.c
PROJECT_SOURCE_FILES += entity_banner.c

# Define all object files from source files
OBJS = $(patsubst %.c, %.o, $(PROJECT_SOURCE_FILES))

XXD_IMG = $(wildcard xxd_resources/img/*.png)
IMG_SRC = $(XXD_IMG:.png=.h)
XXD_MDL = $(wildcard xxd_resources/mdl/*.glb)
MDL_SRC = $(XXD_MDL:.glb=.h)
XXD_MUS = $(wildcard xxd_resources/mus/*.xm)
MUS_SRC = $(XXD_MUS:.xm=.h)
XXD_SND = $(wildcard xxd_resources/snd/*.ogg)
SND_SRC = $(XXD_SND:.ogg=.h)
XXD_LIC = $(wildcard xxd_resources/lic/*.txt)
LIC_SRC = $(XXD_LIC:.txt=.h)
XXD_RES = $(IMG_SRC) $(MDL_SRC) $(MUS_SRC) $(SND_SRC) $(LIC_SRC)

# Define processes to execute
#------------------------------------------------------------------------------------------------
# Default target entry
# NOTE: We call this Makefile target or Makefile.Android target
all: libraylib_$(PLATFORM).a $(PROJECT_NAME)

libraylib_$(PLATFORM).a:
	$(MAKE) PLATFORM=$(PLATFORM) -C ../raylib/src clean
	$(MAKE) PLATFORM=$(PLATFORM) -C ../raylib/src 
	mv ../raylib/src/libraylib.a libraylib_$(PLATFORM).a

%.h: %.png
	xxd -i $< | sed 's|unsigned int|const unsigned int|g' > $@

%.h: %.glb
	xxd -i $< | sed 's|unsigned int|const unsigned int|g' > $@

%.h: %.xm
	xxd -i $< | sed 's|unsigned int|const unsigned int|g' > $@

%.h: %.ogg
	xxd -i $< | sed 's|unsigned int|const unsigned int|g' > $@

%.h: %.txt
	xxd -i $< | sed 's|unsigned int|const unsigned int|g' > $@

# Project target defined by PROJECT_NAME
$(PROJECT_NAME): $(XXD_RES) $(OBJS)
	$(CC) -o $(PROJECT_BUILD_PATH)/$(PROJECT_NAME)$(EXT) $(OBJS) $(CFLAGS) $(INCLUDE_PATHS) $(LDFLAGS) $(LDLIBS) -D$(PLATFORM)

# special rule to rebuild on new assets
xxd_resources/assets.o: $(XXD_RES) xxd_resources/assets.c
	$(CC) -c xxd_resources/assets.c -o $@ $(CFLAGS) $(INCLUDE_PATHS) -D$(PLATFORM)

# Compile source files
# NOTE: This pattern will compile every module defined on $(OBJS)
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDE_PATHS) -D$(PLATFORM)

# Clean everything
clean:
ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),WINDOWS)
		del *.o xxd_resources/assets.o *.exe /s
    endif
    ifeq ($(PLATFORM_OS),LINUX)
		find . -type f -executable -delete
		rm -fv *.o xxd_resources/assets.o
    endif
    ifeq ($(PLATFORM_OS),OSX)
		rm -f *.o external/*.o xxd_resources/assets.o $(PROJECT_NAME)
    endif
endif
ifeq ($(PLATFORM),PLATFORM_DRM)
	find . -type f -executable -delete
	rm -fv *.o xxd_resources/assets.o
endif
ifeq ($(PLATFORM),PLATFORM_WEB)
	del *.o xxd_resources/assets.o *.html *.js
endif
	@echo Cleaning done

SAN_OPT = ASAN_OPTIONS=abort_on_error=1:fast_unwind_on_malloc=0:detect_leaks=0 UBSAN_OPTIONS=print_stacktrace=1
debug:
	$(SAN_OPT) lldb ./$(PROJECT_NAME)
