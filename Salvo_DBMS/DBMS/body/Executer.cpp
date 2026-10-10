#include "Executer.hpp"
#include <iostream>
#include <variant>

void Executer::esegui_select(const ParsedQuery& q, Database& db) {
    // 1. Trova la tabella nel Database tramite il puntatore
    Tabella* t = db.getTabella(q.nomeTabella);
    if (!t) {
        return; // Se la tabella non esiste, getTabella stampa già il messaggio di errore
    }

    // 2. Ottieni le righe di partenza (filtrate dal WHERE oppure tutte)
    std::vector<Riga> righe_lavorazione;

    if (!q.hasWhere || q.condizioni_where.empty()) {
        righe_lavorazione = t->getIstanze();
    } else {
        // Gestiamo la condizione del WHERE (prendiamo la prima per semplicità)
        const auto& c = q.condizioni_where[0];
        
        // Conversione del target stringa nel tipo corretto in base alla colonna della tabella
        Value target_val = c.target; // Default stringa
        int idx_col = t->getIndiceColonna(c.attributo);
        
        if (idx_col != -1) {
            const auto& atts = t->getAttributi();
            if (atts[idx_col].tipo == TipoColonna::INT) {
                try {
                    target_val = std::stoi(c.target);
                } catch (...) {}
            } else if (atts[idx_col].tipo == TipoColonna::DOUBLE) {
                try {
                    target_val = std::stod(c.target);
                } catch (...) {}
            }
        }

        righe_lavorazione = t->seleziona_istanze(c.attributo, c.op, target_val);
    }

    // 3. Applica la proiezione delle colonne (SELECT attr1, attr2... oppure '*')
    std::vector<Riga> righe_finali;
    if (q.attrSelect.size() == 1 && q.attrSelect[0] == "*") {
        righe_finali = righe_lavorazione;
    } else {
        righe_finali = t->seleziona_attributi(righe_lavorazione, q.attrSelect);
    }

    // 4. Stampa formattata del risultato a schermo
    std::cout << "\n=== RISULTATO QUERY ===" << std::endl;
    
    // Stampa intestazioni
    if (q.attrSelect.size() == 1 && q.attrSelect[0] == "*") {
        for (const auto& att : t->getAttributi()) {
            std::cout << att.nome << "\t| ";
        }
    } else {
        for (const auto& col_name : q.attrSelect) {
            std::cout << col_name << "\t| ";
        }
    }
    std::cout << "\n----------------------------------------" << std::endl;

    // Stampa i record riga per riga usando std::holds_alternative e std::get
    for (const auto& r : righe_finali) {
        for (const auto& val : r.riga) {
            if (std::holds_alternative<int>(val)) {
                std::cout << std::get<int>(val) << "\t| ";
            } else if (std::holds_alternative<std::string>(val)) {
                std::cout << std::get<std::string>(val) << "\t| ";
            } else if (std::holds_alternative<double>(val)) {
                std::cout << std::get<double>(val) << "\t| ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "========================================\n" << std::endl;
}