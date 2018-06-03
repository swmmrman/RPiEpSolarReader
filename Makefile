LDFLAGS = -lcsv `mysql_config --cflags` `mysql_config --libs`
modbusFlags = -lmodbus `pkg-config libmodbus --cflags`



all : reader

reader: 
	g++ reader.cxx -o bin/reader ${modbusFlags} 
clean :
	
