LDFLAGS = -lcsv `mysql_config --cflags` `mysql_config --libs`
modbusFlags = -lmodbus `pkg-config libmodbus --cflags`



all : epsolarmonitor

epsolarmonitor:

tools:
	mkdir -p bin/tools
	g++ tools/reader.cxx -o bin/tools/reader ${modbusFlags}
	g++ tools/ScanModBus.cxx -o bin/tools/scanmodbus ${modbusFlags}
	g++ tools/RegScan.cxx -o bin/tools/regscan ${modbusFlags}
clean :
	rm -r bin/tools/
