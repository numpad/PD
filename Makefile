CC=gcc
CSTD=-std=c99
CWARN=-Wall -pedantic
CLIB=-lSDL2 -lSDL2_image -lm
CINCLUDE=-Ilib/

src_dir=src
input=main
output=${input}

all:
	${CC} ${CSTD} ${CWARN} ${CINCLUDE} ${src_dir}/${input}.c ${CLIB} -o${output}

