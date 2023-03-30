# Name: Kevin Qian
# SID: 1709188
# CCID: cq4
# AnonID: 1000317291
# CMPUT 275 , Winter 2023
# Assignment 1: Part 1

# This Makefile compiles the program and runs it on the test cases.
run:
	g++ -o server digraph.cpp server.cpp dijkstra.cpp
	./server < ../tests/test00-input.txt > output.txt
	diff temp.txt ../tests/test00-output.txt

clean:
	rm -rf server output.txt