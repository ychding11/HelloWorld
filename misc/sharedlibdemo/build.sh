g++ -m64 -Wall -Wextra -march=native -O3 -c b.cpp
g++ -m64 -Wall -Wextra -march=native -O3 -fpic -c a.cpp
g++ -m64 -Wl,-rpath,'$ORIGIN' -Wl,-z,now -shared -o liba.so a.o
g++ -m64 -Wall -Wextra -march=native -O3 -c test.cpp 
g++ -m64 -Wl,-rpath,'$ORIGIN' -Wl,-z,now -o test test.o b.o liba.so -lrt

