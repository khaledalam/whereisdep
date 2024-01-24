TARGET=whereisdep

build: whereisdep.cpp
	g++ -std=c++17 whereisdep.cpp -o whereisdep
	
buildandrun: whereisdep.cpp
	g++ -std=c++17 whereisdep.cpp -o whereisdep && \
		./whereisdep \
			-p ./examples/simple-react-app/package.json \
			-s ./examples/simple-react-app
	

run: whereisdep.cpp
	./whereisdep \
		-p ./examples/simple-react-app/package.json \
		-s ./examples/simple-react-app