rm -rf build
mkdir build
gcc src/*.cpp -fno-exceptions -fno-rtti -lallegro -lallegro_primitives -lm -o build/main
