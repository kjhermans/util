all:
	@/bin/true; \
	  date; \
	  TOP='.'; \
	  if [ "x$$SYSTEMTEST" != "x" ]; then \
	    TOP=$$SYSTEMTEST; \
	  fi; \
	  DIRS=`find $$TOP -maxdepth 1 -type d | sort -n` ; \
	  for DIR in $$DIRS; do \
	    if [ "$$DIR" != "." -a -f "$$DIR/Makefile" ]; then \
	      if grep -q SKIP $$DIR/README.txt 2>/dev/null; then \
	        /bin/echo "  [ST] $$DIR; skipped."; \
	      else \
	        RUN=yes; \
	        if [ -x $$DIR/requirements.sh ]; then \
	          if $$DIR/requirements.sh; then \
	            true; \
	          else \
	            RUN=; \
	          fi; \
	        fi; \
	        if [ "$$RUN" = "yes" ]; then \
	          /bin/echo -n "  [ST] $$DIR; result in /tmp/$$DIR.log .. "; \
	          cd $$DIR && make >/tmp/$$DIR.log 2>&1; \
	          if grep -q SUCCESS /tmp/$$DIR.log; then \
	            echo "	Succeeded."; \
	          else \
	            /bin/echo -e "	\e[31mFailed.\e[39m"; \
	          fi; \
	          cd ..; \
	        else \
	          /bin/echo "  [ST] $$DIR; Requirements not met."; \
	        fi; \
	      fi; \
	    fi; \
	  done; \
	  date

clean:
	@DIRS=`find . -maxdepth 1 -mindepth 1 -type d` ; \
	  for DIR in $$DIRS; do \
	    if [ "$$DIR" != "." ]; then \
	      cd $$DIR && make clean; \
	      cd ..; \
	    fi; \
	  done

