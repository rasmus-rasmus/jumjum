build-project:
	./scripts/build.sh

test: build-project
	./build/src/executables/jumjum-test

playground: build-project
	./build/src/executables/playground
	
