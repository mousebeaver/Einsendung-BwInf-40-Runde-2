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

string bitXOR(string a, string b)
{
    //Diese Funktion liefert das exklusive Oder zweier Passwortkarten zurueck
    string output = "";
    for(int i = 0; i < (int) a.length(); i++) //a und b haben gleiche Laenge
    {
        if(a[i] == b[i])
        {
            output += "0";
        }
        else
        {
            output += "1";
        }
    }
    return output;
}

int main()
{
    /*
    Dieses Programm soll das Problem fuer die Eingabedatei stapel5.txt (und zum Testen auch fuer stapel0.txt) loesen.
    Deswegen wird der Einfachheit halber davon ausgegangen, dass (inklusive Sicherungskarte) genau 5 Karten gesucht werden
    */
    int numcards; //Anzahl der Karten im Stapel
    int wanted; //Anzahl der Karten, die aus dem Stapel ausgewaehlt werden sollen (inklusive Sicherungskarte)
    int bits; //Laenge der Passwortkarten
    ofstream fileOutput; //Ausgabedatei
    vector<string> cards(0); //Speicher fuer die Karten in Form von Bitstrings

    readInput(numcards, wanted, bits, fileOutput, cards); //Einlesen der Eingabe

    bool found = false; //Wurde die Loesung schon gefunden?
    for(int split = 2; split < numcards-2 && !found; split++) //Index der dritten Karte
    {
        unordered_map<string, pair<int, int>> pairXOR; //Speichert jeden XOR-Wert zweier Karten vor Split und die zwei Indizes der Karten
        for(int i = 0; i < split-1; i++)
        {
            for(int j = i+1; j < split; j++)
            {
                pairXOR[bitXOR(cards[i], cards[j])] = {i, j};
            }
        }

        //Durchgehen aller XOR-Paare auf der rechten Seite von split:
        for(int i = split+1; i < numcards-1 && !found; i++)
        {
            for(int j = i+1; j < numcards; j++)
            {
                if(pairXOR.count(bitXOR(bitXOR(cards[i], cards[j]), cards[split])) > 0) //Eine Loesung wurde gefunden!
                {
                    found = true;
                    fileOutput<<wanted<<"\n";
                    //Schreiben der einzelnen Karten in die Ausgabedatei:
                    fileOutput<<cards[pairXOR[bitXOR(bitXOR(cards[i], cards[j]), cards[split])].first]<<"\n";
                    fileOutput<<cards[pairXOR[bitXOR(bitXOR(cards[i], cards[j]), cards[split])].second]<<"\n";
                    fileOutput<<cards[split]<<"\n";
                    fileOutput<<cards[i]<<"\n";
                    fileOutput<<cards[j]<<"\n";
                    break;
                }
            }
        }
    }

    fileOutput.close();
    return 0;
}
