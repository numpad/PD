CC=gcc -std=c99
CWARN=-Wall -pedantic
CLIB=-lSDL2 -lSDL2_image -Ilib/QW -lm

src_dir=src
input=main
output=${input}

all:
	${CC} ${CWARN} ${src_dir}/${input}.c ${CLIB} -o${output}

