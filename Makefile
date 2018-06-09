LDFLAGS = -lcsv `mysql_config --cflags` `mysql_config --libs`
modbusFlags = -lmodbus `pkg-config libmodbus --cflags`



all : reader

reader:
	g++ reader.cxx -o bin/reader ${modbusFlags}
debug-tools:
	mkdir -p bin/tools
	g++ ScanModBus.cxx -o bin/tools/scanmodbus ${modbusFlags}
	g++ RegScan.cxx -o bin/tools/regscan ${modbusFlags}
clean :
	rm -r bin/tools/ /bin/reader
