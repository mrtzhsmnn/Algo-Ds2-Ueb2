#include <iostream>
#include <string>
using namespace std;

#include "binheap.h"

// Interaktives Testprogramm fÃ¼r die Klasse BinHeap.
int main (int argc, char** argv) {
    // Wenn ein beliebiges Kommandozeilenargument angegeben ist,
    // werden die eingegeben Befehle auch ausgegeben.
    bool echo = argc > 1;

    // Halde mit PrioritÃ¤ten des Typs string und zugehÃ¶rigen Daten des
    // Typs int.
    // (Die Implementierung muss aber natÃ¼rlich auch mit anderen
    // Typen funktionieren.)
    // Der Kleiner-Operator fÃ¼r string implementiert einen
    // lexikographischen Vergleich.
    BinHeap<string, int> heap;

    // Feld mit allen eingefÃ¼gten EintrÃ¤gen, damit sie spÃ¤ter
    // fÃ¼r remove und changePrio verwendet werden kÃ¶nnen.
    BinHeap<string, int>::Entry* entrys [100];

    // Anzahl der bis jetzt eingefÃ¼gten EintrÃ¤ge.
    uint n = 0;

    // Endlosschleife.
    while (true) {
        // Inhalt und GrÃ¶ÃŸe der Halde ausgeben.
        heap.dump();
        cout << heap.size() << " entry(s)" << endl;

        // Befehl von der Standardeingabe lesen
        // und die entsprechende Operation auf der Halde ausfÃ¼hren.
        string cmd, prio;
        uint entry;
        BinHeap<string, int>::Entry* e;

        cout << "cmd: ";
        cin >> cmd;
        if (echo) cout << cmd;

        switch (cmd[0]) {
            case '+': // insert prio
                // Die laufende Nummer n wird als zusÃ¤tzliche Daten
                // verwendet.
                cin >> prio;
                if (echo) cout << " " << prio << endl;
                entrys[n] = heap.insert(prio, n);
                n++;
                break;
            case '-': // remove entry
                cin >> entry;
                if (echo) cout << " " << entry << endl;
                heap.remove(entrys[entry]);
                break;
            case '?': // minimum
                e = heap.minimum();
                if (echo) cout << endl;
                cout << "minimum: " << e->prio << " " << e->data << endl;
                break;
            case '!': // extractMin
                e = heap.extractMin();
                if (echo) cout << endl;
                cout << "minimum: " << e->prio << " " << e->data << endl;
                break;
            case '=': // changePrio entry prio
                cin >> entry >> prio;
                if (echo) cout << " " << entry << " " << prio << endl;
                heap.changePrio(entrys[entry], prio);
                break;
            case 'q': // quit
                if (echo) cout << endl;
                return 0;
        }
    }
}