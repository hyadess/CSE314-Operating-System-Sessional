g++ -c main.cpp -o main.o
g++ -c CommonFunctions.cpp -o CommonFunctions.o
g++ -c PrintingStation.cpp -o PrintingStation.o
g++ -c BindingStation.cpp -o BindingStation.o
g++ -c Library.cpp -o Library.o
g++ main.o CommonFunctions.o PrintingStation.o BindingStation.o Library.o -o e.exe
rm -r *.o
./e.exe