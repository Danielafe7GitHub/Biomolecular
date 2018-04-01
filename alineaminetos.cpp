#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

string cadena1 = "-"; string cadena2 = "-";
int dummy = -5000;
int cantALin = 0;
vector<vector<int> > secMatrix;

int maximo(int a , int b , int c)
{
    return std::max(std::max(a,b),c);
}

void iterativo(vector<vector<int> > &matrix, int m, int n)
{
    //Llenando espacios Prederminados de la Matriz donde i = 0 j=0
    matrix[0][0] = 0;

    for (int i = 1; i < n; i++) {
        matrix[0][i] = i * -2;
    }

    for (int j = 1; j < m; j++) {
        matrix[j][0] = j * -2;
    }


    //De forma Iterativa
    for (int i = 1; i < m; i++) {
        for (int j = 1; j < n; j++) {
            matrix[i][j] = maximo((cadena1[i] == cadena2[j] ? matrix[i - 1][j - 1] + 1 : matrix[i - 1][j - 1] - 1),
                                  matrix[i - 1][j] - 2, matrix[i][j - 1] - 2);
        }
    }

}



int recursive(vector<vector<int> > &matrix, int i, int j)
{
    if(i == 0)
        return matrix[i][j] = j * -2;
    else if (j == 0 )
        return matrix[i][j] = i * -2;
    else if (matrix[i][j] != dummy)
        return matrix[i][j]; //Ya no lo volemos a calcular
    return matrix[i][j] = maximo(recursive(matrix,i-1,j)- 2,recursive(matrix,i,j-1)-2,recursive(matrix,i-1,j-1) + (cadena1[i] == cadena2[j] ? 1 : -1 )) ;

}


vector<vector<int> > init(string cadena_1, string cadena_2)
{
    //Inicializando la Matriz
    unsigned long m = cadena_1.size() + 1;
    unsigned long n = cadena_2.size() + 1;


    vector<vector<int> > matrix(m);
    for ( int i = 0 ; i < m ; i++ )
        matrix[i].resize(n,dummy);


    cadena1 += cadena_1; cadena2 += cadena_2;

    iterativo(matrix,m,n);
    //recursive(matrix,m-1,n-1);

    return matrix;
}



void printMatrix(vector<vector<int> > &matrix)
{
    for(int i=0; i < matrix.size();i++)
    {
        for(int j=0; j < matrix[0].size();j++)
        {
            cout<< matrix[i][j]<<"          ";
        }
        cout<<endl;
    }
}



void secuencias(int i, int j)
{

    if((i==0 && j==0))
    {
        cantALin += 1;
        return;
    }

    if(i > 0 && j > 0)
    {
        if(secMatrix[i][j] == ( secMatrix[i-1][j-1] + ( (cadena1[i] == cadena2[j]) ? 1 : -1) ) )
        //if(secMatrix[i][j] + 1 ==  secMatrix[i-1][j-1] || secMatrix[i][j] - 1 ==  secMatrix[i-1][j-1])
        {
            //cout<<"Yo vengo de: "<<i<<j<<"  "<<cadena2[j-1]<<endl;
            secuencias(i-1,j-1);
        }
    }

    if(j>0)
    {
        if(secMatrix[i][j] + 2 ==  secMatrix[i][j-1])
        {
            //cout<<"Yo secMatrix de: "<<i<<j<<"  <--"<<endl;
            secuencias(i,j-1);
        }

    }
    if(i>0)
    {
        if(secMatrix[i][j] + 2 ==  secMatrix[i-1][j])
        {
            //cout<<"Yo vengo de: "<<i<<j<<"  |"<<endl;
            secuencias(i-1,j);
        }

    }
}



int main()
{
    /*string cadena_1 = "ACTGATTCA";
    string cadena_2 = "ACGCATCA";*/
    string cadena_1 = "AAAC";
    string cadena_2 = "AGC";
    /*string cadena_1 = "ACGGTGCAGTCACCAGGCGGTGCAGTCACCATAACGGTGCAGTCACCAGGCGGTGCAGTCACCAGCAACGGTGCAGTCACCAGGCGAAGTGCAGTCACC";
    string cadena_2 = "ACGGTGCAGTCACCATTCGGTGCAGTCACCAAAAGGTGCATAACCAGGCGGTGCAGTCACCAGCAACTTTGCAGGGCAGGCGAAGTGCAGTCATT";*/
    /*string cadena_1 = "ACGGTGCACAAGTTCACCAGTTGAACAAATTCGGTGCAGTCACCATAACGGTGCAGTCACCAGGCGGTGCAGTCACCCGGCGGTCGGTGCAGCATGCAAGCAACGGTGCAGTCACCAGGCACCAGTGCACAAGTTCACCAGTTTAACGAAGTGCAGTCACC";
    string cadena_2 = "ACCGTGCAGTTGAACATCGGTGCAGAATTCGGTGCAGTCACCATAACGGTCGGTGCAGTGCAGTCACCAGGCGGTGCAGTCACCCGGCGGTCGGTGCAGCATGCAAGCAACGGTGCAGTCACCAGGCACCATCAGTCACCAGGCACCACCAGCGGTGCAG";
*/
    secMatrix = init(cadena_1,cadena_2);
    //printMatrix(matrix);
    secuencias(cadena_1.size(),cadena_2.size());
    cout<<cadena1<<endl;
    cout<<cadena2<<endl;
    cout<<"La cantidad de Alineamientos Optimos es: "<<cantALin<<endl;

    // hacerlo recursivo, ademas de allar el # de alineamientos ok
    return 0;
}