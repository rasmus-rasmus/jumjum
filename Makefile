build-project:
	./scripts/build.sh

test: build-project
	./build/src/executables/test

playground: build-project
	./build/src/executables/playground
	