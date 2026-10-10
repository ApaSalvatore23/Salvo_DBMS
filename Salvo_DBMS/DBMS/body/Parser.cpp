#include <string>
#include <vector>
#include <sstream>
#include "Tabella.hpp"  // ci serve per Value
#include "Parser.hpp"

//COSA FA IL PARSER? Serve semplicemente a tradurre ed interpretare un input dato dall utente che sia una query, un insert, un create ecc...
// in base a cio vengono richiamate struct e metodi relativi, ad esempio per una query(select) vengono usate la struct ParsedQuery e il metodo query_select per costruire la relativa istanza della struct! 
//Ognuna di queste informazioni elaborate verranno quindi immagazzinate come struct, a seconda del tipo! Ognuna delle quali verra poi eseguita da un Executer.cpp!
//Il Parser si occuperà semplicemente di prendere input e tradurli in qualcosa di operabile dal compilatore. Poi per capire quali metodi devono essere chiamati, quali struct utilizzare a riguardo ecc... 
//useremo un Executer.cpp apposito



//IMPLEMENTAZIONE METODI LIBERI: serviranno per fare il parser(traduzione e grammatica) di un input:

//Metodo per "spezzare" l input, in una squenza di caratteri in un vector<string>...Potra essere una query select, un insert, create ecc...!
std::vector<std::string> build_req(const std::string& input_utente){
    std::vector<std::string> string_list{};
    std::stringstream ss(input_utente);
    std::string parola;
    while(ss >> parola){
        string_list.push_back(parola);
    }
    return string_list;
}

//--NOTA!: qui gestiamo al momento solo query, quindi firmate col SELECT!---
//Metodo per "interpretare correttamente" il vector costruito dal metodo precedente, e dare un senso logico lla query di select(proiezione):
ParsedQuery query_select(const std::vector<std::string>& query_list){
    //costuriamo il nostro ogetto ParsedQuery:
    ParsedQuery q;
    //facciamo un for e troviamo gli indici del SELECT, FROM ecc...per orientarci successivamente:
    int idx_select = -1;
    int idx_from = -1;
    int idx_where = -1; 
    for(int i=0; i<query_list.size(); ++i){
        if(query_list[i] == "SELECT"){idx_select = i;}
        if(query_list[i] == "FROM"){idx_from = i;}
        if(query_list[i] == "WHERE"){idx_where = i;}
    }
    //ora possiamo orientarci e capire parametri e attributi della query, assegnando i rispettivi alla nostra ParsedQuery:
    q.attrSelect.assign(query_list.begin() + idx_select+1, query_list.begin() + idx_from); //con assign, passiamo al vector attrSelect della nostra ParsedQuery i valori che trova dall indice del select all indice del from
    for(auto& col : q.attrSelect){//piccolo for per eliminare le virgole ',' dal vector attrSelect:
        if(!col.empty() && col.back() == ','){ col.pop_back(); }
    } 
    q.nomeTabella = query_list[idx_from+1]; //per ora escludiamo la presenza di JOIN...
    if(idx_where == -1){q.hasWhere = false;} //se nn aveva un WHERE fermiamoci qui.
    else{q.hasWhere = true;}
    //altrimenti controllo e procedo con i relativi suoi attirbuti:
    if(q.hasWhere == true){
        int i=idx_where+1;//iniziamo nel while da subito dopo il WHERE
        //per ogni condizione del WHERE, creiamo una Condizione(struct):
        while(i < query_list.size()){
            CondizioneWhere c;
            //ad ogni aggiunta, andiamo avanti con l indice:
            c.attributo = query_list[i]; i++;
            c.op = query_list[i]; i++;
            c.target = query_list[i]; i++;
            //appena incontra un AND/OR, finisce questa condizione e va alla prossima eventuale (controlliamo sempre di essere per sicurezza dentro i limiti):
            if(i < query_list.size() && (query_list[i] == "AND" || query_list[i] == "OR")){i++;}
            //inseriamo la condizione costruita nel vector<CondizioneWhere> della nostra ParsedQuery:
            q.condizioni_where.push_back(c);
        }
    }
    //una volta finito, restituiamo la ParsedQuery costruita:
    return q;
}