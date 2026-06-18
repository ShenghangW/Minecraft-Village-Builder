.default: all

all: gen-village

clean:
	rm -f gen-village *.o

gen-village: main.o task_a.o task_b.o task_c.o
	g++ -Wall -std=c++17 -g -O -o gen-village main.cpp task_a.cpp village.cpp task_b.cpp room.cpp roomManager.cpp decorate.cpp task_c.cpp -lmcpp

	
// Task A compile command for component 1
g++ -Wall -std=c++17 -g -O -o test_taskA_component1 test_taskA_component1.cpp task_a.cpp village.cpp -lmcpp

// Task A compile command for component 2
g++ -Wall -std=c++17 -g -O -o test_taskA_component2 test_taskA_component2.cpp task_a.cpp village.cpp -lmcpp

// Task A compile command for component 3
g++ -Wall -std=c++17 -g -O -o test_taskA_component3 test_taskA_component3.cpp task_a.cpp village.cpp -lmcpp

// Task A compile command for component 4
g++ -Wall -std=c++17 -g -O -o test_taskA_component4 test_taskA_component4.cpp task_a.cpp village.cpp -lmcpp

// Task A compile command for component 5
g++ -Wall -std=c++17 -g -O -o test_taskA_component5 test_taskA_component5.cpp task_a.cpp village.cpp -lmcpp

// Task A compile command for edge case 1
g++ -Wall -std=c++17 -g -O -o test_taskA_edge1 test_taskA_edge1.cpp task_a.cpp village.cpp -lmcpp

// Task A compile command for edge case 2
g++ -Wall -std=c++17 -g -O -o test_taskA_edge2 test_taskA_edge2.cpp task_a.cpp village.cpp -lmcpp
 
// Task B compile command for component 1
g++ -Wall -Werror -std=c++17 -O -g -o test_component1 test_taskB_component1.cpp task_b.cpp room.cpp roomManager.cpp decorate.cpp -lmcpp

// Task B compile command for component 2
g++ -Wall -Werror -std=c++17 -O -g -o test_component2 test_taskB_component2.cpp  task_b.cpp room.cpp roomManager.cpp decorate.cpp -lmcpp

// Task B compile command for component 3
g++ -Wall -Werror -std=c++17 -O -g -o test_component3 test_taskB_component3.cpp  task_b.cpp room.cpp roomManager.cpp decorate.cpp -lmcpp

// Task B compile command for component 4
g++ -Wall -Werror -std=c++17 -O -g -o test_component4 test_taskB_component4.cpp  task_b.cpp room.cpp roomManager.cpp decorate.cpp -lmcpp

// Task B compile command for edge case 3
g++ -Wall -Werror -std=c++17 -O -g -o test_edge3 test_taskB_edgecase.cpp  task_b.cpp room.cpp roomManager.cpp decorate.cpp -lmcpp

// Task B compile command for edge case 4
g++ -Wall -Werror -std=c++17 -O -g -o test_edge4 test_taskB_edge4.cpp  task_b.cpp room.cpp roomManager.cpp decorate.cpp -lmcpp

//Tack C
# Task C Component 1
	g++ -Wall -std=c++17 -g -O -o test_taskC_component1 test_taskC_component1.cpp task_a.cpp task_b.cpp room.cpp roomManager.cpp decorate.cpp task_c.cpp village.cpp cache.h -lmcpp

# Task C Component 2
	g++ -Wall -std=c++17 -g -O -o test_taskC_component2 test_taskC_component2.cpp task_a.cpp task_b.cpp room.cpp roomManager.cpp decorate.cpp task_c.cpp village.cpp cache.h -lmcpp

# Task C Component 3
	g++ -Wall -std=c++17 -g -O -o test_taskC_component3 test_taskC_component3.cpp task_a.cpp task_b.cpp room.cpp roomManager.cpp decorate.cpp task_c.cpp village.cpp cache.h -lmcpp

%.o: %.cpp
	g++ -Wall -Werror -std=c++17 -g -O -c $^
