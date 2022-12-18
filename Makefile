SRC = main.cpp
TARGET = main
LIB = chat
PREFIX = usr/local/bin

privet:		$(SRC) lib
			g++ -std=c++17 -o $(TARGET) $(SRC) -L. -l$(LIB)

lib:		$(LIB).cpp $(LIB).h message.h SHA-1.h user.h
			g++ -std=c++17 -o $(LIB).o $(LIB).cpp -c
			ar rc lib$(LIB).a $(LIB).o

clean: 
			rm *.o rm *.a

install:
			install $(TARGET) $(PREFIX)

uninstall:
			rm -rf $(PREFIX)/$(TARGET)
