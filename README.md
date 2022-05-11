# Algo-Ds2-Ueb2
Übung 2 von Algorithmen und Datenstrukturen 2 im SoSe 2022 and der Hochschule Aalen.
Mit Nico Haisch, Friedrich Röntsch und Moritz Hausmann.
Abgabetermin ist der 02.06.2022.

**JEDE FUNKTION AUßER DUMP DARF NUR LOGARITHMISCHE LAUFZEIT HABEN**

## COMMIT-ETIQUETTE:
An folgende Commit-Etiquette ist sich zu halten.
https://www.conventionalcommits.org/en/v1.0.0/

Link zur weiteren Erklärung der Keywords am Beginn des Statements.
https://github.com/angular/angular/blob/22b96b9/CONTRIBUTING.md#-commit-message-guidelines

## Aufgabenverteilung:
**Deadline:** Montag 16.05.2022 (next meeting 15:00 Uhr)
Pls update the checklist with 
- [x] `[x]` when done.
### Nico:
  - [ ] Entry* minimum ()
  - [ ] Entry* extractMin ()
  - [ ] bool contains (Entry* e)
### Friedrich:
  - [ ] bool changePrio (Entry* e, P p)
  - [ ] bool remove (Entry* e)
  - [ ] void dump ()
### Moritz:
  - [x] BinHeap ()
  - [x] bool isEmpty ()
  - [x] uint size ()
  - [x] Entry* insert (P p, D d)

# Überlegungen Zu den Aufgaben 
## Allgemein: 
  Es ist gefordert, eine **Minimum-Vorrangwarteschlange** mittels **Binomial-Halden** zu implementieren.
  **ACHTUNG:** Nicht zu verwechseln mit einer Implementierung mittels ~~binärer~~ Halden!
  
## Minimum-Vorrangwarteschlange:
  Ist eine Datenstruktur, die folgende Operationen möglichst effizient unterstützt:
  + Einfg. eines Objekts mit bestimmter Priorität
  + Auslesen und ggf. Entnehmen eines Objekts mit minimaler Priorotät
  + Nachträgl. Ändern der Priorität eines Objekts
  + Entf. eines Objekts 
  + Vereinigen zweier Warteschlangen (geht nur mit binomial heaps)
  Eine Minimum Vorrangwarteschlange kann durch eine Binomial-Halde implementiert werden.

## Binomial-Halden/-Bäume:
### BÄUME:
  Zum Verständnis müssen zunächst **Binomial-Bäume** erklärt werden:
  Ein Binomial-Baum ist eine Baumstruktur mit folgender Besonderheit:
  + Ein BB mit Grad k=0 besteht aus einem einzelnen Knoten
  + Für k>=1 besteht ein BB aus zwei Unterbäumen mit Grad k-1
  + Dabei ist ein Unterbaum der erste Nachfolger des anderen Unterbaums

Hier von links nach rechts mit Grad 0 bis 3. Bei dem dritten Grad ist dann ersichtlich, dass dieser einen Teilbaum der Größe k=2 links an den Vaterknoten eines k=2 Baumes angehängt hat.
<br />
![Binomial-Heap](https://i.ibb.co/BnMkq8Q/Binomial-Tree-0-3.png)

Für jeden Binomial-Baum mit Grad k>=0 gilt:
+ Tiefe des Baums und Grad des Wurzelknotens ist k 
+ Nachfolger des Wurzelknotens sind BB mit Grad k-1,...,0 in dieser Reihenfolge
+ Ein Baum besitzt 2^k Knoten
+ Auf Ebene l mit 0 <= l <= k gibt es genau k über l Knoten.
### MINIMAL:
Ein **Minimal**-Binomial-Baum ist also ein Binomialbaum, der die Minimum-Bedingung erfüllt.
Ein Baum erfüllt die Minimal-Bedingung wenn jeder seiner Knoten, außer die Wurzel, höchstens so groß wie sein Nachfolger ist.
In der Folge sind alle Knoten eines Teilbaums mindestens so groß wie die Wurzel dieses Teilbaums.
Also ist die Wurzel des gesamten Baums ein minimales Element des Baums.
### HALDEN:
Eine **Minimum-Binomial-Halde** ist eine (endliche) Folge von Minimum-Binomial-Bäumen, deren Grad streng monoton wächst.
Folgerung:
+ Alle Bäume innerhalb einer Halde sind von unterschiedl. Grad.
+ Eine Binomial-Halde mit N Elementen besteht aus BB mit Grad k1 < ... < kp sodass N die Summe von 2^ki von k1 bis kp ist. Dem folgend sind die Gräder genau die Ziffern in der Binomialdarstellung von N die dort den Wert 1 besitzen.
+ Sowohl die Anzahl der Bäume in einer solchen Halde als auch deren Grad und Tiefe ist jeweils höchstens O(log2 N)
### IMPLEMENTIERUNG:
#### Allgemein: 
Es wird eine Struktur an Binomial-Bäumen angelegt, bei welchen alle Wurzelknoten mit .sibling auf den nächsten Wurzelknoten verweisen.
Diese sich ergebende liste ist endlich und nicht zirkulär, allerdings nach grad der Bäume aufsteigend sortiert.

Das Child eines Wurzelknotens ist der Nachfolger (also nicht sibling) mit dem größten Grad. Das Child wiederum verweist in einer zirkulären Liste auf den Knoten der selben Ebene mit dem nächstniedrigen Grad.
Jeder Knoten außer den Wurzelknoten verweist mit dem Parent-Zeiger auf den Vorgängerknoten. Also verweist ein Wurzelknoten von einem Baum Grad 2 beispielsweise zwar nur auf den linken Kindknoten mit child.
Da dieser einen höheren Grad hat als der rechte, allerdings verweist der Linke Kindknoten auf den rechten Kindknoten mit sibling, und der rechte zirkulär auf den linken mit sibling.
Außerdem verweisen beide mit parent auf den Wurzelknoten.
<br /> 

#### Knoten eines Baums/Halde:
+ `degree` Speichert Grad des Knotens
+ `parent` verweist auf Vorgänger. Bei Wurzelknoten = NULL
+ `child` verweist auf Nachfolger. Bei Blatt = NULL
+ `sibling` verkettet Nachfolger eines Knotens in einer nach aufst. Grad sortierten Liste
     - Der Nachfolger mit dem größten Grad verweist auf den Nachfolger mit dem
    kleinsten Grad.
     - Jeder andere Nachfolger verweist auf den Nachfolger mit dem nächstgrößeren
    Grad.
     - Wenn es nur einen Nachfolger gibt, verweist er auf sich selbst.
    Außerdem verkettet `sibling` die Wurzelknoten aller Bäume der Halde in einer
    nach aufsteigendem Grad sortier ten einfachen Liste, d. h. jeder Wurzelknoten
    verweist ggf. auf den Wurzelknoten mit dem nächstgrößeren Grad.
+ `entry` verweist auf das Objekt das in dem Knoten gespeichert werden soll

#### TreeMerge:
Bei einem Merge zweier **Bäume** wird überprüft welcher Baum den minimalen Prioritätswert am Wurzelknoten hat. Um beim Merge die Minimal-Bedingung zu erhalten.
Entsprechend wird dann dem Wurzelknoten des Baumes mit der niedrigeren Priorität der Wurzelknoten des baumes mit der höheren Priorität als Sibling zugeordnet.

#### HeapMerge:
Bei einem Merge zweier **Halden** wir einfach die kleinere Halde an die größere angehängt.
Dann wird mit degrsort die Wurzelknoten nach aufsteigendem Grad sortiert. Sodass monoton steigender Grad.
Nun kann mit jedem Baumpaar mit gleichem Grad TreeMerge ausgeführt werden. 
*Zur weiteren Erklärung der gefordertern Implementierungen Siehe [PDF mit Folien](https://github.com/mrtzhsmnn/Algo-Ds2-Ueb2/blob/main/Folien%20zur%20Implementierung.pdf) aus der Vorlesung ab Seite 104.*
