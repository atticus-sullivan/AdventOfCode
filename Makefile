.PHONY: clean

WFLAGS = -Wall -Wextra -Wpedantic -Waddress -Warray-bounds -Wfloat-equal -Wformat -Wimplicit -Wimplicit-fallthrough -Winline -Wall -g

OPTIMIZATION= -O0

SANITIZE=

LIST = 01 02 03 04 05 06 07 08
targets = $(addprefix day, $(LIST))

all: $(targets)

run: all
	@$(foreach num,$(LIST),printf "\n\n./day$(num)\n" ; ./day$(num);)

sanAll: asan ubsan lsan tsan stat

asan: SANITIZE = -fsanitize=address
asan: run

ubsan: SANITIZE = -fsanitize=undefined
ubsan: run

lsan: SANITIZE = -fsanitize=leak
lsan: run

tsan: SANITIZE = -fsanitize=thread
tsan: run

stat: SANITIZE = -fanalyzer
stat: run

$(targets): day%: day-%.c read_file.o
	$(CC) $(SANITIZE) $(OPTIMIZATION) -o $@ $^ $(WFLAGS)

read_file.o: read_file.c read_file.h
	$(CC) $(SANITIZE) $(OPTIMIZATION) -c -o $@ $< $(WFLAGS)
