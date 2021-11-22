int TAM_BUFF = 5;
semaphore e,s,n;

productor(){
    while(true){
        cout << "Produciendo una (1) unidad de algo..." << endl;
        P(e);
        P(s);
        // añadir al buffer;
        V(s);
        V(n);
    }
}

consumidor(){
    while(true){
        P(n);
        P(s);
        // coger del buffer;
        V(s);
        V(e);
        cout << "Consumiendo una (1) unidad de algo..." << endl;
    }
}

main(){
    initialsem(s,1);
    initialsem(n,0);
    initialsem(e,TAM_BUFF);
    codebegin{
        productor();
        consumidor();
    }
}