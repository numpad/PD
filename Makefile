CC=gcc
CSTD=-std=c99
CWARN=-Wall -pedantic
CLIB=-lSDL2 -lSDL2_image -lSDL2_gfx -lm
CINCLUDE=-Ilib/
CDEF=-DDEBUG -ggdb

src_dir=src
input=main
output=${input}

all:
	${CC} ${CSTD} ${CWARN} ${CINCLUDE} ${src_dir}/${input}.c ${CLIB} -o${output}

debug:
	${CC} ${CSTD} ${CWARN} ${CDEF} ${CINCLUDE} ${src_dir}/${input}.c ${CLIB} -o${output}
