CC=cc

[ -z "$1" ] && exit 1;

[ -d build ] || mkdir build

$CC src/$1.c -o build/$1

[ $? = 0 ] && [ "$2" = "run" ] && ./build/$1