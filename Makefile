LDFLAGS = -lcsv `mysql_config --cflags` `mysql_config --libs`
modbusFlags = -lmodbus `pkg-config libmodbus --cflags`



all : epsolarmonitor tools

epsolarmonitor: #argparser.o
	##g++ solarmonitor.cxx -o bin/solarmonitor ${modbusFlags}
	g++ livestats.cxx -o bin/livestats

tools: loadswitch
	mkdir -p bin/tools
	g++ tools/reader.cxx -o bin/tools/reader ${modbusFlags}
	g++ tools/ScanModBus.cxx -o bin/tools/scanmodbus ${modbusFlags}
	g++ tools/RegScan.cxx -o bin/tools/regscan ${modbusFlags}

argparser:
	g++ argparser.c++ -o argparser.o

loadswitch: 
	g++ tools/loadswitch.cxx -o bin/tools/loadswitch ${modbusFlags}

clean:
	rm *.o

dist-clean:
	rm -r bin/tools/
