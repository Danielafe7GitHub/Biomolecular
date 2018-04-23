#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>
using namespace std;

string cadena1 = "-"; string cadena2 = "-";
int cantALin = 0;
vector<vector<int> > secMatrix;
vector< string > cadenaFinal;
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



vector<vector<int> > init(string cadena_1, string cadena_2)
{
    //Inicializando la Matriz
    unsigned long m = cadena_1.size() + 1;
    unsigned long n = cadena_2.size() + 1;


    vector<vector<int> > matrix(m);
    for ( int i = 0 ; i < m ; i++ )
        matrix[i].resize(n,0);


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



void trace_back(int i, int j,string aliment)
{

    if((i==0 && j==0))
    {
        cantALin += 1;
        cadenaFinal.push_back(aliment);
        return;
    }

    string temp = aliment;
    if(i > 0 && j > 0)
    {
        if(secMatrix[i][j] == ( secMatrix[i-1][j-1] + ( (cadena1[i] == cadena2[j]) ? 1 : -1) ) )
        {
            aliment += cadena2[j];
            trace_back(i-1,j-1,aliment);
        }
    }

    aliment = temp;
    if(j>0)
    {
        if(secMatrix[i][j]  ==  secMatrix[i][j-1] - 2)
        {
            aliment += "-";
            trace_back(i,j-1,aliment);
        }

    }
    aliment = temp;
    if(i>0)
    {
        if(secMatrix[i][j]  ==  secMatrix[i-1][j] - 2)
        {

            aliment += "-";
            trace_back(i-1,j,aliment);
        }

    }
}

void print_alineamientos()
{
    for(int i = 0 ; i < cadenaFinal.size() ; ++i ){
        reverse(cadenaFinal[i].begin() , cadenaFinal[i].end() );
        cout<<cadenaFinal[i].c_str()<<endl;
    }
}


/*Alineamiento Estrella */
vector<string> alineamientos_msa; //Vector que almacena todos los alineamientos a alinear
int matriz_score_pares()
{
    vector<int> matrix_score;
    int score_matriz_estrella = 0;
    int tam_matriz = alineamientos_msa.size();
    for(int i=0;i<tam_matriz;i++)
    {
        for(int j=0;j<tam_matriz;j++)
        {
            if(i == j)
            {
                score_matriz_estrella += 0; //Diagonal no cuenta
                //cout<<"El score  es: "<<0<<endl;
            }
            else
            {
                secMatrix = init(alineamientos_msa[i],alineamientos_msa[j]);
                //cout<<"El score  es: "<<secMatrix[alineamientos_msa[i].size()][alineamientos_msa[j].size()]<<endl;
                score_matriz_estrella += secMatrix[alineamientos_msa[i].size()][alineamientos_msa[j].size()];
                secMatrix.clear();
                cadena1 = "-"; cadena2 = "-";
            }

        }
        cout<<"El score Total es: "<<score_matriz_estrella<<endl;
        matrix_score.push_back(score_matriz_estrella);
        score_matriz_estrella = 0;
    }
    int major =-5000;
    int pos_cadena = -1;
    for(int pos=0;pos<matrix_score.size();pos++)
    {
        if(matrix_score[pos] > major)
        {
            major = matrix_score[pos];
            pos_cadena = pos;
        }

    }
    cout<<"La cadena Centro es: "<<pos_cadena+1<<endl;
    return pos_cadena;  //Retorna la cadena Centro
}

/*Construyendo Alineamiento múltiple*/
vector<string> msa_results;
void building_msa(int centro)
{
        string cadena_centro = alineamientos_msa[centro];
    msa_results.push_back(cadena_centro);
    for(int i=0;i<alineamientos_msa.size();i++)
    {
        if(i != centro)
        {
            secMatrix = init(cadena_centro,alineamientos_msa[i]);
            trace_back(cadena_centro.size(),alineamientos_msa[i].size(),"");
            reverse(cadenaFinal[0].begin() , cadenaFinal[0].end() );             //Agregamos el primer alinemiento del centro vs el resto
            msa_results.push_back(cadenaFinal[0].c_str());
            //Limpiamos
            secMatrix.clear();
            cadena1="-"; cadena2="-";
            cadenaFinal.clear();
            cantALin = 0;

        }
    }

    int string_major_lenth = 0;
    for(int i=0;i<msa_results.size();i++)
    {
        cout<<msa_results[i]<<endl;
        if(msa_results[i].size() > string_major_lenth)
        {
            string_major_lenth = msa_results[i].size();
        }

    }

    //cout<<string_major_lenth<<endl;
    int gaps_num = 0;
    for(int i=0;i<msa_results.size();i++)
    {

        if(msa_results[i].size()<string_major_lenth)
        {
            gaps_num = string_major_lenth - msa_results[i].size();
            for(int j=0;j<gaps_num;j++)
                msa_results[i].push_back('-');
            gaps_num = 0;
        }
        cout<<msa_results[i]<<endl;
    }

}

/*Reglas*/
int _sc(char a, char b)
{
    if(a == '-' and b == '-') return 0;
    else if(a == '-' or b == '-') return -2;
    else if(a == b) return 1;
    else if(a !=b ) return -1;
}

/*Suma de Pares*/
void suma_pares()
{
    int columnas = msa_results[0].size();
    int filas = msa_results.size();
    cout<<"Columnas "<<columnas<<"  filas "<<filas<<endl;
    //Por cada coumna de los alineamientos
    for(int i=0;i<columnas;i++)
    {
        for(int j=0;j<filas;j++)
        {

        }
    }
}
int main()
{
    clock_t t,t1;

    /*string cadena_1 = "ATTGCCATT";
    string cadena_2 = "ACTGACC";*/
    string cadena_1 = "ATTGCCATT";
    string cadena_2 = "ATGGCCATT";
    string cadena_3 = "ATCCAATTTT";
    string cadena_4 = "ATCTTCTT";
    string cadena_5 = "ACTGACC";

    alineamientos_msa.push_back(cadena_1);
    alineamientos_msa.push_back(cadena_2);
    alineamientos_msa.push_back(cadena_3);
    alineamientos_msa.push_back(cadena_4);
    alineamientos_msa.push_back(cadena_5);
    int pos_estrella = matriz_score_pares();

    building_msa(pos_estrella);
    suma_pares();
    /*t = clock();
    secMatrix = init(cadena_3,cadena_5);
    t = clock() - t;
    cout<<"El score es: "<<secMatrix[cadena_3.size()][cadena_5.size()]<<endl;*/

    //printMatrix(secMatrix);
    /*t1 = clock();
    trace_back(cadena_1.size(),cadena_2.size(),"");
    t1 = clock() - t1;*/
    /*cout<<cadena1<<endl;
    cout<<cadena2<<endl;*/

    /*cout<<"El score es: "<<secMatrix[cadena_1.size()][cadena_2.size()]<<endl;
    cout<<"La cantidad de Alineamientos Optimos es: "<<cantALin<<endl;
    printf ("Me tomo  (%f seconds) llenar la Matriz.\n",((float)t)/CLOCKS_PER_SEC);
    printf ("Me tomo  (%f seconds) el traceBack.\n",((float)t1)/CLOCKS_PER_SEC);
    print_alineamientos();*/


    return 0;
}
