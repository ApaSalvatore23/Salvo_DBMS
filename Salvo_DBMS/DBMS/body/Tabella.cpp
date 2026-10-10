#include "Tabella.hpp" 
#include <algorithm>
#include <iostream>   
using namespace std;
//NOTA: scrivere bool Tabella :: metodo...Signfica "prendere" dalla classe Tabella dentro Tabella.hpp (Non prendere da Tabella.hpp direttamente!)

//IMPLEMENTAZIONE CORPO DEI METODI:

//implementazine costruttore:
Tabella :: Tabella(std::string n, std::vector<Colonna> a) : nomeTab(n), attributi(a){}

//implementazione del metodo privato di controllo:
bool Tabella::controlla(const Value& val_riga, TipoColonna attributo) const {
 //a seconda del tipo svolge il case:
    switch(attributo){
    case TipoColonna :: INT :
        return holds_alternative<int>(val_riga);
    case TipoColonna :: STRING :
        return holds_alternative<string>(val_riga);
    case TipoColonna :: DOUBLE :
        return holds_alternative<double>(val_riga);
    }//holds_alternative si occupa di verificare se il val_riga(un Value) corrisponde con il tipo di dato Template <T>
    return false; //return di sicurezza (magari in futuro confronto valori nuovi che non ho aggiornato)
}

//metodo di supporto per i confronti del metodo pubblico 'filtra_istanze': 
bool Tabella::confronta(const Value& att, std::string op, const Value& target) const {
    if(op == "="){
        if(holds_alternative<int>(att) && holds_alternative<int>(target)){return get<int>(att) == get<int>(target);}
        else if(holds_alternative<string>(att) && holds_alternative<string>(target)){return get<string>(att) == get<string>(target);}
        else if(holds_alternative<double>(att) && holds_alternative<double>(target)){return get<double>(att) == get<double>(target);}
    }
    else if(op == ">"){
        if(holds_alternative<int>(att) && holds_alternative<int>(target)){return get<int>(att) > get<int>(target);}
        else if(holds_alternative<string>(att) && holds_alternative<string>(target)){return get<string>(att) > get<string>(target);}
        else if(holds_alternative<double>(att) && holds_alternative<double>(target)){return get<double>(att) > get<double>(target);}
    }
    else if(op == "<"){
        if(holds_alternative<int>(att) && holds_alternative<int>(target)){return get<int>(att) < get<int>(target);}
        else if(holds_alternative<string>(att) && holds_alternative<string>(target)){return get<string>(att) < get<string>(target);}
        else if(holds_alternative<double>(att) && holds_alternative<double>(target)){return get<double>(att) < get<double>(target);}
    }
    return false; //return di sicurezza!     
}


//implementazione di inserisci_istanza:
bool Tabella :: inserisci_istanza(const Riga& r){
    //controlliamo se è una riga valida:
    if(r.riga.size() != attributi.size()){return false;}
    //per ogni elemento nella riga:
    for(int i=0; i<r.riga.size(); ++i){
        //confronta il relativo attributo:
        if(!controlla(r.riga[i], this->attributi[i].tipo)){return false;}
    }
    //altrimenti inseriamo la riga nella tabella:
    this->istanze.push_back(r);
    return true;
}

//implementazione di getIndiceColonna:
int Tabella :: getIndiceColonna(const std::string& nomeC) const {
    //usiamo il find per cercare l iteratore all attributo: 
    auto it = find_if(this->attributi.begin(), this->attributi.end(), [nomeC](const Colonna& c){return c.nome == nomeC;});
    //se effettivamente lo ha trovato:
    if(it != this->attributi.end()){
       //ricaviamo l indice con distance():
        return distance(this->attributi.begin(), it);
    }
    return -1; //se non lo trova
}

//implementazone della stampa:
void Tabella :: stampa_tabella() const {
    //stampiamo il nome della Tabella:
    cout<<"Nome Tabella: "<<nomeTab<<endl;
    //poi in successione le colonne(attributi):
    for(const auto& c : attributi){
        cout<< c.nome <<"|"; //ricorda che c è una struct Colonna! 
    }
    cout<<endl;
    //poi in sucessione ogni riga:
    for(const auto& r : istanze){ //per ogni riga...
        for(const auto& r_val : r.riga){ //...il suo valore(Value AS variant) all attributo
            //ne verifichiamo la corrispondenza corretta di valore, estraendone e stampando il contenuto:
            if(holds_alternative<int>(r_val)){cout<<get<int>(r_val);}  //col get estraiamo il contenuto reale, di r_val che è un Value(variant)
            else if(holds_alternative<string>(r_val)){cout<<get<string>(r_val);}
            else if(holds_alternative<double>(r_val)){cout<<get<double>(r_val);}
            cout<<"|";
        }
        cout<<endl;
    }
    return;
}

//implementazione dei getter di Attributi e Righe:
string Tabella :: getNome() const {return nomeTab;}
const vector<Colonna>& Tabella :: getAttributi() const {return attributi;}
const vector<Riga>& Tabella :: getIstanze() const {return istanze;}

//Metodi che serviranno per il Query Engine:-------------------------------------------------------------------------------

//Metodo di supporto per il SELECT: simile al metodo di stampa, ma prendera una Tabella specifica(gia filtrata o meno)
std::vector<Riga> Tabella :: seleziona_attributi(const vector<Riga>& tab_operazione, const std::vector<std::string>& att) const {
    vector<Riga> tab_filtrata{};
    //individuiamo gli indici degli attributi in questione, ma lanciamo errore e fermiamo se ce almeno un indice non valido:
    std::vector<int> indici{};
    //controliammo se non sia vuota:
    if(att.empty()){cout<<"ATTENZIONE: sequenza di attributi vuota!"<<endl; return {};}
    //inseriamo gli indici colonna per colonna 
    for(const std::string& a : att){
        int idx = getIndiceColonna(a); //per non richiamare la funzione piu volte, meglio chiamarla una sola volta salvando il risultato come 'idx'
        //se anche solo uno non è valido o inesistente, chiudi e lancia errore:
        if(idx == -1){cout<<"ATTENZIONE: sequenza di attributi non valida!"<<endl; return {};}
        else{indici.push_back(idx);}
    }
    //ora prendiamo le istanze solo con gli attributi(indici) necessari:
    for(const Riga& r : tab_operazione){//per ogni istanza nella Tabella su cui operare
        Riga new_riga; //crea una nuova istanza/Riga
        for(int i : indici){//su cui inserire solo i valori degli attributi con indice salvato
            new_riga.riga.push_back(r.riga[i]);
        }
        tab_filtrata.push_back(new_riga);
    }
    return tab_filtrata;
}

//Metodo di supporto per il WHERE: deve filtrare le istanze e restituire la tabella filtrata(un vector<Riga>) 
std::vector<Riga> Tabella :: seleziona_istanze(const std::string& att, const std::string op, const Value& target) const {
    vector<Riga> tab_filtrata{};
    //ricaviamo l indice della colona in questione, conoscendone la string nome:
    int indice_c = getIndiceColonna(att);
    //assicuriamoci che l attributo esista:
    if(indice_c != -1){
        for(const Riga& r : istanze){//per ogni istanza r in istanze, confrontami il suo valore all attributo in questione di cui abbiamo l indice:
            if(confronta(r.riga[indice_c], op, target)){tab_filtrata.push_back(r);}; 
            //r.riga[indice_c] accede al valore del vector<Value> riga; dell istanza r all indice della colonna/attribuo corrispondente...Che corrisponde appunto a un Value! 
        }    
    }
    return tab_filtrata;
}