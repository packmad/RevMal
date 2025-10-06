EXES=sum32 sum64 sum32_nofp
CFLAGS=-Wall -ggdb -O0 -fno-pie -no-pie -fno-stack-protector -fno-omit-frame-pointer -fcf-protection=none -fno-builtin -U_FORTIFY_SOURCE

all: $(EXES)

sum32: sum.c
	gcc $(CFLAGS) -m32 -o $@ $^

sum32_nofp: sum.c
	gcc $(CFLAGS) -m32 -o $@ -fomit-frame-pointer $^

sum64: sum.c
	gcc $(CFLAGS) -m64 -o $@ $^

clean:
	rm -f $(EXES)

.PHONY: clean

