#define ll long long
#define plb pair<ll, bool>
#define OBERGRENZE 100000
#include <bits/stdc++.h>
using namespace std;

void eingabeEinlesen(int& zahlen, ofstream& ausgabedatei)
{
    /*
    Diese Funktion liest die vom Anwender einzugebenden Daten ein.
    Sie oeffnet ausserdem die Ausgabedatei und uebergibt an den deterministischen Zufallsgenerator dessen Anfangswert.
    */

    cout<<"Name der Ausgabedatei: ";
    string dateiname;
    cin>>dateiname;
    ausgabedatei.open(dateiname, ios_base::out);

    cout<<"Anzahl der Operatoren des Rechenraetsels: ";
    //Diese Zahl ist fuer den Anwender intuitiver und wird in die Anzahl der Zahlen auf der linken Seite der Gleichung im Rechenraetsel umgerechnet.
    cin>>zahlen;
    if(zahlen < 1)
    {
        cout<<"Es muss im Rechenraetsel mindestens einen Operator geben!"<<endl;
        exit(0);
    }
    zahlen++; //Es gibt auf der linken Seite der Gleichung eine Zahl mehr, als es Operatoren gibt.

    //Vorbereiten des Zufallsgenerators
    srand((unsigned) time(nullptr));
}

void ausgabe(ll ergebnis, vector<ll> raetsel, ofstream& ausgabedatei, string loesung)
{
    //Diese Funktion macht die Ausgabe
    if(raetsel.size()-1 > 1)
    {
        ausgabedatei<<"Rechenraetsel mit "<<raetsel.size()-1<<" Operatoren: ";
    }
    else
    {
        ausgabedatei<<"Rechenraetsel mit 1 Operator: ";
    }
    for(int i = 0; i < (int) raetsel.size(); i++)
    {
        ausgabedatei<<raetsel[i]<<" ";
        if(i < (int) raetsel.size()-1)
        {
            ausgabedatei<<"_ "; //Die einzutragenden Operatoren werden durch Unterstriche markiert.
        }
    }
    ausgabedatei<<"= "<<ergebnis<<endl;
    ausgabedatei<<"Loesung dieses Rechenraetsels: ";
    for(int i = 0; i < (int) raetsel.size(); i++)
    {
        ausgabedatei<<raetsel[i]<<" ";
        if(i < (int) raetsel.size()-1)
        {
            ausgabedatei<<loesung[i]<<" "; //Die einzutragenden Operatoren werden durch Unterstriche markiert.
        }
    }
    ausgabedatei<<"= "<<ergebnis<<endl;
    ausgabedatei.close();
}

int main()
{
    int zahlen; //Die Anzahl der Zahlen auf der linken Seite der Gleichung im Rechenraetsel
    ofstream ausgabedatei; //Datei, in die das Rechenraetsel geschrieben wird
    eingabeEinlesen(zahlen, ausgabedatei); //Liest die benoetigten Eingaben vom Anwender ein

    vector<ll> raetsel(zahlen); //Dieser Container enthaelt die Ziffern auf der linken Seite des Rechenraetsels
    //Randomisiertes Initialisieren des Containers raetsel:
    for(int i = 0; i < zahlen; i++)
    {
        if(i == 0 || raetsel[i-1] != 2)
        {
            //Die Zahl 2 darf fuer die aktuelle Ziffer verwendet werden
            raetsel[i] = 2+rand()%8;
        }
        else
        {
            //Die Zahl 2 darf fuer die aktuelle Ziffer nicht verwendet werden
            raetsel[i] = 3+rand()%7;
        }
    }

    vector<ll> preProdukt(zahlen); //Der i-te Wert ist das Produkt der ersten i Zahlen
    vector<ll> postProdukt(zahlen); //Der i-te Wert ist das Produkt aller Zahlen, die auf die i-te folgen (ohne diese selbst)
    preProdukt[0] = raetsel[0];
    for(int i = 1; i < zahlen; i++)
    {
        preProdukt[i] = preProdukt[i-1]*raetsel[i];
    }
    postProdukt[zahlen-1] = 0;
    postProdukt[zahlen-2] = raetsel[zahlen-1];
    for(int i = zahlen-3; i >= 0; i--)
    {
        postProdukt[i] = postProdukt[i+1]*raetsel[i+1];
        //Abfangen von Ueberlaeufen in dieser Zahl:
        if(postProdukt[i]/raetsel[i+1] != postProdukt[i+1])
        {
            cout<<"Die im Programm verwendeten Datentypen reichen fuer die zufaellig ausgewaehlten Zahlen nicht aus!"<<endl;
            cout<<"Bitte versuchen Sie es erneut und verringern Sie ggf. die Zahl der Operatoren!"<<endl;
            return 0;
        }
    }

    vector<vector<plb>> m(zahlen, vector<plb> (0)); //Container fuer die mit den ersten i Zahlen erzeugbaren Ergebnisse
    vector<vector<plb>> s(zahlen, vector<plb> (0)); //Container fuer die durch Multiplikaton und Division erzeugten Summanden fuer jede Zahl
    //In den obigen Containern ist der zu einer Zahl gehoerige bool-Wert genau dann True, wenn sie eindeutig ist
    vector<unordered_map<ll, ll>> mPos(zahlen); //Hashtabellen mit den Indizes bestimmter Elemente in m
    vector<unordered_map<ll, ll>> sPos(zahlen); //Hashtabellen mit den Indizes bestimmter Elemente in s
    vector<unordered_map<ll, string>> mLoes(zahlen); //Hashtabellen mit den Operatoren-loesungen fuer Elemente von m
    vector<unordered_map<ll, string>> sLoes(zahlen); //Hashtabellen mit den Operatoren-loesungen fuer Elemente von s

    for(int i = 0; i < zahlen; i++) //Berechnen von m[i] fuer jede Ziffer im Rechenraetsel
    {
        //Aktualisieren der Summanden:
        for(int j = 0; j < i; j++)
        {
            ll anzahl = (ll) s[j].size(); //Anzahl der bisher gefundenen Summanden
            for(ll k = 0; k < anzahl; k++) //Multiplikation mit der neuen Zahl:
            {
                sPos[j].clear(); //Hashtabelle fuer den aktuellen Container von Summanden zuruecksetzen
                s[j][k].first *= raetsel[i];
                sLoes[j][s[j][k].first] = sLoes[j][s[j][k].first/raetsel[i]]+"*";
                sLoes[j].erase(s[j][k].first/raetsel[i]);
                sPos[j][s[j][k].first] = k;
            }
            for(ll k = 0; k < anzahl; k++) //Division durch die neue Zahl
            {
                if(s[j][k].first%(raetsel[i]*raetsel[i]) == 0)
                {
                    //Der Summand ist durch die neue Zahl teilbar (bzw. durch ihr Quadrat, da er vorher mit der neuen Zahl multipliziert wurde)
                    if(sPos[j].count(s[j][k].first/(raetsel[i]*raetsel[i])) == 1)
                    {
                        //Die Zahl wurde schon gefunden
                        s[j][sPos[j][s[j][k].first/(raetsel[i]*raetsel[i])]].second = false;
                    }
                    else
                    {
                        //Der gefundene Summand ist neu
                        s[j].push_back({s[j][k].first/(raetsel[i]*raetsel[i]), true});
                        sPos[j][s[j][k].first/(raetsel[i]*raetsel[i])] = (ll) s[j].size()-1;

                        string loesung = sLoes[j][s[j][k].first];
                        loesung = loesung.substr(0, loesung.length()-1);
                        loesung = loesung + ":";
                        sLoes[j][s[j][k].first/(raetsel[i]*raetsel[i])] = loesung;
                        if(!s[j][k].second) //Der Summand ist trotzdem nicht neu
                        {
                            s[j][sPos[j][s[j][k].first/(raetsel[i]*raetsel[i])]].second = false;
                        }
                    }
                }
            }
        }
        s[i].push_back({raetsel[i], true});
        sPos[i][raetsel[i]] = 0;
        sLoes[i][raetsel[i]] = "";

        //Berechnen aller moeglichen Ergebnisse fuer die ersten i Zahlen:
        //Alle bisherigen Zahlen bilden einen einzigen Summanden:
        for(plb p : s[0])
        {
            if(p.first - postProdukt[i] < OBERGRENZE || p.first == preProdukt[i])
            {
                //Die Zahl ist klein genug oder die groesstmoegliche
                m[i].push_back({p.first, p.second});
                mPos[i][p.first] = (ll) m[i].size() - 1;
                mLoes[i][p.first] = sLoes[0][p.first];
            }
        }
        //Summanden werden zu bisherigen Ergebnissen addiert
        for(int j = 1; j <= i; j++) //Erste Zahl des letzten Summanden
        {
            for(plb p : m[j-1]) //Durchgehen aller vorherigen Ergebnisse
            {
                for(plb a : s[j]) //Durchgehen aller Summanden zu diesem Ergebnis
                {
                    //Addition des Summanden:
                    if(mPos[i].count(p.first+a.first) == 1)
                    {
                        //Das Ergebnis ist mehrdeutig
                        m[i][mPos[i][p.first+a.first]].second = false;
                    }
                    else
                    {
                        //Das Ergebnis ist neu
                        if(p.first+a.first-postProdukt[i] <= OBERGRENZE && p.first+a.first+postProdukt[i] > 0)
                        {
                            //Das Ergebnis liegt in den noetigen Schranken
                            if(!p.second || !a.second)
                            {
                                //Ein Summand ist nicht eindeutig
                                m[i].push_back({p.first+a.first, false});
                            }
                            else
                            {
                                //Das Ergebnis ist eindeutig
                                m[i].push_back({p.first+a.first, true});
                            }
                            mPos[i][p.first+a.first] = m[i].size()-1;
                            mLoes[i][p.first+a.first] = mLoes[j-1][p.first]+"+"+sLoes[j][a.first];
                        }
                    }

                    //Substraktion des Summanden:
                    if(mPos[i].count(p.first-a.first) == 1)
                    {
                        //Das Ergebnis ist mehrdeutig
                        m[i][mPos[i][p.first-a.first]].second = false;
                    }
                    else
                    {
                        //Das Ergebnis ist neu
                        if(p.first-a.first-postProdukt[i] <= OBERGRENZE && p.first-a.first+postProdukt[i] > 0)
                        {
                            //Das Ergebnis liegt in den notwendigen Schranken
                            if(!p.second || !a.second)
                            {
                                //Ein Summand ist nicht eindeutig
                                m[i].push_back({p.first-a.first, false});
                            }
                            else
                            {
                                //Das Ergebnis ist eindeutig
                                m[i].push_back({p.first-a.first, true});
                            }
                            mPos[i][p.first-a.first] = m[i].size()-1;
                            mLoes[i][p.first-a.first] = mLoes[j-1][p.first]+"-"+sLoes[j][a.first];
                        }
                    }
                }
            }
        }
    }

    //Nun Auswaehlen eines Ergebnisses aus dem letzten Container m
    //Zunaechst Loeschen aller mehrdeutigen Elemente:
    for(ll i = 0; i < (ll) m[zahlen-1].size(); i++)
    {
        if(!m[zahlen-1][i].second)
        {
            m[zahlen-1].erase(m[zahlen-1].begin()+i);
            i--;
        }
    }
    //Wenn moeglich, Loeschen des groessten Elements:
    if(m[zahlen-1].size() > 1)
    {
        m[zahlen-1].erase(max_element(m[zahlen-1].begin(), m[zahlen-1].end()));
    }

    //Nun zufaelliges Auswaehlen eines Ergebnisses:
    ll ergebnis = m[zahlen-1][rand()%m[zahlen-1].size()].first;
    string loesung = mLoes[zahlen-1][ergebnis];

    //Ausgabe:
    ausgabe(ergebnis, raetsel, ausgabedatei, loesung);

    return 0;
}
