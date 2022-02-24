assembler: main.o utils.o preProcessor.o firstPass.o secondPass.o parseCodeToFile.o dataLinkedList.o wordLinkedList.o
	gcc -ansi -Wall -g main.o utils.o preProcessor.o firstPass.o secondPass.o parseCodeToFile.o dataLinkedList.o wordLinkedList.o -o assembler
main.o: main.c extrnVariables.h utils.h
	gcc -c -ansi -Wall -g main.c -o main.o
utils.o: utils.c utils.h
	gcc -c -ansi -Wall -g utils.c -o utils.o
preProcessor.o: preProcessor.c preProcessor.h
	gcc -c -ansi -Wall -g preProcessor.c -o preProcessor.o
firstPass.o: firstPass.c firstPass.h
	gcc -c -ansi -Wall -g firstPass.c -o firstPass.o
secondPass.o: secondPass.c secondPass.h
	gcc -c -ansi -Wall -g secondPass.c -o secondPass.o
parseCodeToFile: parseCodeToFile.c parseCodeToFile.h
	gcc -c -ansi -Wall -g parseCodeToFile.c -o parseCodeToFile.o
dataLinkedList: dataLinkedList.c dataLinkedList.h
	gcc -c -ansi -Wall -g dataLinkedList.c -o dataLinkedList.o
wordLinkedList: wordLinkedList.c wordLinkedList.h
	gcc -c -ansi -Wall -g wordLinkedList.c -o wordLinkedList.o