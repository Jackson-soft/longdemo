all: clean cmakes build

cmakes:
	cmake -S . -B build -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=YES

build:
	cmake --build build

clean:
	rm -rf build
