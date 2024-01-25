TARGET=whereisdep

build: whereisdep.cpp
	g++ -std=c++17 whereisdep.cpp -o whereisdep
	
buildandrun: whereisdep.cpp
	g++ -std=c++17 whereisdep.cpp -o whereisdep && \
		./whereisdep \
			-p ./examples/simple-react-app/package.json \
			-s ./examples/simple-react-app

install: whereisdep.cpp
	.build
	cp ./whereisdep /usr/local/bin/whereisdep

run: whereisdep.cpp
	./whereisdep \
		-p ./examples/simple-react-app/package.json \
		-s ./examples/simple-react-app
		
		
dev: whereisdep.cpp
	make build
	./whereisdep -v
	
