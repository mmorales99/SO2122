#include <iostream>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <bits/stdc++.h>

using namespace std;

const short unsigned int MAX_MEM = 2000;
const short unsigned int MIN_ASS_MEM = 100;
const string OUTPUT = "particiones.txt";

class Proceso{
    private:
        string name;
        int arrival = 0;
        int required_mem = 0;
        int exec_time = 0;
        int mounting_point=-1;
    
    public:
    int get_mp(){
        return this->mounting_point;
    }
    int get_rm(){
        return this->required_mem;
    }
    int get_at(){
        return this->arrival;
    }
    int get_et(){
        return this->exec_time;
    }
    void setMountingPoint(int mp){
        if(mp>=0) this->mounting_point = mp;
    }
    void setName(string s){
        this->name = s;
    }
    void setArrival(int a){
        this->arrival = a;
    }
    void setRequiredMem(int m){
        this->required_mem = m;
    }
    void setExecTime(int e){
        this->exec_time = e;
    }
    string tostring(){
        char *r;
        sprintf(r, "[%d %s %d]", this->get_mp(), this->name.c_str(), this->get_rm());
        return string(r);
    }
};

class Event{
    private:
    int num=-1; // numero del evento, orden cronologico
    vector<Proceso> list;
    public:
    Event(int n){
        this->num = n;
    }
    void addElement(Proceso p){
        this->list.push_back(p);
    }
    vector<Proceso> getList(){
        return list;
    }
    int getOrder(){
        return num;
    }
    void setOrder(int n){
        this->num = n;
    }
};

class Hueco : public Proceso{
    public:
    Hueco(int start, int space){
        this->setMountingPoint(start);
        this->setRequiredMem(space);
    }
    string tostring(){
        char *r;
        sprintf(r, "[%d hueco %d]", this->get_mp(), this->get_rm());
        return string(r);
    }
};

void saveResults(vector<Event> *events) {
    ofstream file(OUTPUT);
    if(file.is_open()){
        for(unsigned int i=0;i<events->size();i++){
            string str;
            str+=events->at(i).getOrder();
            str+=" ";
            vector<Proceso> list = events->at(i).getList();
            for(int j=0;j<list.size();j++){
                str += list.at(j).tostring();
                str += " ";
            }
            str+="\n";
            file << str;
        }
    }
	file.close();
}

void readFile(string filename, vector<Event> *events){
    ifstream file(filename);
    if(file.is_open()) {
        Event *e = new Event(0);
        string str;
        while(getline(file, str)){
            vector<string> tokens;
            stringstream ss(str);
            string aux;
            while (getline(ss,aux,' ')){
                tokens.push_back(aux);
            }
            Proceso *pp = new Proceso();
            pp->setName(tokens[0]);
            pp->setArrival(atoi(tokens[1].c_str()));
            pp->setRequiredMem(atoi(tokens[2].c_str()));
            pp->setExecTime(atoi(tokens[3].c_str()));
            e->addElement(*pp);
        }
        events->push_back(*e);
    }
	file.close();
}

bool procesosArrivaltimeSort(Proceso p1, Proceso p2){
    return(p1.get_at() < p2.get_at());
}

bool procesosMountingPointSort(Proceso p1, Proceso p2){
    return(p1.get_mp() < p2.get_mp());
}

void mejorHueco(vector<Event> *events){
    vector<Proceso> runnign_proces;
    vector<Hueco> huecos;
    int i = 0, j = 0;
    vector<Proceso> list_p = events->at(0).getList();
    sort(list_p.begin(), list_p.end(),procesosArrivaltimeSort);
    huecos.push_back(Hueco(0, MAX_MEM));
    Proceso pp = list_p.at(j++);
    while(true){
        for(vector<Proceso>::iterator pos = runnign_proces.begin(); pos != runnign_proces.end();pos++){
            Proceso pa = *pos;
            if( i + pa.get_at() >= pa.get_et() ){
                Hueco h1(pa.get_mp(), pa.get_rm());
                huecos.push_back(h1);
                runnign_proces.erase(pos);
            }
        }
        sort(huecos.begin(), huecos.end(), procesosArrivaltimeSort);
        for(vector<Hueco>::iterator pos1 = huecos.begin(); pos1 != huecos.end();pos1++){
            Hueco h1 = *pos1;
            for(vector<Hueco>::iterator pos2 = huecos.begin(); pos2 != huecos.end();pos2++){
                Hueco h2 = *pos2;
                if(h1.get_mp()==h2.get_mp()) continue;
                if((h1.get_mp()+h1.get_rm())==(h2.get_mp()-1)){
                    h1.setRequiredMem(h1.get_rm() + h2.get_rm());
                    huecos.erase(pos2);
                }
            }
        }
        if(pp.get_at() == i){
            vector<Hueco>::iterator pos;
            Hueco h = huecos.at(0);
            for(vector<Hueco>::iterator pos = huecos.begin(); pos != huecos.end();pos++){
                Hueco h1 = *pos;
                if(h1.get_mp() == h.get_mp()) continue;
                if(h1.get_rm() < h.get_rm())
                    if(pp.get_rm() <= h1.get_rm())
                        h = h1;
            }
            pp.setMountingPoint(h.get_mp());
            if(pp.get_rm() == h.get_rm()){
                huecos.erase(pos);
            }
            else{ h.setMountingPoint(pp.get_rm()); h.setRequiredMem(h.get_rm() - pp.get_rm());}
            runnign_proces.push_back(pp);
            if(j < list_p.size()) 
                    pp = list_p.at(j++);
        }
        Event *e = new Event(i);
        for(Proceso pa: runnign_proces) e->addElement(pa);
        events->push_back(*e);
        i++;
        if(runnign_proces.empty() && j>= list_p.size()) break;
    }
}

void peorHueco(vector<Event> *events){
    vector<Proceso> runnign_proces;
    vector<Hueco> huecos;
    int i = 0, j = 0;
    vector<Proceso> list_p = events->at(0).getList();
    sort(list_p.begin(), list_p.end(),procesosArrivaltimeSort);
    huecos.push_back(Hueco(0, MAX_MEM));
    Proceso pp = list_p.at(j++);
    while(true){
        for(vector<Proceso>::iterator pos = runnign_proces.begin(); pos != runnign_proces.end();pos++){
            Proceso pa = *pos;
            if( i + pa.get_at() >= pa.get_et() ){
                Hueco h1(pa.get_mp(), pa.get_rm());
                huecos.push_back(h1);
                runnign_proces.erase(pos);
            }
        }
        sort(huecos.begin(), huecos.end(),procesosArrivaltimeSort);
        for(vector<Hueco>::iterator pos1 = huecos.begin(); pos1 != huecos.end();pos1++){
            Hueco h1 = *pos1;
            for(vector<Hueco>::iterator pos2 = huecos.begin(); pos2 != huecos.end();pos2++){
                Hueco h2 = *pos2;
                if(h1.get_mp()==h2.get_mp()) continue;
                if((h1.get_mp()+h1.get_rm())==h2.get_mp()-1){
                    h1.setRequiredMem(h1.get_rm() + h2.get_rm());
                    huecos.erase(pos2);
                }
            }
        }
        if(pp.get_at()==i){
            int max = huecos.at(0).get_rm();
            int index = 0;
            for(unsigned int i = 0; i< huecos.size();i++){
                Hueco h1 = huecos.at(i);
                if(h1.get_rm()>max){
                    max = h1.get_rm();
                    index = i;
                }
            }
            Hueco h = huecos.at(index);
            if(h.get_rm()>pp.get_rm()){
                pp.setMountingPoint(MAX_MEM - h.get_rm());
                runnign_proces.push_back(pp);
                h.setRequiredMem(h.get_rm() - pp.get_rm());
                h.setMountingPoint(pp.get_rm());
                if(j < list_p.size())
                    pp = list_p.at(j++);
            }
        }
        Event *e = new Event(i);
        for(Proceso pa: runnign_proces) e->addElement(pa);
        events->push_back(*e);
        i++;
        if(runnign_proces.empty() && j>= list_p.size()) break;
    }
}

void siguienteHueco(vector<Event> *events){
    vector<Proceso> runnign_proces;
    vector<Hueco> huecos;
    int i = 0, j = 0;
    vector<Proceso> list_p = events->at(0).getList();
    sort(list_p.begin(), list_p.end(),procesosArrivaltimeSort);
    huecos.push_back(Hueco(0, MAX_MEM));
    Proceso pp = list_p.at(j++);
    int last_assigned = -1;
    while(true){
        for(vector<Proceso>::iterator pos = runnign_proces.begin(); pos != runnign_proces.end();pos++){
            Proceso pa = *pos;
            if( i + pa.get_at() >= pa.get_et() ){
                Hueco h1(pa.get_mp(), pa.get_rm());
                huecos.push_back(h1);
                runnign_proces.erase(pos);
            }
        }
        sort(huecos.begin(), huecos.end(),procesosArrivaltimeSort);
        for(vector<Hueco>::iterator pos1 = huecos.begin(); pos1 != huecos.end();pos1++){
            Hueco h1 = *pos1;
            for(vector<Hueco>::iterator pos2 = huecos.begin(); pos2 != huecos.end();pos2++){
                Hueco h2 = *pos2;
                if(h1.get_mp()==h2.get_mp()) continue;
                if((h1.get_mp()+h1.get_rm())==h2.get_mp()-1){
                    h1.setRequiredMem(h1.get_rm() + h2.get_rm());
                    huecos.erase(pos2);
                }
            }
        }
        if(pp.get_at()==i){
            int index = 0;
            for(unsigned int i = index; i< huecos.size();i++){
                if(last_assigned == huecos.at(i).get_mp()){
                    index = i;
                    break;
                }
            }
            for(unsigned int i = index; i< huecos.size();i++){
                if(pp.get_rm() <= huecos.at(i).get_rm()){
                    index = i;
                    break;
                }
            }
            Hueco h = huecos.at(index);
            if(h.get_rm()>pp.get_rm()){
                pp.setMountingPoint(MAX_MEM - h.get_rm());
                runnign_proces.push_back(pp);
                h.setRequiredMem(h.get_rm() - pp.get_rm());
                h.setMountingPoint(pp.get_rm());
                last_assigned = h.get_mp();
                if(j < list_p.size())
                    pp = list_p.at(j++);
            }
        }
        Event *e = new Event(i);
        for(Proceso pa: runnign_proces) e->addElement(pa);
        events->push_back(*e);
        i++;
        if(runnign_proces.empty() && j>= list_p.size()) break;
    }
}

void primerHueco(vector<Event> *events){
    vector<Proceso> runnign_proces;
    vector<Hueco> huecos;
    int i = 0, j = 0;
    vector<Proceso> list_p = events->at(0).getList();
    sort(list_p.begin(), list_p.end(),procesosArrivaltimeSort);
    huecos.push_back(Hueco(0, MAX_MEM));
    Proceso pp = list_p.at(j++);
    while(true){
        for(vector<Proceso>::iterator pos = runnign_proces.begin(); pos != runnign_proces.end();pos++){
            Proceso pa = *pos;
            if( i + pa.get_at() >= pa.get_et() ){
                Hueco h1(pa.get_mp(), pa.get_rm());
                huecos.push_back(h1);
                runnign_proces.erase(pos);
            }
        }
        sort(huecos.begin(), huecos.end(),procesosArrivaltimeSort);
        for(vector<Hueco>::iterator pos1 = huecos.begin(); pos1 != huecos.end();pos1++){
            Hueco h1 = *pos1;
            for(vector<Hueco>::iterator pos2 = huecos.begin(); pos2 != huecos.end();pos2++){
                Hueco h2 = *pos2;
                if(h1.get_mp()==h2.get_mp()) continue;
                if((h1.get_mp()+h1.get_rm())==h2.get_mp()-1){
                    h1.setRequiredMem(h1.get_rm() + h2.get_rm());
                    huecos.erase(pos2);
                }
            }
        }
        if(pp.get_at()==i){
            int index = 0;
            for(unsigned int i = 0; i< huecos.size();i++){
                if(pp.get_rm() <= huecos.at(i).get_rm()){
                    index = i;
                    break;
                }
            }
            Hueco h = huecos.at(index);
            if(h.get_rm()>pp.get_rm()){
                pp.setMountingPoint(MAX_MEM - h.get_rm());
                runnign_proces.push_back(pp);
                h.setRequiredMem(h.get_rm() - pp.get_rm());
                h.setMountingPoint(pp.get_rm());
                if(j < list_p.size())
                    pp = list_p.at(j++);
            }
        }
        Event *e = new Event(i);
        for(Proceso pa: runnign_proces) e->addElement(pa);
        events->push_back(*e);
        i++;
        if(runnign_proces.empty() && j>= list_p.size()) break;
    }
}

void executeAlgorithms(string file, int opt){
    vector<Event> *events = new vector<Event>();
    readFile(file,events);
    switch(opt){
        case 'M':
            mejorHueco(events);
            break;
        case 'P':
            peorHueco(events);
            break;
        case 'S':
			siguienteHueco(events);
		    break;
        case 'F':
			primerHueco(events);
		    break;
        default:
            cout << "Opcion incorrecta... Use: M,P,S,F" << endl;
            return;
    }
    saveResults(events);
}

int main(int argc, char **arrgv){
    do {                
        printf("Introduce la direccion del archivo que contiene los procesos a simular: ");
        string filename; getline(cin, filename);
        printf("Que algoritmo deseas aplicar? Mejor Hueco: M | Peor Hueco: P ");
        char opt = getchar();
        opt = toupper(opt);
        executeAlgorithms(filename, opt);
        printf("Los resultados los puedes consultar en el archivo: %s", OUTPUT.c_str());
        printf("Quieres visualizarlos ahora? [S/N]");
        printf("Quieres introducir nuevos procesos? Se borrara el contenido de %s [S/N]", OUTPUT.c_str());
        if(toupper(getchar())=='N') break;
    } while (true);
    return 0;
}
