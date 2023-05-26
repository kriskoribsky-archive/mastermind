# ================================================================================
# PlatformIO Makefile template for TUKE prog-2023 course in C programming language
# Suitable for small to medium projects
# author: kristian.koribsky@student.tuke.sk
# ================================================================================



# RECIPES
# ================================================================================
build: 
	platformio run

test: # TODO add test config for unit testing
	platformio test

static:
	platformio check --fail-on-defect medium --fail-on-defect high

debug:
	platformio debug

full: build static # TODO add test target

upload: 
	platformio run --target upload --upload-port /dev/ttyUSB0



clean:
	platformio run --target clean