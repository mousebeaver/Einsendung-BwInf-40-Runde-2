/*
Dieses Programm testet, ob eine Ausgabe von Karten auch korrekt ist (also XOR = 0 gilt)
*/
#include <bits/stdc++.h>
using namespace std;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin>>n;
    vector<string> s(n);
    for(int i = 0; i < n; i++)
    {
        cin>>s[i];
    }
    int l = (int) s[0].length();

    bool correct = true;
    for(int i = 0; i < l; i++)
    {
        int c = 0;
        for(int j = 0; j < n; j++)
        {
            if(s[j][i] == '1')
            {
                c = !c;
            }
        }
        if(c != 0)
        {
            correct = false;
            break;
        }
    }

    if(correct)
    {
        cout<<"CORRECT"<<endl;
    }
    else
    {
        cout<<"NOT CORRECT"<<endl;
    }

    return 0;
}
