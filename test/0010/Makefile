all:
	@$(CC) -I../../src/include \
	  -o /tmp/test test.c ../../src/lib/libutil.a && \
	  rm -f /tmp/log && \
	  valgrind /tmp/test > /tmp/log 2>&1 && \
	  grep -i 'no leaks' /tmp/log && \
	  echo "SUCCESS"

