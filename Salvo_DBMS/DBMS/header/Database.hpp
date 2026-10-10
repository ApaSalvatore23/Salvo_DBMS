#pragma once
#include <algorithm>
#include <string>
#include <vector>
#include <variant>
#include <iostream>
#include<map>
#include "Tabella.hpp"

// FIRME DEI METODI E PARAMETRI (HEADER):
class Database{
    protected:
       //il DB sara modellato internamente come una Map
        std::map<std::string, Tabella> TabelleBD; //ogni nome(string) è associato a una Tabella
    public:
        //costruttore di un DB:
        Database();

        //inserimento di Tabelle(non gia esistenti):
        bool add_tabella(const Tabella& T);

        //Rimozione di Tabelle(se esistenti):
        bool drop_tabella(std::string nome_tab);

        //per ottenere il puntatore in memoria ad una Tabella...Sarà di supporto per il FROM:
        Tabella* getTabella(std::string nome_tab);

        //per mostrare le Tabelle presenti nel DB:
        void mostra_tabelle() const;
}; 
 

