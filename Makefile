all:
	g++ -O3 -s script.cpp -o script.cgi /usr/lib/x86_64-linux-gnu/libcgicc.a
clean:
	rm -f script.cgi
