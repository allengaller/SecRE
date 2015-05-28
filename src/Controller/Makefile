CC=g++
BIN=main TestSys
#TestAll
FLAGS=-Wall -g -fpermissive -std=c++0x -O3
RM=-rm -f
SRC=$(wildcard *.cpp)
OBJ=$(SRC:*.cpp=*.o)
COMMON_OBJ=$(filter-out main.cpp TestSys.cpp, $(OBJ))

#thrift
THRIFT_LINK=-lthrift
THRIFT_LIB=-L/usr/local/lib

#openssl
OPENSSL_LINK=-lcrypto

#caravel
CARAVEL_LINK=-lcaravel
CARAVEL_LIB=-L../Caravel
CARAVEL_INC=-I../Caravel

#GMP
GMP_LINK=-lgmp

#paillier
PAILLIER_LINK=-lpaillier
PAILLIER_INC=-I/usr/local/include
PAILLIER_LIB=-L/usr/local/lib

#combine the inc & lib
INC=$(CARAVEL_INC) $(OPENSSL_INC)
# $(THRIFT_INC) $(PAILLIER_INC)
LIB=$(CARAVEL_LINK) $(CARAVEL_LIB) $(OPENSSL_LINK)
# $(THRIFT_LINK) $(THRIFT_LIB)


all: $(BIN)

%.d: %.cpp
	$(CC) -MM $(FLAGS) $(INC) $< -o $@

%.o: %.cpp %.d
	$(CC) -c $(FLAGS) $(INC) $< -o $@

main:$(COMMON_OBJ)
	$(CC) $(FLAGS) -o $@ $@.cpp $(COMMON_OBJ) $(LIB)

TestSys:$(COMMON_OBJ)
	$(CC) $(FLAGS) -o $@ $@.cpp $(COMMON_OBJ) $(LIB)


clean:
	$(RM) *.o
	$(RM) *.d
	$(RM) $(BIN)

-include $(SRC:.cpp=.d)
