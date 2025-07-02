all:
	cd src && make

.PHONY: test

test:
	cd test && make

clean:
	@MFS=`find src/ doc/ -name Makefile | xargs grep -l 'clean:'`; \
	  for MF in $$MFS; do \
	    DIR=`dirname $$MF`; \
	    make -C $$DIR clean; \
	  done

archive: clean
	RELEASE=$$(cat release); \
	/bin/echo "  [TAR] ~/util-src-$$RELEASE.tar.gz"; \
	cd .. && \
	  tar czf ~/work/archive/util-src-$$RELEASE.tar.gz \
	  --exclude=\.git util/

install: all
	cp -rf src/include/util /usr/local/include
	cp -rf src/lib/libutil.a /usr/local/lib
