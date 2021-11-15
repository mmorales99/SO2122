make:
	make srvr
	make clnt

srvr:
	cd servidor && rm -rf bin && mkdir bin/ && gcc -o bin/Servidor src/servidor.c -iquote .. ../lib.h && cd ..

clnt:
	cd cliente && rm -rf bin && mkdir bin/ && gcc -o bin/Cliente src/cliente.c -iquote .. ../lib.h && cd ..

clean:
	rm -rf servidor/bin 2> /dev/null
	rm -rf cliente/bin 2> /dev/null
	rm *.html 2> /dev/null