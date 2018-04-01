run: test
	./build/main  
test: build
	./build/main_test
build: build_dir
	cd build; cmake ..; make; cd ..
docs: doc_dir
	doxygen doxyfile
doc_dir:
	mkdir -p docs/doxygen
build_dir:
	mkdir -p build
clean:
	rm -rf build; rm -rf docs/doxygen