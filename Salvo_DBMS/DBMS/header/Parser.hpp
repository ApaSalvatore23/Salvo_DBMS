#pragma once
#include <string>
#include <vector>
#include <sstream>
#include "Tabella.hpp"  // ci serve per Value

//COSA FA IL PARSER? Serve semplicemente a tradurre ed interpretare un input dato dall utente che sia una query, un insert, un create ecc...
// in base a cio vengono richiamate struct e metodi relativi, ad esempio per una query(select) vengono usate la struct ParsedQuery e il metodo query_select per costruire la relativa istanza della struct! 
//Ognuna di queste informazioni elaborate verranno quindi immagazzinate come struct, a seconda del tipo! Ognuna delle quali verra poi eseguita da un Executer.cpp!
//Il Parser si occuperà semplicemente di prendere input e tradurli in qualcosa di operabile dal compilatore. Poi per capire quali metodi devono essere chiamati, quali struct utilizzare a riguardo ecc... 
//useremo un Executer.cpp apposito

//con questa struct posso rappresentare una condizione di WHERE, gestendo nel ParsedQuery la presenza di piu condizioni nel WHERE per una query
struct CondizioneWhere{
    //ogni condizione WHERE come vista in Tabella deve avere un attributo, operatore e un target di confronto:
    std::string attributo;
    std::string op;
    std::string target;
};

//struct che deve contenere tutti gli elementi e info di una query di SELECT interpretata per una certa tabella:
//quindi ogni query sarà un ogetto(struct) in memoria! Che conterra dei dati che verrano elaborati dai metodi successivi!
struct ParsedQuery{
    std::string nomeTabella;//per il FROM
    std::vector<std::string> attrSelect;//per il SELECT
    bool hasWhere = false; //non tutte le query hanno un WHERE di filtro, inizializziamoolo a false
    //Il WHERE lo rappresentiamo come un vector di condizioni (separate eventualmente da un AND/OR):
    std::vector<CondizioneWhere> condizioni_where;
};

//METODI LIBERI: serviranno per fare il parser(traduzione e grammatica) di una query semplice:

//Metodo per "spezzare" una richiesta in input dall utente, in una squenza di caratteri in un vector<string>
std::vector<std::string> build_req(const std::string& input_utente);

//Metodo per "interpretare correttamente" il vector costruito dal metodo precedente
ParsedQuery query_select(const std::vector<std::string>& query_list);

