enigma: main.o enigma.o plugboard.o rotor.o reflector.o 
	g++ -g main.o enigma.o plugboard.o rotor.o reflector.o -o enigma

main.o: main.cpp errors.h enigma.h
	g++ -Wall -g -c main.cpp

enigma.o: enigma.cpp errors.h enigma.h 
	g++ -Wall -g -c enigma.cpp 

plugboard.o: plugboard.cpp errors.h plugboard.h
	g++ -Wall -g -c plugboard.cpp

rotor.o: rotor.cpp errors.h rotor.h
	g++ -Wall -g -c rotor.cpp

reflector.o: reflector.cpp errors.h reflector.h
	g++ -Wall -g -c reflector.cpp

clean:
	rm -f *.o enigma
