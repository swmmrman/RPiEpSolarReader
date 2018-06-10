LDFLAGS = -lcsv `mysql_config --cflags` `mysql_config --libs`
modbusFlags = -lmodbus `pkg-config libmodbus --cflags`



all : epsolarmonitor

epsolarmonitor:

debug-tools:
	mkdir -p bin/tools
	g++ reader.cxx -o bin/tools/reader ${modbusFlags}
	g++ ScanModBus.cxx -o bin/tools/scanmodbus ${modbusFlags}
	g++ RegScan.cxx -o bin/tools/regscan ${modbusFlags}
clean :
	rm -r bin/tools/ /bin/reader
