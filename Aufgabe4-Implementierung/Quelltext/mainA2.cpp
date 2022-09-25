#include <bits/stdc++.h>
using namespace std;

void readInput(int& numcards, int& wanted, int& bits, ofstream& fileOutput, vector<string>& cards)
{
    //Diese Funktion liest die Eingabe aus der Eingabedatei

    char nameInput[100]; //Name der Eingabedatei
    string nameOutput; //Name der Ausgabedatei
    cout<<"Name der Eingabedatei: ";
    cin>>nameInput;
    cout<<"Name der Ausgabedatei: ";
    cin>>nameOutput;

    //Oeffnen der Ein- und Ausgabedateien:
    fileOutput.open(nameOutput, ios_base::out);
    freopen(nameInput, "r", stdin);

    //Einlesen der eigentlichen Eingabe:
    cin>>numcards;
    cin>>wanted;
    wanted++; //Die Sicherungskarte wird mitgezaehlt
    cin>>bits;
    for(int i = 0; i < numcards; i++)
    {
        string s;
        cin>>s;
        cards.push_back(s);
    }
}

void solveEquations(int wanted, vector<vector<short>>& equations, vector<int> output, ofstream& fileOutput, vector<string>& cards)
{
    //Diese Funktion loest das Gleichungssystem, wenn bekannt ist, welche Variablen aus dem Container unknown auf 1 gesetzt werden
    vector<int> res(equations.size(), 0); //Werte auf der rechten Seite der Gleichung
    for(int i : output) //Aktualisieren von res auf Basis des Containers output
    {
        for(int a = 0; a < (int) equations.size(); a++)
        {
            if(equations[a][i] == 1)
            {
                res[a] = !res[a];
            }
        }
    }

    //Auswaehlen unter den weiteren Karten, Loesung des Gleichungssystems:
    int position = 0;
    for(int i = 0; i < (int) equations.size(); i++)
    {
        while(equations[i][position] == 0)
        {
            position++;
        }
        if(res[i] == 1)
        {
            output.push_back(position);
        }
    }

    if((int) output.size() == wanted) //Die Loesung wurde gefunden!
    {
        fileOutput<<wanted<<"\n"; //Die Anzahl der ausgegebenen Karten wird in die Ausgabedatei geschrieben
        for(int i : output)
        {
            fileOutput<<cards[i]<<"\n";
        }
        fileOutput.close();
        exit(0);
    }
}

void seekSolution(int wanted, vector<vector<short>>& equations, vector<int> output, ofstream& fileOutput, vector<string>& cards,
                  vector<int>& unknown, int guess, int remaining, int position)
{
    //Diese Funktion geht rekursiv alle Moeglichkeiten durch, eine Anzahl von Variabeln aus dem Container unknown auf 1 zu setzen
    //Fuer jede dieser Moeglichkeiten wird die Funktion solveEquation aufgerufen
    //Im Container output werden die entsprechenden Indizes gespeichert
    //Die Variable remaining haelt fest, wie viele Indizes noch gewaehlt werden muessen, die Variable position, ab welchem Index dies moeglich ist
    if(remaining == 0) //Alle Indizes wurden gewaehlt
    {
        solveEquations(wanted, equations, output, fileOutput, cards);
    }
    else
    {
        for(int i = position; i < guess; i++)
        {
            //Hinzufuegen eines weiteren Indizes:
            output.push_back(unknown[i]);
            seekSolution(wanted, equations, output, fileOutput, cards, unknown, guess, remaining-1, i+1);
            output.pop_back();
        }
    }
}

int main()
{
    int numcards; //Anzahl der Karten im Stapel
    int wanted; //Anzahl der Karten, die aus dem Stapel ausgewaehlt werden sollen (inklusive Sicherungskarte)
    int bits; //Laenge der Passwortkarten
    ofstream fileOutput; //Ausgabedatei
    vector<string> cards(0); //Speicher fuer die Karten in Form von Bitstrings

    readInput(numcards, wanted, bits, fileOutput, cards); //Einlesen der Eingabe

    vector<vector<short>> equations(bits, vector<short> (numcards, 0)); //Container fuer die Koeffizienten des Gleichungssystems
    //Bestimmen dieser Koeffizienten:
    for(int i = 0; i < bits; i++)
    {
        for(int j = 0; j < numcards; j++)
        {
            if(cards[j][i] == '1')
            {
                equations[i][j] = 1;
            }
        }
    }

    //Es folgt die Durchfuehrung der Gauss-Adaption
    int settled = 0; //Anzahl der bereits an ihrem Ort festgelegten Gleichungen
    int position = 0; //Nummer k der aktuell zu untersuchenden Variable w_k
    vector<short> tautology(numcards, 0); //Koeffizienten einer wahren Gleichung
    vector<int> unknown(0); //Indizes k, bei denen w_k nicht als erster gesetzter Koeffizient einer Gleichung auftauchen konnte, der aber gesetzt ist

    while(settled < (int) equations.size()) //Durchgehen der Variablen
    {
        //Untersuchen, ob das aktuelle Bit in mindestems einem Passwort gesetzt ist:
        int index = -1; //Index einer Karte mit dem aktuellen, gesetzten Bit
        for(int i = settled; i < (int) equations.size(); i++)
        {
            if(equations[i][position] == 1)
            {
                index = i;
                break;
            }
        }

        if(index == -1)
        {
            //Das aktuelle Bit ist in keiner spaeteren Gleichugn gesetzt
            //Wurde es in einer bereits gefundenen gesetzt?
            bool used = false;
            for(int i = 0; i < settled; i++)
            {
                if(equations[i][position] == 1)
                {
                    used = true;
                    break;
                }
            }
            if(used)
            {
                unknown.push_back(position);
            }

            position++;
        }
        else
        {
            //Das Bit ist in mindestens einem Passwort gesetzt
            swap(equations[settled], equations[index]); //Tauschen der Gleichung an die richtige Position
            //Addieren der Gleichungen:
            for(int i = 0; i < (int) equations.size(); i++)
            {
                if(i != settled && equations[i][position] == 1) //In der Gleichung ist das Bit gesetzt
                {
                    for(int j = position; j < numcards; j++)
                    {
                        if(equations[settled][j] == 1)
                        {
                            equations[i][j] = !equations[i][j];
                        }
                    }
                    if(equations[i] == tautology)
                    {
                        //Die Gleichung enthaelt nur die wahre Aussage 0 = 0
                        equations.erase(equations.begin()+i);
                        i--;
                    }
                }
            }
            settled++;
            position++;
        }
    }
    while(position < numcards)
    {
        unknown.push_back(position);
        position++;
    }

    //Der Container unkown enthaelt eine Liste der Variablen v_k, fuer die es am sinnvollsten ist, alle Werte durchzuprobieren.
    int guess = unknown.size(); //Anzahl dieser Variablen
    //Iterieren durch alle Moeglichkeiten, maximal vier(fuenf) dieser Variablen auf 1 zu setzen
    //Dabei ist es unsinnig, alle diese Variablen auf 0 zu setzen, da dies die triviale Loesung ergibt, in der alle Variablen 0 sind
    for(int i = 1; i < 11; i++)
    {
        vector<int> output(0); //Indizes der auszugebenden Karten
        //Suchen einer Loesung
        seekSolution(wanted, equations, output, fileOutput, cards, unknown, guess, i, 0);
    }

    fileOutput.close();
    return 0;
}
