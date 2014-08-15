all: libcodec.so

libcodec.so: codec.o
	gcc -shared -Wl,-soname,$@ -o $@ $<

codec.o: ../firmware/codec_8b10b.c
	gcc -c -fPIC -std=gnu11 -DCODEC_PC_TEST -O2 $< -o $@

clean:
	rm -f libcodec.so
	rm -f codec.o
		
