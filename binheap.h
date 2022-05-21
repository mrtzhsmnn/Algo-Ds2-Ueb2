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
        cout << "insert" << endl;
        // Neuen Eintrag erzeugen.
        Entry* entry = new Entry(p, d);
        Node* node = new Node(entry);
        if (isEmpty()) head = node;
        else head=heapmerge(head, node);
        return entry;
    }

    // HILFSFUNKTION: treecount Gibt die anzahl der Bäume in der Halde zurück.
    uint treecount(Node* a){
        cout << "treecount" << endl;
        uint count = 0;
        Node* n = a;
        while (n != nullptr){
            count++;
            n = n->sibling;
        }
        return count;
    }

    // HILFSFUNKTION: treeinsert, fügt einen Baum am Ende der Halde ein.
    Node* treeinsert(Node* a, Node* b){
        cout << "treeinsert" << endl;
        if (a == nullptr){
            cout << "treeinsert nullptr" << endl;
            a = b;
        }
        else{
            cout << "treeinsert else" << endl;
            Node* n = a;
            while (n->sibling != nullptr){
                cout << "treeinsert else while" << endl;
                n = n->sibling;
            }
            n->sibling = b;
            n->sibling->sibling = nullptr;
        }
        return a;
    }

    // HILFSFUNKTION: treemerge, vereinigt zwei Bäume und liefert eine neue halde zurück.
    Node* oldtreemerge(Node* a, Node* b){
        cout << "treemerge" << endl;
        // Ist die Priorität des ersten Baumes(a) Größer als die des zweiten Baumes(b)?
        if (b->entry->prio < a->entry->prio) {
            cout << "treemerge prioswap" << endl;
            // Wenn ja wird a und b getauscht.
            // Da ja die Priorität von a größer ist muss a teilbaum von b werden.
            Node* swap = a;
            a = b;
            b = swap;
        }
        // Führe nun Merge aus.
        Node* temp = a->child;
        a->child = b;
        b->parent = a;
        b->sibling = temp;
        if (temp != nullptr) {
            while (temp->degree!=0){
                temp = temp->sibling;
            }
            temp->sibling = b;
        }
        // Degree des neuen Wurzelknotens wird um 1 erhöht.
        a->degree = a->degree + 1;
        a->sibling = nullptr;
        return a;
    }



    // HILFSFUNKTION: treemerge, vereinigt zwei Bäume und liefert eine neue halde zurück.
    Node* treemerge(Node* a, Node* b){
        cout << "treemerge" << endl;
        // Ist die Priorität des ersten Baumes(a) Größer als die des zweiten Baumes(b)?
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
                b->child = a;
                b->child->sibling = a;
            }
            return b;
        }
        else{
            cout << "else" << endl;
            a->sibling = nullptr;
            a->degree = a->degree + 1;
            b->parent = a;
            if (a->child == nullptr){
                a->child = b;
                b->sibling = b;
            }
            else {
                b->sibling = a->child->sibling;
                a->child = b;
                a->child->sibling = b;
            }
            return a;
        }
    }

    // HILFSFUNKTION:memsize
    uint memsize(Node** mem){
        uint ret = sizeof(mem)/sizeof(mem[0]);
        return ret;
    }


    // HILFSFUNKTION: heapmerge, vereinigt zwei Halden und liefert eine neue halde zurück.
    Node* heapmerge (Node* a, Node* b){
        //Hilfvariablen
        cout << "heapmerge" << endl;
        Node* c = nullptr;
        uint k = 0;
        uint i = 0;
        Node** mem = new Node*[3]();
        // Solange a und b nicht leer sind.
        while (a!=nullptr || b!=nullptr || i!=0){
            cout << "heapmergewhile" << endl;
            // Ist a leer? (prüfen wegen fehler sonst mit degree)
            if (a!=nullptr) {
                cout << "heapmergewhile a if" << endl;
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
                cout << "heapmergewhile b if" << endl;
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
                cout << "tree count == 1" << endl;
                // Wenn ja, dann mem[0] an c anhängen.
                c = treeinsert(c, mem[0]);
                // mem leeren.
                mem[0] = nullptr;
                i--;
            }
            // Zahl der Bäume in Temp == 3?
            if (i == 3) {
                cout << "treecount == 3" << endl;
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
                cout << "treecount == 2" << endl;
                // Wenn ja, dann Beide Bäume Mergen
                mem[0] = treemerge(mem[0], mem[1]);
                mem[1] = nullptr;
                i--;
                cout << "survived treemerge" << endl;
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
            Node *n_mem = nullptr;
            while (n != nullptr){
                //FIXME: Ausgabe nach fix wieder entfernen
                cout << "minimum() while Schleife" << endl;
                if(n->entry->prio < n_mem->entry->prio || n_mem == nullptr){
                    n_mem = n;
                }
                n=n->sibling;
            }
            return n_mem->entry;
        }
    }

    // Eintrag mit minimaler Priorität liefern
    // und aus der Halde entfernen (aber nicht freigeben).
    // (Bei einer leeren Halde wirkungslos mit Nullzeiger als Resultatwert.)
    Entry* extractMin (){
        //FIXME: Ausgabe nach fix wieder entfernen
        cout << "extractMin() Beginn der Funktion" << endl;
        if(isEmpty()) return nullptr; //wenn Halde leer ist
        else{ //wenn Halde nicht leer ist
            //FIXME: Ausgabe nach fix wieder entfernen
            cout << "extractMin() Halde ist nicht leer" << endl;
            Node* min = minimum()->node; //minimum wird gesucht und in min gespeichert!
            Node* min_mem = min; //min_mem merkt sich min (um min zu löschen)
            Node* min_child = min->child; //min_child ist der child von min.
            remove(min->entry); //min wird aus der Halde entfernt
            Node* temp = nullptr; //temp ist ein leerer Baum
            temp = head; //in temp wird head gespeichert
            while(temp->sibling!=min_mem){
                //FIXME: Ausgabe nach fix wieder entfernen
                cout << "extractMin() while Schleife" << endl;
                temp=temp->sibling;
            }
            temp->sibling=min_mem->sibling; //temp wird mit min_mem->sibling verbunden
            if(min_child != nullptr){ //wenn a ein Nachfolger hatte
                //FIXME: Ausgabe nach fix wieder entfernen
                cout << "extractMin() a hat einen Nachfolger" << endl;
                Node* x= min_mem->child->sibling; //x ist der Nachfolger von min_mem
                min_mem->child->sibling=nullptr; //min_mem->child->sibling wird auf null gesetzt
                head=heapmerge(x,head); //x wird mit head verbunden
            }
            return min_mem->entry; //min_mem wird zurückgegeben
        }
    }

    // Enthält die Halde den Eintrag e?
    // Resultatwert false, wenn e ein Nullzeiger ist.
    bool contains (Entry* e){
        //FIXME: Ausgabe nach fix wieder entfernen
        cout << "contains() Beginn der Funktion" << endl;
        if(e == nullptr) return false; //ist e ein Nullzeiger?
        else{
            //FIXME: Ausgabe nach fix wieder entfernen
            cout << "contains() e ist kein Nullzeiger" << endl;
            Node* n = e->node;
            while(n->parent != nullptr) {
                //FIXME: Ausgabe nach fix wieder entfernen
                cout << "contains() while Schleife 2" << endl;
                n = n->parent;
            }
            Node* p=head;
            while (p->sibling != nullptr){
                //FIXME: Ausgabe nach fix wieder entfernen
                cout << "contains() while Schleife 2" << endl;
                if(p==n) return true;
                p=p->sibling;
            }
            return false;
        }
    }

    // HILSFUNKTION: vertauscht zwei entry Objekte
    void swapEntry(Entry* a, Entry* b){
        Entry* tempa = a;
        a = b;
        b = tempa;
    }
    // Priorität des Eintrags e auf p ändern.
    // Hierbei darf auf keinen Fall ein neues Entry-Objekt entstehen,
    // selbst wenn die Operation intern als Entfernen und Neu-Einfügen
    // implementiert wird.
    // (Wirkungslos mit Resultatwert false, wenn e ein Nullzeiger ist
    // oder e nicht zur aktuellen Halde gehört; sonst Resultatwert true.)
    bool changePrio (Entry* e, P p){
        P oldprio = e->prio;
        Node *n = e->node; //e->node->parent->entry
        if(e == nullptr || !(contains(e))){ //wenn e ein Nullzeiger ist oder e nicht zur aktuellen Halde gehört
            return false; //wenn nicht, dann false zurückgeben
        }
        //Ansonsten Priorität ändern
        else {
            // Setze die Priorität des Objekts auf die neue Priorität
            n->entry->prio = p;
            //Wenn die neue Priorität kleiner oder gleich der alten Priorität ist
            if(!(oldprio < p)){
                //Solange die Priorität des Objekts kleiner oder gleich der alten Priorität ist
                while(!(oldprio < p)) {
                    //FIXME: Ausgabe nach fix wieder entfernen
                    cout << "changePrio() while Schleife" << endl;
                    //Vertausche das Objekt mit seinem Vorgänger
                    swapEntry(e, n->parent->entry);
                }
            }
            //Sofern sich das Objekt nicht in einem Blattknoten befindet
            else if (n->child->entry != nullptr){
                //Entferne das Objekt
                remove(e);
                //Füge das Objekt mit der neuen Priorität wieder ein
                insert(p,e->data);
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

    // HILFSFUNKTION: Baum ausgeben
    void oldprintTree(Node *n)
    {
        cout << "PRINTTREE" << endl;
        while (n != nullptr)
        {
            //Inhalt des Baums ausgeben
            cout << n->entry->prio << " " << n->entry->data << endl;
            //Aufruf mit Child von n
            printTree(n->child);
            //Setze n auf den nächsten Sibling
            n = n->sibling;
        }
    }

    void printTree(Node *n)
    {
//        while (n != nullptr)
//        {
//            n=n->sibling;
//            while (n->degree < n->sibling->degree)
//            {
//                cout << n->entry->prio << " " << n->entry->data << endl;
//                n=n->sibling;
//            }
//            n = n->child;
//        }
        cout << n->entry->prio << " " << n->entry->data << endl;
        if (n->child != nullptr) {
            cout << " ";
            if (n->child->sibling != nullptr) printTree(n->child->sibling);
            else printTree(n->child);
        }
        if ((n->sibling != nullptr) && (n->degree < n->sibling->degree)) printTree(n->sibling);
    }

    // Inhalt der Halde zu Testzwecken ausgeben.
    void olddump (){
        cout << "PENISDUMP" << endl;
        // Inhalt der Halde ausgeben
        Node *n = head;
        int i = 0;
        while(i != size()){
            cout << "DUMPWHILE" << endl;
            cout << " ";
            printTree(n);
            i++;
        }
    }

    void newdump (){
        cout << "dump" << endl;
        Node *n = head;
        while (n != nullptr) {
            cout << n->entry->prio << " " << n->entry->data << endl;
            if (n->child != nullptr) {
                cout << " ";
                if (n->child->sibling != nullptr) printTree(n->child->sibling);
                else printTree(n->child);
            }
            n = n->sibling;
        }
    }
};