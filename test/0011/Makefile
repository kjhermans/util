all: data exec

data: data.inc

data.inc:
	perl ./gendata.pl > data.inc

exec:
	@$(CC) -I../../src/include \
	  -o /tmp/test test.c ../../src/lib/libutil.a && \
	  /tmp/test && \
	  echo "SUCCESS"
