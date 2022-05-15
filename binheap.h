// Vorzeichenlose ganze Zahl.
using uint = unsigned int;

// Als Binomial-Halde implementierte Minimum-Vorrangwarteschlange
// mit Prioritäten des Typs P und zusätzlichen Daten des Typs D.
// An der Stelle, an der BinHeap für einen bestimmten Typ P verwendet
// wird, muss ein Kleiner-Operator (<) für den Typ P bekannt sein.
// Werte des Typs P dürfen nur mit diesem Operator verglichen werden;
// die übrigen Vergleichsoperatoren (<=, >, >=, ==, !=) dürfen nicht
// verwendet werden.
template <typename P, typename D>
struct BinHeap {
    // Notwendige Vorabdeklaration.
    struct Node;

    // Eintrag einer solchen Warteschlange bzw. Halde, bestehend aus
    // einer Priorität prio mit Typ P und zusätzlichen Daten data mit
    // Typ D.
    // Wenn der Eintrag momentan tatsächlich zu einer Halde gehört,
    // verweist node auf den zugehörigen Knoten eines Binomialbaums
    // dieser Halde; andernfalls ist node ein Nullzeiger.
    struct Entry {
        // Priorität, zusätzliche Daten und zugehöriger Knoten.
        P prio;
        D data;
        Node* node;

        // Initialisierung mit Priorität p und zusätzlichen Daten d.
        Entry (P p, D d) : prio(p), data(d), node(nullptr) {}
    };

    // Knoten eines Binomialbaums innerhalb einer solchen Halde.
    // Neben den eigentlichen Knotendaten (degree, parent, child,
    // sibling), enthält der Knoten einen Verweis auf den zugehörigen
    // Eintrag.
    struct Node {
        // Zugehöriger Eintrag.
        Entry* entry;

        // Grad des Knotens.
        uint degree;

        // Vorgänger (falls vorhanden; Nullzeiger bei einem Wurzelknoten).
        Node* parent;

        // Nachfolger mit dem größten Grad
        // (falls vorhanden; Nullzeiger bei einem Blattknoten).
        Node* child;

        // Zirkuläre Verkettung aller Nachfolger eines Knotens
        // bzw. einfache Verkettung aller Wurzelknoten einer Halde,
        // jeweils sortiert nach aufsteigendem Grad.
        Node* sibling;

        // Initialisierung als Wurzelknoten mit Grad 0 und Eintrag e,
        // der dann auf diesen Knoten zurückverweist.
        Node (Entry* e) : entry(e), degree(0),
                          parent(nullptr), child(nullptr), sibling(nullptr) {
            e->node = this;
        }
    };

    // Anfang der Wurzelliste (Nullzeiger bei einer leeren Halde).
    Node* head;

    // Initialisierung als leere Halde.
    BinHeap (){
        head = nullptr;
    }

    // Ist die Halde momentan leer?
    bool isEmpty (){
        return head == nullptr;
    }

    // Größe der Halde, d. h. Anzahl momentan gespeicherter Einträge
    // liefern.
    uint size (){ ///TODO TESTING
        // Var. für das Zählen der Einträge.
        uint s = 0;
        // Pointer auf den aktuellen Wurzelknoten
        Node* n = head;
        // Iteration über die Wurzelliste.
        // Bedingung ist, dass der Grad des Siblings des aktuellen Knotens kleiner als der Grad des aktuellen Knotens ist.
        while (n != nullptr){
            // Der Formel aus der Vorlesung folgend (Folie 101):
            s += 2^n->degree;
            // neues n ist sibling.
            n = n->sibling;
        }
        return s;
    }

    // Neuen Eintrag mit Priorität p und zusätzlichen Daten d erzeugen,
    // zur Halde hinzufügen und zurückliefern.
    Entry* insert (P p, D d){
        // Neuen Eintrag erzeugen.
        Entry entry = new Entry(p, d);
        Node node = new Node(entry);
        heapmerge(head, node);
        return &entry;
    }

    // HILFSFUNKTION: swapnode swappt zwei Knoten.
    void swapnode(Node* a, Node* b){
        Node* swap = a;
        a = b;
        b = swap;
    }
    // HILFSFUNKTION: treecount Gibt die anzahl der Bäume in der Halde zurück.
    uint treecount(Node* a){
        uint count = 0;
        Node* n = a;
        while (n != nullptr){
            count++;
            n = n->sibling;
        }
        return count;
    }

    // HILFSFUNKTION: treeinsert, fügt einen Baum am Ende der Halde ein.
    void treeinsert(Node* a, Node* b){
        if (a == nullptr){
            a = b;
        }
        else{
            Node* n = a;
            while (n->sibling != nullptr){
                n = n->sibling;
            }
            n->sibling = b;
        }
    }

    // HILFSFUNKTION: treemerge, vereinigt zwei Bäume und liefert eine neue halde zurück.
    Node* treemerge(Node* a, Node* b){
        // Ist die Priorität des ersten Baumes(a) Größer als die des zweiten Baumes(b)?
        if (b->entry->prio < a->entry->prio) {
            // Wenn ja wird a und b getauscht.
            // Da ja die Priorität von a größer ist muss a teilbaum von b werden.
            swapnode(a, b);
        }
        // Führe nun Merge aus.
        Node* temp = a->child;
        a->child = b;
        b->parent = a;
        b->sibling = temp;
        while (temp->degree!=0){
            temp = temp->sibling;
        }
        temp->sibling = b;
        // Degree des neuen Wurzelknotens wird um 1 erhöht.
        a->degree++;
        return a;
    }

    // HILFSFUNKTION: heapmerge, vereinigt zwei Halden und liefert eine neue halde zurück.
    Node* heapmerge (Node* a, Node* b){
        //Hilfvariablen
        Node* temp = nullptr;
        Node* c = nullptr;
        uint k = 0;
        // Solange a und b nicht leer sind.
        while (a!=nullptr || b!=nullptr){
            // Ist a leer? (prüfen wegen fehler sonst mit degree)
            if (a!=nullptr) {
                // degree ist k?
                if (a->degree == k) {
                    // Wenn ja a an temp anhängen.
                    treeinsert(temp, a);
                    // a iterieren.
                    a = a->sibling;
                }
            }
            // Ist b leer? (prüfen wegen fehler sonst mit degree)
            if (b!=nullptr) {
                // degree ist k?
                if (b->degree == k) {
                    // Wenn ja b an temp anhängen.
                    treeinsert(temp, b);
                    // b iterieren.
                    b = b->sibling;
                }
            }
            // Zahl der Bäume in Temp == 1?
            if (treecount(temp) == 1) {
                // Wenn ja, dann temp an c anhängen.
                treeinsert(c, temp);
                // temp leeren.
                temp = nullptr;
            }
            // Zahl der Bäume in Temp == 3?
            if (treecount(temp) == 3) {
                // Wenn ja, dann dritten Baum in Temp an c anhängen.
                treeinsert(c, temp->sibling->sibling);
                // Dritter Baum in Temp löschen.
                temp->sibling->sibling = nullptr;
            }
            // Zahl der Bäume in Temp == 2?
            if (treecount(temp) == 2) {
                // Wenn ja, dann Beide Bäume Mergen
                temp = treemerge(temp, temp->sibling);
            }
            // k um 1 erhöhen.
            k++;
        }
        return c;
    }

    // Eintrag mit minimaler Priorität liefern.
    // (Nullzeiger bei einer leeren Halde.)
    Entry* minimum (){
        if(isEmpty()) return nullptr; // falls leer, dann nullptr zurückgeben.
        else { // sonst, dann den Eintrag mit minimaler Priorität suchen und zurückgeben.
            Node *n = head;
            Node n_mem = nullptr;
            while (n != nullptr){
                if(n[0] < n_mem[0] || n_mem == nullptr){
                    n_mem = n;
                }
                n=n->sibling;
            }
            return n_mem;
        }
    }

    // Eintrag mit minimaler Priorität liefern
    // und aus der Halde entfernen (aber nicht freigeben).
    // (Bei einer leeren Halde wirkungslos mit Nullzeiger als Resultatwert.)
    Entry* extractMin (){ ///TODO REFINEMENT
        if(isEmpty()) return nullptr; //wenn Halde leer ist
        else{ //wenn Halde nicht leer ist
            Node min=minimum(); //minimum wird gesucht und in min gespeichert!
            Node min_mem=min; //min_mem merkt sich min (um min zu löschen)
            Node min_parent=min->parent; //min_parent ist der parent von min.
            Node min_child=min->child; //min_child ist der child von min.
            min.remove(); //min wird aus der Halde entfernt
            if(min_child != nullptr){ //wenn a ein Nachfolger hatte
                heapmerge(min_parent, min_child); //wenn a nur ein Kind hat, dann wird aus der Halde entfernt.
            }
            return min_mem; //a_mem wird zurückgegeben
        }
    }

    // Enthält die Halde den Eintrag e?
    // Resultatwert false, wenn e ein Nullzeiger ist.
    bool contains (Entry* e){
        if(e == nullptr) return false; //ist e ein Nullzeiger?
        else{ //sonst
            Node *n = head; //n wird auf head gesetzt
            while(n != nullptr){ //solange n nicht null ist
                for (int i = 0; i < sizeof* n; i++) { //für alle elemente von n
                    if(n[i]->data == e->data) return true; //wenn der Eintrag e gefunden wurde!
                }
                n = n->sibling; //n wird auf nächsten sibling gesetzt
            }
            return false; //wenn Eintrag e nicht gefunden wurde
        }
    }

    // Priorität des Eintrags e auf p ändern.
    // Hierbei darf auf keinen Fall ein neues Entry-Objekt entstehen,
    // selbst wenn die Operation intern als Entfernen und Neu-Einfügen
    // implementiert wird.
    // (Wirkungslos mit Resultatwert false, wenn e ein Nullzeiger ist
    // oder e nicht zur aktuellen Halde gehört; sonst Resultatwert true.)
    bool changePrio (Entry* e, P p){
        if(e == nullptr || !(contains(e))){ //wenn e ein Nullzeiger ist oder e nicht zur aktuellen Halde gehört
            return false; //wenn nicht, dann false zurückgeben
        }
        //Ansonsten Priorität ändern
        else {
            // Setze die Priorität des Objekts auf die neue Priorität
            e->prio = p;
            //Wenn die neue Priorität kleiner oder gleich der alten Priorität ist
            if(e->prio <= e->parent->prio){
                //Solange die Priorität des Objekts kleiner oder gleich der alten Priorität ist
                while(e->prio <= e->parent->prio) {
                    //Vertausche das Objekt mit seinem Vorgänger
                    swap(e, e->parent);
                }
            }
            //Sofern sich das Objekt nicht in einem Blattknoten befindet
            else if(e->parent->child != nullptr){
                //Entferne das Objekt
                e->remove();
                //Füge das Objekt mit der neuen Priorität wieder ein
                //TODO: Objekt einfügen
            }
            //true zurückgeben
            return true;
        }
    }

    // Eintrag e aus der Halde entfernen (aber nicht freigeben).
    // (Wirkungslos mit Resultatwert false, wenn e ein Nullzeiger ist
    // oder e nicht zur aktuellen Halde gehört; sonst Resultatwert true.)
    bool remove (Entry* e){
        if(e != nullptr || !(contains(e))){
            return false;
        }
        else {
            //entnimm das Objekt mit minimaler Priorität
            Entry *min = extractMin(e);
            //Wenn das entnommende Objekt nicht das zu entfernde Objekt ist
            if (min != e) {
                //Ändere die Priorität des Objekts auf die Priorität des entnommenen Objekts
                min->prio = e->prio;
                //TODO: Verbinde das entnommene Objekt mit dem Knoten des zu entfernenden Objekts

                //Setze die Priorität des zu entfernden Objekts wieder auf ihren ursprünglichen Wert
                e->prio = e->parent->prio;


                return true;
            }
        }
    }

    // Inhalt der Halde zu Testzwecken ausgeben.
    void dump (){
        //TODO: Ausgabe, Testen ob Halde leer ist
        // Inhalt der Halde ausgeben



    }
};