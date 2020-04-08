rm -rf build
mkdir build
gcc src/*.cpp -lallegro -lallegro_primitives -lm -o build/main
