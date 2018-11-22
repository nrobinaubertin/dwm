# dwm version
VERSION = 6.1

X11INC = /usr/include/X11
X11LIB = /usr/lib

# freetype
FREETYPELIBS = -lfontconfig -lXft
FREETYPEINC = /usr/include -I/usr/include/freetype2

# includes and libs
INCS = -I${X11INC} -I${FREETYPEINC}
LIBS = -L${X11LIB} -lX11 ${FREETYPELIBS}

# flags
CPPFLAGS = -D_DEFAULT_SOURCE -D_POSIX_C_SOURCE=2 -DVERSION=\"${VERSION}\"
CFLAGS   = -std=c11 -pedantic -Wall -Wno-unused-result -Wno-deprecated-declarations -Oz ${INCS} ${CPPFLAGS}
LDFLAGS  += ${LIBS}

# compiler and linker
CC = clang
