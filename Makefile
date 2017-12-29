

server : webSocket.cpp WavHeader.cpp
	g++ -g -I/home/eranl/boost_1_65_1 -I/home/eranl/websocketpp-master webSocket.cpp SampleBuffer.cpp WavHeader.cpp -lboost_system -L/home/eranl/boost_1_65_1/stage/lib -o server -lboost_chrono -std=c++11

copy : WavHeader.cpp copyWav.cpp
	g++ -g -I/home/eranl/boost_1_65_1 -I/home/eranl/websocketpp-master copyWav.cpp  WavHeader.cpp SampleBuffer.cpp -lboost_system -L/home/eranl/boost_1_65_1/stage/lib -o copyWav -lboost_chrono -std=c++11

echo: wsServerLib
	g++ -g -std=c++11 -Wall EchoServer.cpp -o EchoServer  -I/home/eranl/websocketpp-master -I/home/eranl/boost_1_65_1 -L /home/eranl/WebsocketServer -lElWSServer -L/home/eranl/boost_1_65_1/stage/lib -lboost_chrono -lboost_system

streamWav: wsServerLib
		g++ -g -std=c++11 -Wall StreamWavServer.cpp -o StreamWavServer -I/home/eranl/ElInfra -I/home/eranl/websocketpp-master -I/home/eranl/boost_1_65_1 -L /home/eranl/WebsocketServer -lElWSServer -L/home/eranl/ElInfra -lElInfra -L/home/eranl/boost_1_65_1/stage/lib -lboost_chrono -lboost_system

sineWav: wsServerLib
		g++ -g -std=c++11 -Wall SineWavServer.cpp -o SineWavServer -I/home/eranl/ElInfra -I/home/eranl/websocketpp-master -I/home/eranl/boost_1_65_1 -L /home/eranl/WebsocketServer -lElWSServer -L/home/eranl/ElInfra -lElInfra -L/home/eranl/boost_1_65_1/stage/lib -lboost_chrono -lboost_system

wsServerLib: source
	g++ -shared -o libElWSServer.so WebsocketServer.o ElWSSEcho.o ElWSStream.o ElWSSine.o

source: WebsocketServer.cpp ElWSSEcho.cpp ElWSStream.cpp ElWSSine.cpp
	g++ -g -I/home/eranl/boost_1_65_1 -I/home/eranl/websocketpp-master WebsocketServer.cpp  -std=c++11 -g -Wall -c -fPIC -o WebsocketServer.o
	g++ -g -I/home/eranl/boost_1_65_1 -I/home/eranl/websocketpp-master  ElWSSEcho.cpp -std=c++11 -g -Wall -c -fPIC -o ElWSSEcho.o
	g++ -g -I/home/eranl/boost_1_65_1 -I/home/eranl/websocketpp-master -I/home/eranl/ElInfra  ElWSStream.cpp -std=c++11 -g -Wall -c -fPIC -o ElWSStream.o
	g++ -g -I/home/eranl/boost_1_65_1 -I/home/eranl/websocketpp-master -I/home/eranl/ElInfra  ElWSSine.cpp -std=c++11 -g -Wall -c -fPIC -o ElWSSine.o
