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
    int position = 0; //Nummer k der aktuell zu untersuchenden Variable v_k
    vector<short> tautology(numcards, 0); //Koeffizienten einer wahren Gleichung 0 = 0
    vector<int> unknown(0); //Indizes k, bei denen v_k nicht als erster gesetzter Koeffizient einer Gleichung auftauchen konnte, der aber gesetzt ist (entspricht der Menge U)

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
            //Das aktuelle Bit ist in keiner spaeteren Gleichung gesetzt
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
    //Iterieren durch alle Moeglichkeiten fuer diese Variablen mit BitMagic:
    for(long long i = 0; i < (1<<guess); i++)
    {
        vector<int> res(equations.size(), 0); //Werte auf der rechten Seite der Gleichung
        vector<int> output(0); //Indizes der auszugebenden Karten

        for(int j = 0; j < guess; j++)
        {
            if((i & (1<<j)) != 0) //Durchgehen durch alle unbekannten Indizes
            {
                for(int a = 0; a < (int) equations.size(); a++) //Aktualisieren der Gleichungen
                {
                    if(equations[a][unknown[j]] == 1)
                    {
                        res[a] = !res[a];
                    }
                }
                output.push_back(unknown[j]); //Einfuegen von unbekannten Indizes, die auf 1 gesetzt wurden
            }
        }

        //Auswaehlen unter den weiteren Karten:
        int position = 0; //Position des Indizes, der aktuell bestimmt wird
        for(int i = 0; i < (int) equations.size(); i++)
        {
            while(equations[i][position] == 0)
            {
                position++;
            }
            if(res[i] == 1) //Ein auf 1 gesetzter bekannter Index wurde gefunden
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
            break;
        }
    }

    fileOutput.close();
    return 0;
}
