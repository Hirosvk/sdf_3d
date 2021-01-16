main:
	g++\
		-std=c++11\
		-I /usr/local/include/freetype2\
		-L/usr/local/lib/ -lglfw\
		-L/usr/local/lib/ -lglew\
		-L/usr/local/lib/ -lfreetype\
		-framework Cocoa\
		-framework OpenGL\
		-framework IOKit\
		-framework CoreVideo\
		-o main.out\
		-ferror-limit=5\
		main.cpp\
		open_gl/*.cpp

exp:
	g++\
		-std=c++11\
		-o exp.out\
		-ferror-limit=5\
		exp.cpp

test:
	g++\
		-std=c++11\
		-o test.out\
		-ferror-limit=5\
		test.cpp geometry/*.cpp

