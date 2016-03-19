all: sql.cpp
	g++ -o libsql_builder.so -lboost_system -shared -fPIC -D_DEBUG sql.cpp
test: test.cpp all
	g++ -g -ggdb -lboost_system -lboost_test_exec_monitor -lsql_builder -L. -D_DEBUG -o test test.cpp
run: test
	./test
