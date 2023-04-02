# Makefile

src=$(wildcard ./*.cpp)
objs=$(patsubst %.cpp, %.o, $(src))
target=app
$(target):$(objs)
	$(CXX) $(objs) -o $(target)

%.o:%.cpp
	$(CXX) -c $< -o $@

.PHONY:clean
clean:
	rm $(objs) -f

