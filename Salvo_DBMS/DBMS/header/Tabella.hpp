#pragma once // Evita che l'header venga incluso più volte nello stesso file
#include <string>
#include <vector>
#include <variant>

// FIRME DEI METODI E PARAMETRI (HEADER):


// Variant per gestire i tipi supportati nel DBMS
using Value = std::variant<int, std::string, double>; //Value lo creiamo noi come Alias di variant<int, string, double>

// Tipi di dati ammessi per ogni colonna
enum class TipoColonna { INT, STRING, DOUBLE };

// Strutture e Tipi di Dati per la Tabella

// Rappresenta un attributo della tabella
struct Colonna {
    std::string nome;
    TipoColonna tipo;
};

// Rappresenta un record/istanza (vettore di valori)
struct Riga {
    std::vector<Value> riga{}; //Un vector di <string/int/double> in base ai tipi di dato che contiene
    //cosi ogni vector di ogni istanza puo contenere piu dati di tipo diverso! 
};


class Tabella {
protected:
    std::string nomeTab;
    std::vector<Colonna> attributi{};
    std::vector<Riga> istanze{}; //un vector di Riga, quindi un vector doppio di vector<Value>....istanze sara quinid il vector appunto di righe/istanze della Tabella! 

    //Metodo privato di supporto per verificare la compatibilità dei tipi tra un valore di un istanza e il relativo attributo:
    bool controlla(const Value& val_riga, TipoColonna attributo) const;
    //metodo privato di supporto per i confronti del metodo pubblico 'filtra_istanze': 
    bool confronta(const Value& att, std::string op, const Value& target) const;   //val_att "<,=,>" target
    

public:
    // Costruttori
    Tabella() = default;
    Tabella(std::string n, std::vector<Colonna> a);

    // Operazioni principali sulla tabella
    bool inserisci_istanza(const Riga& r);
    int getIndiceColonna(const std::string& nomeC) const;
    void stampa_tabella() const;

    // Getter di sola lettura
    std::string getNome() const;
    const std::vector<Colonna>& getAttributi() const;
    const std::vector<Riga>& getIstanze() const;

    //Metodi che serviranno per il Query Engine:-------------------------------------------------------------------------------
    
    //Metodo di supporto per il SELECT:
    std::vector<Riga> seleziona_attributi(const std::vector<Riga>& tab_operazione, const std::vector<std::string>& att) const;
    //Metodo di supporto per il WHERE:
    std::vector<Riga> seleziona_istanze(const std::string& att, const std::string op, const Value& target) const;
}; 