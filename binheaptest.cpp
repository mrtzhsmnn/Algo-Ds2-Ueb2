#include <iostream>
#include <string>
using namespace std;

#include "binheap.h"

// Interaktives Testprogramm für die Klasse BinHeap.
int main (int argc, char** argv) {
    // Wenn ein beliebiges Kommandozeilenargument angegeben ist,
    // werden die eingegebenen Befehle auch ausgegeben.
    bool echo = argc > 1;

    // Halde mit Prioritäten des Typs string und zugehörigen Daten des
    // Typs int.
    // (Die Implementierung muss aber natürlich auch mit anderen
    // Typen funktionieren).
    // Der Kleiner-Operator für string implementiert einen
    // lexikografischen Vergleich.
    BinHeap<string, int> heap;

    // Feld mit allen eingefügten Einträgen, damit sie später
    // für remove und changePrio verwendet werden können.
    BinHeap<string, int>::Entry* entrys [100];

    // Anzahl der bis jetzt eingefügten Einträge.
    uint n = 0;

    // Endlosschleife.
    while (true) {
        // Inhalt und GröÃŸe der Halde ausgeben.
        heap.dump();
        cout << heap.size() << " entry(s)" << endl;

        // Befehl von der Standardeingabe lesen
        // und die entsprechende Operation auf der Halde ausführen.
        string cmd, prio;
        uint entry;
        BinHeap<string, int>::Entry* e;

        cout << "cmd: ";
        cin >> cmd;
        if (echo) cout << cmd;

        switch (cmd[0]) {
            case '+': // insert prio.
                // Die laufende Nummer n wird als zusätzliche Daten
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