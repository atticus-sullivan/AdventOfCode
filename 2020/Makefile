.PHONY: clean

WFLAGS = -Wall -Wextra -Waddress -Warray-bounds -Wfloat-equal -Wformat -Wimplicit -Wimplicit-fallthrough -Winline -g

LFLAGS = -lm

OPTIMIZATION = -O0

SANITIZE =

DEBUG=-DDEBUG_EXT -DDEBUG
DEBUG=-DDEBUG
DEBUG=

LIST = 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23
targets         = $(addprefix day, $(LIST))

asantargets     = $(addsuffix -asan,     $(targets))
lsantargets     = $(addsuffix -lsan,     $(targets))
ubsantargets    = $(addsuffix -ubsan,    $(targets))
tsantargets     = $(addsuffix -tsan,     $(targets))
stattargets     = $(addsuffix -stat,     $(targets))
valgrindtargets = $(addsuffix -valgrind, $(targets))
perftargets     = $(addsuffix -perf,     $(targets))

.PHONY: $(asantargets) $(lsantargets) $(ubsantargets) $(tsantargets) $(stattargets) $(valgrindtargets)

all: $(targets)

run: all
	@$(foreach num,$(LIST),printf "\n\n./day$(num)\n" ; ./day$(num);)

$(asantargets): day%-asan: day-%.c read_file.o
	$(CC) $(SANITIZE) $(OPTIMIZATION) -o $@ $^ $(WFLAGS) $(LFLAGS) -fsanitize=address
	./$@

$(lsantargets): day%-lsan: day-%.c read_file.o
	$(CC) $(SANITIZE) $(OPTIMIZATION) -o $@ $^ $(WFLAGS) $(LFLAGS) -fsanitize=leak
	./$@

$(ubsantargets): day%-ubsan: day-%.c read_file.o
	$(CC) $(SANITIZE) $(OPTIMIZATION) -o $@ $^ $(WFLAGS) $(LFLAGS) -fsanitize=undefined
	./$@

$(tsantargets): day%-tsan: day-%.c read_file.o
	$(CC) $(SANITIZE) $(OPTIMIZATION) -o $@ $^ $(WFLAGS) $(LFLAGS) -fsanitize=thread
	./$@

$(stattargets): day%-stat: day-%.c read_file.o
	$(CC) $(SANITIZE) $(OPTIMIZATION) -o $@ $^ $(WFLAGS) $(LFLAGS) -fanalyzer

$(valgrindtargets): day%-valgrind: day-%.c read_file.o
	$(CC) $(SANITIZE) $(OPTIMIZATION) -o $@ $^ $(WFLAGS) $(LFLAGS)
	valgrind --tool=memcheck --leak-check=full ./$@

$(perftargets): day%-perf: day-%.c read_file.o
	$(CC) $(SANITIZE) $(OPTIMIZATION) -o $@ $^ $(WFLAGS) $(LFLAGS)
	$(RM) $@.perf
	perf record -o $@.perf ./$@
	perf report -i $@.perf

$(targets): day%: day-%.c read_file.o
	$(CC) $(DEBUG) $(SANITIZE) $(OPTIMIZATION) -o $@ $^ $(WFLAGS) $(LFLAGS)

read_file.o: read_file.c read_file.h
	$(CC)  $(SANITIZE) $(OPTIMIZATION) -c -o $@ $< $(WFLAGS)

clean:
	$(RM) $(targets) read_file.o $(asantargets) $(lsantargets) $(ubsantargets) $(tsantargets) $(stattargets) $(valgrindtargets) $(perftargets) $(perftargets).perf
