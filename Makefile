
run : clean testing123
	./testing123	

testing123 : testing123.c src/objects.h
	clang -g -o testing123 testing123.c

clean :
	rm -f testing123

# runval : clean testing123
#     valgrind --track-origins=yes --error-exitcode=2 --leak-check=full ./testing123
	 