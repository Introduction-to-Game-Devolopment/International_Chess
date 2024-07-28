# variable (Nhớ để flag -I, -L)
CXX = g++
CXXFLAGS = -I/home/trongdoanh/Documents/Project/GAMEING_WEB/Chess/raylib/include -Wall -std=c++20	#Thay bằng folder chứa include của raylib
LDFLAGS = -L/home/trongdoanh/Documents/Project/GAMEING_WEB/Chess/raylib/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 #Thay đường dẫn bằng folder chưa lib của raylib

# execute file
TARGET = app

# source file
SRCS = main.cpp piece.cpp player.cpp board.cpp general.cpp menu.cpp tool.cpp

# object file 
OBJS = $(SRCS:.cpp=.o)

# default 
all: $(TARGET)

# build execute file
$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LDFLAGS)
menu123: menu.cpp general.cpp
	g++ menu.cpp general.cpp button.cpp -o menu -L/home/trongdoanh/Documents/Project/GAMEING_WEB/Chess/raylib/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# build objects
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# clear
clean:
	rm -f $(OBJS)

# clear all
cleanall: clean
	rm -f $(TARGET) *.o
