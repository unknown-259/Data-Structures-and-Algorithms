#------------------------------------------------------------------------------
#  Makefile for CSE 101 Programming Assignment 8
#
#  make                		makes Order and WordFrequency
#  make DictionaryTest      make DictionaryTest
#  make clean          		removes binary files
#  make DictionaryCheck     runs DictionaryTest under valgrind
#  make WordFrequencyCheck  runs WordFrequency under valgrind on Gutenberg
#------------------------------------------------------------------------------

All : Order WordFrequency

Order : Order.o Dictionary.o
	g++ -std=c++17 -Wall -o Order Order.o Dictionary.o 

Order.o : Dictionary.h Order.cpp
	g++ -std=c++17 -Wall -c Order.cpp

WordFrequency : WordFrequency.o Dictionary.o
	g++ -std=c++17 -Wall -o WordFrequency WordFrequency.o Dictionary.o 

WordFrequency.o : Dictionary.h WordFrequency.cpp
	g++ -std=c++17 -Wall -c WordFrequency.cpp

DictionaryTest : DictionaryTest.o Dictionary.o
	g++ -std=c++17 -Wall -o DictionaryTest DictionaryTest.o Dictionary.o 

DictionaryTest.o : Dictionary.h DictionaryTest.cpp
	g++ -std=c++17 -Wall -c DictionaryTest.cpp

Dictionary.o : Dictionary.h Dictionary.cpp
	g++ -std=c++17 -Wall -c Dictionary.cpp

clean :
	rm -f Order Order.o WordFrequency WordFrequency.o DictionaryTest DictionaryTest.o Dictionary.o

DictionaryCheck : DictionaryTest
	valgrind --leak-check=full DictionaryTest

WordFrequencyCheck : Order
	valgrind --leak-check=full Order in5 junk5

cleanTxt:
	rm -f *.txt

spotless:
	rm -f *.txt *.sh