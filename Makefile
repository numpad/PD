CC=gcc
CSTD=-std=c99
CWARN=-Wall -pedantic
CLIB=-lSDL2 -lSDL2_image -Ilib -lm

src_dir=src
input=main
output=${input}

all:
	${CC} ${CSTD} ${CWARN} ${src_dir}/${input}.c ${CLIB} -o${output}

