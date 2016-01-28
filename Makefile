CC=g++
CWARN=-Wall -pedantic
CLIB=-lSDL2 -lSDL2_image -Ilib/QW

src_dir=src
input=main
output=${input}

all:
	${CC} ${CWARN} ${src_dir}/${input}.cpp ${CLIB} -o${output}

