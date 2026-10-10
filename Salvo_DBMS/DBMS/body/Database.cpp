#include <algorithm>
#include <string>
#include <vector>
#include <variant>
#include <iostream>
#include<map>
#include "Tabella.hpp"
#include "Database.hpp"

//costruttore:
Database :: Database(){}

//metodo di inserimento:
bool Database :: add_tabella(const Tabella& T){
    //controlliamo se esiste gia nel DB:
        if(TabelleBD.find(T.getNome()) != TabelleBD.end()){std::cout<<"Tabella con quel nome gia esistente!"<<std::endl; return false;}
        //altrimenti aggiungo:
        TabelleBD[T.getNome()] = T;
        return true;
}

 //Rimozione di Tabelle(se esistenti):
bool Database :: drop_tabella(std::string nome_tab){
    //se non esiste non eseguiamo nulla:
    if(TabelleBD.find(nome_tab) == TabelleBD.end()){std::cout<<"Tabella inesistente!"<<std::endl; return false;}
    //altrimenti la rimuoviamo:
    TabelleBD.erase(nome_tab);
    return true;
}

//per ottenere il puntatore in memoria ad una Tabella...Sarà di supporto per il FROM::
Tabella* Database :: getTabella(std::string nome_tab){
    //controlliamo se esiste:
    auto it = TabelleBD.find(nome_tab); //it punta all eventuale tabella
    if(it == TabelleBD.end()){std::cout<<"Tabella inesistente!"<<std::endl; return nullptr;}
    //altrimenti:
    return &(it->second); //accediamo al second dereferenziato dell iteratore, ovvero la Tabella in questione, e ci returniamo l indirizzo.
}

//per mostrare le Tabelle presenti nel DB (solo come un elenco di nomi):
void Database :: mostra_tabelle() const {
    for(const auto& T : TabelleBD){
        std::cout<<T.first<<std::endl;
    }
}
