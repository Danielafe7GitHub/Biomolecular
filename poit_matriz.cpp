#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

//Score Matrix
vector<int> row_1;
vector<int> row_2;

//string cadena_1 = "-AAAC";
//string cadena_2 = "-AGC";

string cadena_1 = "-ACGGTGCACAAGTTCACCAGTTGAACAAATTCGGTGCAGTCACCATAACGGTGCAGTCACCAGGCGGTGCAGTCACCCGGCGGTCGGTGCAGCATGCAAGCAACGGTGCAGTCACCAGGCACCAGTGCACAAGTTCACCAGTTTAACGAAGTGCAGTCACC";
string cadena_2 = "-ACCGTGCAGTTGAACATCGGTGCAGAATTCGGTGCAGTCACCATAACGGTCGGTGCAGTGCAGTCACCAGGCGGTGCAGTCACCCGGCGGTCGGTGCAGCATGCAAGCAACGGTGCAGTCACCAGGCACCATCAGTCACCAGGCACCACCAGCGGTGCAG";

string alineamiento;

int m = cadena_1.size();
int n = cadena_2.size();
int pos = 0; //cambialo a bit
//Poiting Matrix
vector<vector<int>> matrix(m); //Debe ser del tipo bit

int maximo(int a , int b , int c)
{
    int _max = std::max(std::max(a,b),c);
    if(_max == a)
        pos = 2; //diagonal
    else if(_max == b)
        pos = 3; //horizontal
    else if(_max == c)
        pos = 1; // vertical
    return _max;
}

void init()
{
    //Inicializando la matriz de score
    for(int i=0;i<cadena_2.size();i++)
    {
        row_1.push_back(i * -2);
        row_2.push_back((i +1) * -2);
    }

    //Inicializando pointting matrix
    for ( int i = 0 ; i < m ; i++ )
        matrix[i].resize(n,0);

    for (int i = 1; i < n; i++) {
        matrix[0][i] = 3;
    }

    for (int j = 1; j < m; j++) {
        matrix[j][0] = 1;
    }

    /*for(int i=0;i<m;i++)
    {
        for(int j=0;j<n;j++)
            cout<<matrix[i][j]<<"   ";
        cout<<endl;
    }*/
    /*for(int i=0;i<n;i++)
    {
        cout<<row_1[i]<<"   ";
    }
    cout<<endl;
    for(int i=0;i<n;i++)
    {
        cout<<row_2[i]<<"   ";
    }*/
}

void global_aligment()
{
    cout<<"m: "<<m<<" n: "<<n<<endl;
    int _c = 2;
    for(int i=1;i<m;i++)
    {
       for(int j=1;j<n;j++)
       {
           row_2[j] = maximo((cadena_1[i] == cadena_2[j] ? row_1[j - 1] + 1 : row_1[j - 1] - 1),
                             row_2[j - 1] - 2, row_1[j] - 2);
           matrix[i][j] = pos;
       }
       row_1 = row_2;
       row_2[0] = _c * -2;
       _c++;
       //Quien guarda la última fila(score):row_1
       /*for(int i=0;i<n;i++)
       {
           cout<<row_1[i]<<"   ";
       }
       cout<<endl;*/
    }
}

void traice_back()
{
    /*for(int i=0;i<m;i++)
    {
        for(int j=0;j<n;j++)
            cout<<matrix[i][j]<<"   ";
        cout<<endl;
    }*/
    //Recorremos las columnas (tam cadena 2, la que queremos alinear)
    int fila = m - 1;
    int columna = n - 1;
    for(int i=n-1;i>=0;i--)
    {
        if(matrix[fila][columna] == 2) //Diagonal
        {
            alineamiento.push_back(cadena_2[i]);
            fila -= 1;
            columna -= 1;
        }
        else if(matrix[fila][columna] == 3) //Horizontal
        {
            alineamiento.push_back('-');
            columna -= 1;
        }
        else if(matrix[fila][columna] == 1) //Vertical
        {
            alineamiento.push_back('-');
            fila -= 1;
        }
    }

}



int main()
{
    init();
    global_aligment();
    traice_back();
    cout<<"Aline tam: "<<alineamiento.size()<<endl;
    reverse(alineamiento.begin() , alineamiento.end() );
    cout<<alineamiento.c_str();

    //Falta probar con otros ejmplos y medir memoria :)
    return 0;
}
