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
        return (head == nullptr);
    }

    // Größe der Halde, d. h. Anzahl momentan gespeicherter Einträge
    // liefern.
    uint size (){
        // Var. für das Zählen der Einträge.
        uint s = 0;
        // Pointer auf den aktuellen Wurzelknoten
        Node* n = head;
        // Iteration über die Wurzelliste.
        // Bedingung ist, dass der Grad des Siblings des aktuellen Knotens kleiner als der Grad des aktuellen Knotens ist.
        while (n != nullptr){
            // Der Formel aus der Vorlesung folgend (Folie 101):
            s = s + (1 << n->degree);
            // neues n ist sibling.
            n = n->sibling;
        }
        return s;
    }

    // Neuen Eintrag mit Priorität p und zusätzlichen Daten d erzeugen,
    // zur Halde hinzufügen und zurückliefern.
    Entry* insert (P p, D d){
        // Neuen Eintrag erzeugen.
        Entry* entry = new Entry(p, d);
        Node* node = new Node(entry);
        // Wenn die Halde leer ist, ist der neue Knoten die Wurzel.
        if (isEmpty()) head = node;
        // Wenn nicht, dann Heapmerge
        else head=heapmerge(head, node);
        return entry;
    }

    // Neuen Eintrag mit Priorität p und zusätzlichen Daten d erzeugen,
    // zur Halde hinzufügen und zurückliefern.
    Entry* helpinsert (Entry* entry, Node* node){
        // Wenn die Halde leer ist, ist der neue Knoten die Wurzel.
        if (isEmpty()) head = node;
            // Wenn nicht, dann Heapmerge
        else head=heapmerge(head, node);
        return entry;
    }


    // HILFSFUNKTION: treeinsert, fügt einen Baum am Ende der Halde ein.
    Node* treeinsert(Node* a, Node* b){
        // a nullptr?
        if (a == nullptr){
            // a wird zu b
            a = b;
        }
        else{
            Node* n = a;
            // While zum finden des letzten Knotens
            while (n->sibling != nullptr){
                n = n->sibling;
            }
            // Sibling von n wird b
            n->sibling = b;
            n->sibling->sibling = nullptr;
        }
        return a;
    }


    // HILFSFUNKTION: treemerge, vereinigt zwei Bäume und liefert eine neue halde zurück.
    Node* treemerge(Node* a, Node* b){
        // Anfängliche Prüfung:
        if (a == nullptr && b == nullptr) return nullptr;
        if (a==nullptr) return b;
        if (b==nullptr) return a;
        // PRÜFUNG ANALOG ZUR FOLIE:
        if (b->entry->prio < a->entry->prio) {
            b->sibling = nullptr;
            b->degree = b->degree + 1;
            a->parent = b;
            if (b->child == nullptr){
                b->child = a;
                a->sibling = a;
            }
            else {
                a->sibling = b->child->sibling;
                b->child->sibling = a;
                b->child = a;

            }
            return b;
        }
        else{
            a->sibling = nullptr;
            a->degree = a->degree + 1;
            b->parent = a;
            if (a->child == nullptr){
                a->child = b;
                b->sibling = b;
            }
            else {
                b->sibling = a->child->sibling;
                a->child->sibling = b;
                a->child = b;
            }
            return a;
        }
    }


    // HILFSFUNKTION: heapmerge, vereinigt zwei Halden und liefert eine neue halde zurück.
    Node* heapmerge (Node* a, Node* b){
        if (a == nullptr && b == nullptr) return nullptr;
        if (a==nullptr) return b;
        if (b==nullptr) return a;
        //Hilfvariablen
        Node* c = nullptr;
        uint k = 0;
        uint i = 0;
        Node** mem = new Node*[3]();
        // Solange a und b nicht leer sind.
        while (a!=nullptr || b!=nullptr || i!=0){
            // Ist a leer? (prüfen wegen fehler sonst mit degree)
            if (a!=nullptr) {
                // degree ist k?
                if (a->degree == k) {
                    // Wenn ja a an mem anhängen.
                    mem[i] = a;
                    i++;
                    // a iterieren.
                    a = a->sibling;
                }
            }
            // Ist b leer? (prüfen wegen fehler sonst mit degree)
            if (b!=nullptr) {
                // degree ist k?
                if (b->degree == k) {
                    // Wenn ja b an mem anhängen.
                    mem[i] = b;
                    i++;
                    // b iterieren.
                    b = b->sibling;
                }
            }
            // Zahl der Bäume in Temp == 1?
            if (i == 1) {
                // Wenn ja, dann mem[0] an c anhängen.
                c = treeinsert(c, mem[0]);
                // mem leeren.
                mem[0] = nullptr;
                i--;
            }
            // Zahl der Bäume in Temp == 3?
            if (i == 3) {
                // Wenn ja, dann ersten Baum in Temp an c anhängen.
                c = treeinsert(c, mem[0]);
                // erster Baum in mem löschen
                mem[0] = mem[1];
                mem[1] = mem[2];
                mem[2] = nullptr;
                i--;
            }
            // Zahl der Bäume in Temp == 2?
            if (i == 2) {
                // Wenn ja, dann Beide Bäume Mergen
                mem[0] = treemerge(mem[0], mem[1]);
                mem[1] = nullptr;
                i--;
            }
            // k um 1 erhöhen.
            k++;
        }
        return c;
    }

    // Eintrag mit minimaler Priorität liefern.
    // (Nullzeiger bei einer leeren Halde.)
    Entry* minimum (){
        // falls Halde leer, dann nullptr zurückgeben.
        if(isEmpty()) return nullptr;
        else {
            // sonst, dann den Eintrag mit minimaler Priorität suchen und zurückgeben.
            Node *n = head;
            Node *n_mem = nullptr;
            // durchlaufe die Liste der Wurzelknoten
            while (n != nullptr){
                // Wenn n_mem leer oder prio Größer als prio von n:
                if(n_mem == nullptr || n->entry->prio < n_mem->entry->prio){
                    // dann n_mem auf n setzen.
                    n_mem = n;
                }
                // n weiterlaufen.
                n=n->sibling;
            }
            // Eintrag von n_mem zurückgeben.
            return n_mem->entry;
        }
    }


    // Funktion extractMin() zum extrahieren von des Minimums
    Entry* extractMin(){
        // falls leer -> nullptr zurückgeben
        if(isEmpty()) return nullptr;
        Entry* retmin = minimum();
        // min = minimum node
        Node* min = retmin->node;
        // Hilfsvariablen
        Node* prev = head;
        Node* temp = nullptr;
        // nachfolgervariable auf nachfolger von min
        Node* post = min->sibling;
        // While zur des Vorgängers von min
        while((prev->sibling != nullptr) && (prev->sibling!=min)){
            prev = prev->sibling;
        }
        // ist min der erste Knoten?
        if (min == head){
            // dann setze head auf den nachfolger von min
            head = post;
        }
        // wenn nicht, dann setze prev->sibling auf den nachfolger von min
        else prev->sibling = post;
        min->sibling = nullptr;
        // sich um die Kinder Kümmern:
        if (min->child != nullptr){
            temp = min->child->sibling;
            min->child->sibling = nullptr;
            min = temp;
            while(temp!= nullptr){
                temp->parent = nullptr;
                temp = temp->sibling;
            }
            head = heapmerge(head, min);
        }
        return retmin;
    }


    // Enthält die Halde den Eintrag e?
    // Resultatwert false, wenn e ein Nullzeiger ist.
    bool contains (Entry* e){
        // ist e ein Nullzeiger?
        if(e == nullptr) return false;
        // wenn nicht:
        else{
            // Nodepointer n wird auf den Knoten von e gesetzt.
            Node* n = e->node;
            // Solange der Vaterknoten von n nicht nullptr ist.
            while(n->parent != nullptr) {
                // n wird auf den Vaterknoten von n gesetzt.
                n = n->parent;
            }
            // Nodepointer p wird auf den Kopf der Halde gesetzt.
            Node* p = this->head;
            // Solange p nicht nullptr ist.
            while (p != nullptr){
                // Ist der Knoten von p gleich dem Knoten von n?
                if(p==n) return true; // wenn ja, dann true zurückgeben.
                // Halde weiter durchlaufen.
                p=p->sibling;
            }
            // falls der wurzelknoten zum eintrag nicht gefunden wurde, dann false zurückgeben.
            return false;
        }
    }


    // Priorität des Eintrags e auf p ändern.
    // Hierbei darf auf keinen Fall ein neues Entry-Objekt entstehen,
    // selbst wenn die Operation intern als Entfernen und Neu-Einfügen
    // implementiert wird.
    // (Wirkungslos mit Resultatwert false, wenn e ein Nullzeiger ist
    // oder e nicht zur aktuellen Halde gehört; sonst Resultatwert true.)
    bool changePrio (Entry* e, P p){
        //wenn e ein Nullzeiger ist oder e nicht zur aktuellen Halde gehört
        if(e == nullptr || !(contains(e))){
            return false; // dann false zurückgeben
        }
        // Ansonsten Priorität ändern
        else {
            // Eintrag von e auf p setzen.
            P old = e->prio;
            e->prio = p;
            // Hilfsvariablen
            Node* n = e->node;
            Entry* mem = nullptr;
            // Neue Prio kleiner der Alten?
            if (p < old) {
                // Solange die Priorität des Objekts kleiner als die seines Vorgängers ist
                while (n->parent != nullptr && n->entry->prio < n->parent->entry->prio){
                    mem = n->entry;
                    n->entry = n->parent->entry;
                    n->entry->node = n;
                    n->parent->entry = mem;
                    n->parent->entry->node = n->parent;
                    n = n->parent;
                }
                return true;
            }
            else {
                if (e->node->child == nullptr) {
                    return true;
                }
                else{
                    remove(e);
                    Node* in = new Node(e);
                    head = heapmerge(head, in);
                }
            }
            return true;
        }
    }

    // Eintrag e aus der Halde entfernen (aber nicht freigeben).
    // (Wirkungslos mit Resultatwert false, wenn e ein Nullzeiger ist
    // oder e nicht zur aktuellen Halde gehört; sonst Resultatwert true.)
    bool remove (Entry* e){
        if(e == nullptr || !(contains(e))) return false;
        else {
            //Entnimm das Objekt mit minimaler Priorität
            Entry* min = extractMin();
            P temp = e->prio;
            //Wenn das entnommene Objekt nicht das zu entfernende Objekt ist
            if (min != e) {
                //Ändere die Priorität des Objekts auf die Priorität des entnommenen Objekts
                e->prio = min->prio;
                //Verbinde das entnommene Objekt mit dem Knoten des zu entfernenden Objekts
                min->node = e->node;
                min->node->entry = min;
                //Setze die Priorität des zu entfernenden Objekts wieder auf ihren ursprünglichen Wert
                e->prio = temp;
            }
            return true;
        }
    }


    void printTree(Node *n, uint depth, uint s) {
        // abbruchbedingung der Rekursion
        if (n == nullptr||s <= 0) return;
        // tiefe ausprinten
        for (uint i = 0; i < depth; i++)
        {
            cout << "  ";
        }
        // ausgabe des eintrags
        cout << n->entry->prio << " " << n->entry->data << endl;
        s--;
        // rekursiver Aufruf
        if (n->child != nullptr) {
            depth++;
            printTree(n->child->sibling, depth, s);
        }
        else if (n->sibling == n) {
            depth--;
            printTree(n->parent->sibling, depth, s);
        }
        else {
            printTree(n->sibling, depth, s);
        }
    }
    void dump (){
        // Ausgabe der Halde
        uint depth = 0;
        uint s = 0;
        s = size();
        // eigentlicher Aufruf der Rekursiven Ausgabefunktion
        printTree(head, depth, s);
    }
};