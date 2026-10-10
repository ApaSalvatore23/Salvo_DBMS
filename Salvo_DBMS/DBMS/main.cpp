#include <iostream>
#include <vector>
#include <string>
#include "Tabella.hpp"
#include "Database.hpp"
#include "Parser.hpp"
#include "Executer.hpp"

int main() {
    std::cout << "=== AVVIO DEL DBMS PERSONALE ===" << std::endl;

    // 1. Creazione del Database
    Database mioDatabase;

    // 2. Definizione della struttura di una Tabella (es. "studenti")
    // Le colonne hanno un nome e un tipo (INT, STRING, DOUBLE)
    std::vector<Colonna> colonne_studenti = {
        {"id", TipoColonna::INT},
        {"nome", TipoColonna::STRING},
        {"media", TipoColonna::DOUBLE}
    };

    // Istanziamo la tabella
    Tabella tabella_studenti("studenti", colonne_studenti);

    // 3. Inserimento di alcune istanze (righe) nella tabella
    // Sfruttiamo il tipo Riga che contiene un vector di Value (variant<int, string, double>)
    tabella_studenti.inserisci_istanza(Riga{{1, std::string("Mario"), 28.5}});
    tabella_studenti.inserisci_istanza(Riga{{2, std::string("Giulia"), 30.0}});
    tabella_studenti.inserisci_istanza(Riga{{3, std::string("Luca"), 24.2}});
    tabella_studenti.inserisci_istanza(Riga{{4, std::string("Elena"), 27.8}});

    // 4. Aggiungiamo la tabella al Database
    mioDatabase.add_tabella(tabella_studenti);

    std::cout << "\n[Info] Tabella 'studenti' creata e inserita nel Database." << std::endl;
    std::cout << "[Info] Tabella completa salvata in memoria:\n" << std::endl;
    
    // Mostriamo la tabella grezza usando il metodo che avevi già implementato
    tabella_studenti.stampa_tabella();

    // 5. Simulazione di una query SQL inserita dall'utente
    // Puoi provare a cambiare questa stringa (es. con o senza WHERE, oppure cambiando i campi)
    std::string stringa_query = "SELECT nome, media FROM studenti WHERE id > 2";
    
    std::cout << "\n----------------------------------------" << std::endl;
    std::cout << "Query inserita dall'utente: \"" << stringa_query << "\"" << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    // FASE 1: Il Parser spezzetta la richiesta testuale in un vettore di stringhe
    std::vector<std::string> parole_richiesta = build_req(stringa_query);

    // FASE 2: Il Parser interpreta la sintassi e costruisce l'oggetto strutturato ParsedQuery
    ParsedQuery query_analizzata = query_select(parole_richiesta);

    // FASE 3: L'Executer prende la query analizzata, interroga il Database e stampa i risultati filtrati
    Executer::esegui_select(query_analizzata, mioDatabase);

    std::cout << "=== FINE ESECUZIONE ===" << std::endl;
    return 0;
}