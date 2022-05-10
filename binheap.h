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
        merge(node, head);
        return &entry;
    }

    // HILFSFUNKTION: swapnode swappt zwei Knoten.
    void swapnode(Node* a, Node* b){
        Node* swap = a;
        a = b;
        b = swap;
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
        // Degree des neuen Wurzelknotens wird um 1 erhöht.
        a->degree++;
        return a;
    }

    // HILFSFUNKTION: heapmerge, vereinigt zwei Halden und liefert eine neue halde zurück.
    Node* heapmerge (Node* a, Node* b){
        // Nodepointer als Hilfsvariable. Wertzuweisung mit aktuellem Wurzelknoten von a.
        Node* curr = nullptr;
        curr = a;
        // durchlaufen von a bis letztem sibling.
        while (curr->sibling != nullptr){
            curr = curr->sibling;
        }
        // Verkettung von a und b.
        curr->sibling = b;
        // Sortieren der Halde. a die nun aus a und b vereinigt besteht.
        degrsort(a);
        // setze curr auf Wurzelknoten von a.
        curr = a;
        // weitere Hilfsvariablen.
        Node* next = curr->sibling;
        Node* nextnext = nullptr;
        Node* prev = nullptr;
        // durchlaufen von a bis letztem sibling.
        while (curr->sibling != nullptr){
            // Wenn Grad zweier Bäume Identisch ist.
            if (curr->sibling->degree == curr->degree){ // Wenn ja, dann merge.
                // Hilfsvariablen.
                nextnext = next->sibling;
                // Merge.
                curr = treemerge(curr, next);
                // Wenn prev null ist, dann ist curr die neue Wurzel.
                // Ansonsten wird curr zu prevsibling.
                if (prev != nullptr) prev->sibling = curr;
                else a = curr;
                // Sibling von curr wird aktualisiert.
                curr->sibling = nextnext;
                // next aktualisieren.
                next = nextnext;
            } else { // Nur wenn nicht gemerged wird, wird im Baum weitergegangen.
                prev = curr;
                curr = next;
                next = next->sibling;
            }
        }
        return a;
    }

    // HILFSFUNKTION: degrsort, sortiert die Halde nach grad.
    void degrsort (Node* n){ ///TODO REFINEMENT
        Node* prev = nullptr;
        Node* curr = n;
        Node* next = curr->sibling;
        while (next != nullptr){
            if (next->degree < curr->degree){
                if (prev != nullptr) prev->sibling = next;
                curr->sibling = next->sibling;
                next->sibling = curr;
                prev = nullptr;
                curr = n;
                next = curr->sibling;
            }
            else {
                prev = curr;
                curr = next;
                next = next->sibling;
            }
        }
    }

    // Eintrag mit minimaler Priorität liefern.
    // (Nullzeiger bei einer leeren Halde.)
    Entry* minimum (){
        return nullptr; // pseudoreturn
    }

    // Eintrag mit minimaler Priorität liefern
    // und aus der Halde entfernen (aber nicht freigeben).
    // (Bei einer leeren Halde wirkungslos mit Nullzeiger als Resultatwert.)
    Entry* extractMin (){
        return nullptr; // pseudoreturn
    }

    // Enthält die Halde den Eintrag e?
    // Resultatwert false, wenn e ein Nullzeiger ist.
    bool contains (Entry* e){
        return false; // pseudoreturn
    }

    // Priorität des Eintrags e auf p ändern.
    // Hierbei darf auf keinen Fall ein neues Entry-Objekt entstehen,
    // selbst wenn die Operation intern als Entfernen und Neu-Einfügen
    // implementiert wird.
    // (Wirkungslos mit Resultatwert false, wenn e ein Nullzeiger ist
    // oder e nicht zur aktuellen Halde gehört; sonst Resultatwert true.)
    bool changePrio (Entry* e, P p){
        return false; // pseudoreturn
    }

    // Eintrag e aus der Halde entfernen (aber nicht freigeben).
    // (Wirkungslos mit Resultatwert false, wenn e ein Nullzeiger ist
    // oder e nicht zur aktuellen Halde gehört; sonst Resultatwert true.)
    bool remove (Entry* e){
        return false; // pseudoreturn
    }

    // Inhalt der Halde zu Testzwecken ausgeben.
    void dump (){
        // ...
    }
};