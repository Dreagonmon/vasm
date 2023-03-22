BUILD=build
CC=gcc
STD=c11

CFLAGS = -std=${STD} -g -Wall

# include
CFLAGS += -Ivasmvm
CFLAGS += ${shell pkg-config --libs --cflags sdl2}

# source
SOURCE = vasmvm/main.c
SOURCE += vasmvm/vasm_core.c
SOURCE += vasmvm/vasm_ext.c

# .SUFFIXES: .c

all: main

main:
	@mkdir -p ${BUILD}
	${CC} ${CFLAGS} -o ${BUILD}/main ${SOURCE}

run: ${SOURCE}
	@mkdir -p ${BUILD}
	@${CC} ${CFLAGS} -o ${BUILD}/main ${SOURCE}
	@chmod +x ${BUILD}/main
	@echo "================"
	@echo ""
	@${BUILD}/main

as_and_run:
	@deno task devas
	@deno task devds
	@mkdir -p ${BUILD}
	@${CC} ${CFLAGS} -o ${BUILD}/main ${SOURCE}
	@chmod +x ${BUILD}/main
	@echo "================"
	@echo ""
	@${BUILD}/main

clean:
	rm -r ${BUILD}
