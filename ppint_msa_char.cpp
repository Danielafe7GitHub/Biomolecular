#include <iostream>
#include <vector>
#include <algorithm>
#include "sys/types.h"
#include "sys/sysinfo.h"

struct sysinfo memInfo;
using namespace std;

//Score Matrix
vector<int> row_1;
vector<int> row_2;

//string cadena_1 = "-AAAC";
//string cadena_2 = "-AGC";

/*
string cadena_1 = "-ACGGTGCACAAGTTCACCAGTTGAACAAATTCGGTGCAGTCACCATAACGGTGCAGTCACCAGGCGGTGCAGTCACCCGGCGGTCGGTGCAGCATGCAAGCAACGGTGCAGTCACCAGGCACCAGTGCACAAGTTCACCAGTTTAACGAAGTGCAGTCACC";
string cadena_2 = "-ACCGTGCAGTTGAACATCGGTGCAGAATTCGGTGCAGTCACCATAACGGTCGGTGCAGTGCAGTCACCAGGCGGTGCAGTCACCCGGCGGTCGGTGCAGCATGCAAGCAACGGTGCAGTCACCAGGCACCATCAGTCACCAGGCACCACCAGCGGTGCAG";
*/

string cadena_1 = "";
string cadena_2 = "";

string alineamiento;


int pos = 0; //cambialo a bit
char _pos;
//Poiting Matrix
vector<vector<char>> matrix; //Debe ser del tipo bit

//Vectores para MSA
vector<string> alineamientos_msa; //Vector que almacena todos los alineamientos a alinear
vector<string> msa_results;

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
    /*cout<<"c1: "<<cadena_1<<endl;
    cout<<"c2: "<<cadena_2<<endl;*/
    int m = cadena_1.size();
    int n = cadena_2.size();
    //Inicializando la matriz de score
    for(int i=0;i<n;i++)
    {
        row_1.push_back(i * -2);
        row_2.push_back((i +1) * -2);
    }



    //Inicializando pointting matrix
    vector<char> tmp;
    for ( int i = 0 ; i < m ; i++ )
    {
        for(int j = 0;j<n;j++)
            tmp.push_back('0');
        matrix.push_back(tmp);
        tmp.clear();
    }

    for (int i = 1; i < n; i++) {
        matrix[0][i] = '3';
    }

    for (int j = 1; j < m; j++) {
        matrix[j][0] = '1';
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
    int m = cadena_1.size();
    int n = cadena_2.size();
    //Calculamos Matriz de scores y Poitting Matrix al mismo tiempo
    //--cout<<"m: "<<m<<" n: "<<n<<endl;
    int _c = 2;
    for(int i=1;i<m;i++)
    {
        for(int j=1;j<n;j++)
        {
            row_2[j] = maximo((cadena_1[i] == cadena_2[j] ? row_1[j - 1] + 1 : row_1[j - 1] - 1),
                              row_2[j - 1] - 2, row_1[j] - 2);
            _pos = pos + '0';
            matrix[i][j] = _pos;
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
    int m = cadena_1.size();
    int n = cadena_2.size();
/*    for(int i=0;i<m;i++)
    {
        for(int j=0;j<n;j++) {
            cout<<matrix[i][j]<<"   ";
        }
        cout<<endl;
    }
    cout<<"c1: "<<cadena_1<<endl;
    cout<<"c2: "<<cadena_2<<endl;*/

    //Recorremos las columnas (tam cadena 2, la que queremos alinear)
    int fila = m - 1;
    int columna = n - 1;
    int pos = n - 1;
    for(int i=n-1;i>=0;i--)
    {
        if(matrix[fila][columna] == '2') //Diagonal
        {
            alineamiento.push_back(cadena_2[pos]);
            fila -= 1;
            columna -= 1;
            pos--;
        }
        else if(matrix[fila][columna] == '3') //Horizontal
        {
            alineamiento.push_back('-');
            columna -= 1;
            pos--;
        }
        else if(matrix[fila][columna] == '1') //Vertical
        {
            alineamiento.push_back('-');
            fila -= 1;
            i++;//Nos quedamos quietos
        }
    }
/*    cout<<"alineamiento : : "<<alineamiento<<endl;
    cout<<"-----------------"<<endl;*/

}


/*Alineamiento Estrella :  Hallando cadena centro*/
int matriz_score_pares()
{
    vector<int> matrix_score;
    int score_matriz_estrella = 0;
    int tam_matriz = alineamientos_msa.size();
    cadena_1="-"; cadena_2="-";
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
                cadena_1 += alineamientos_msa[i];
                cadena_2 += alineamientos_msa[j];
                /*cout<<"c1: "<<cadena_1<<endl;
                cout<<"c2: "<<cadena_2<<endl;
                cout<<"------------------------"<<endl;*/

                init();
                global_aligment();
                //cout<<"alineamiento: "<<alineamiento<<endl;
                //cout<<"El score  es: "<<secMatrix[alineamientos_msa[i].size()][alineamientos_msa[j].size()]<<endl;
                //score_matriz_estrella += matrix[alineamientos_msa[i].size()][alineamientos_msa[j].size()];
                score_matriz_estrella += row_1[cadena_2.size()-1];
                //Limpiamos Vectores para los siguientes alineamientos
                row_1.clear(); row_2.clear();
                matrix.clear();
                cadena_1="-"; cadena_2="-";

            }

        }
        //cout<<"El score Total es: "<<score_matriz_estrella<<endl;
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
    //cout<<"La cadena Centro es: "<<pos_cadena+1<<endl;
    return pos_cadena;  //Retorna la cadena Centro
}

/*Construyendo Alineamiento múltiple*/

void building_msa(int centro)
{
    cadena_1="-"; cadena_2="-";
    string cadena_centro = alineamientos_msa[centro];
    msa_results.push_back(cadena_centro);
    for(int i=0;i<alineamientos_msa.size();i++)
    {
        if(i != centro)
        {
            cadena_1 += cadena_centro;
            cadena_2 += alineamientos_msa[i];
            init();
            global_aligment();
            traice_back();
            reverse(alineamiento.begin() , alineamiento.end() );             //Agregamos el primer alinemiento del centro vs el resto
/*            cout<<"c1: "<<cadena_1<<endl;
            cout<<"c2: "<<cadena_2<<endl;
            cout<<"El alinemaiento es: "<<alineamiento<<endl;
            cout<<"-----------------"<<endl;*/
            msa_results.push_back(alineamiento.c_str());
            //Limpiamos Vectores para los siguientes alineamientos
            alineamiento.clear();
            row_1.clear(); row_2.clear();
            matrix.clear();
            cadena_1="-"; cadena_2="-";


        }
    }

    //Función que encuentra e alineamiento + largo para "Once gap, always gap"
    int string_major_lenth = 0;
    for(int i=0;i<msa_results.size();i++)
    {
        //cout<<msa_results[i]<<endl;
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
        cout<<"Los alineamientos finales son:"<<endl;
        cout<<msa_results[i]<<endl;
        cout<<endl;
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
    int score_total = 0;
    //cout<<"Columnas "<<columnas<<"  filas "<<filas<<endl;
    //Por cada coumna de los alineamientos
    for(int i=0;i<columnas;i++)
    {
        for(int j=0;j<filas;j++)
        {
            for(int k=j+1; k<filas;k++)
            {
                //cout<<msa_results[j][i]<<" vs "<<msa_results[j][i]<<endl;
                score_total += _sc(msa_results[j][i],msa_results[j][i]);

            }
        }
        //cout<<endl;
        //cout<<"----------------------------------"<<endl;
    }
    cout<<"El score total es: "<<score_total<<endl;
}
int main()
{
/*    string c1 = "ATTGCCATT";
    string c2 = "ATGGCCATT";
    string c3 = "ATCCAATTTT";
    string c4 = "ATCTTCTT";
    string c5 = "ACTGACC";*/

    string c1 = "ACGGTGAGTGATCACCTAGCACAGGAGTGATCACCTAACGGTGCAGTCACCCGGCGGAGAGCGGTGCAGTCACCTCGGTGCAGCATGCAAGC";
    string c2 = "AACGGTGCAGTAGACGAGTGATCACCTACAGGCACCAGTGCACAAGTTCACCAGTTTAACGAAGTGCAGTCACCAGTTGAACA";
    string c3 = "AATTCGGTGCAGTCACCACACCTACACCTAACGGACGGAGTGAGTGATCACCTAGCACACCTACACCTAACGGACGG";
    string c4 = "ACCTACAAGTCTAAAGTCTAAGTGGAACACCTCTACGAGTGATCACCTAACGCACTAACGGACGGACTCGGCGGACGCGGACGGACTGACT";
    string c5 = "AAACCTAACGGTTCGGTGCAGTCACCACTAACGACCTAGCACACCATCACCTATTGGGAAACGG";
    string c6 = "TACACGGAGTGAGTGAACGACGATCAATCACCTATCATCACCGGGATAATCACCTGGGAATTCTAT";
    string c7 = "CACCTAAGGGTATAACGTCGAACCGAATAACATCATCACATCACCTATCTATGTGTAATCACGCTATGGGGACCTATGCGG";
    string c8 = "CTACAGGCACCAGTCACCTAGCACGGAGAGCGGTGCAGAGGAATAATCACCTGG";
    string c9 = "ACCTCAACCTCAGGCAACCCAACACGGCAACCCCTCAGGCAACCCAACAC";
    string c10 = "AACACACCAACCAACCTCAGGCTCAGGCCGAGGAGAGAGCCTCAGGCCCTCACCCTCAGGCGGC";

    alineamientos_msa.push_back(c1);
    alineamientos_msa.push_back(c2);
    alineamientos_msa.push_back(c3);
    alineamientos_msa.push_back(c4);
    alineamientos_msa.push_back(c5);
    alineamientos_msa.push_back(c6);
    alineamientos_msa.push_back(c7);
    alineamientos_msa.push_back(c8);
    alineamientos_msa.push_back(c9);
    alineamientos_msa.push_back(c10);

    int pos_estrella = matriz_score_pares();
    cout<<"El centro es: "<<alineamientos_msa[pos_estrella]<<endl;
    building_msa(pos_estrella);
    suma_pares();


    sysinfo (&memInfo);
    long long totalVirtualMem = memInfo.totalram;
    //Add other values in next statement to avoid int overflow on right hand side...
    totalVirtualMem += memInfo.totalswap;
    totalVirtualMem *= memInfo.mem_unit;
    long long virtualMemUsed = memInfo.totalram - memInfo.freeram;
    //Add other values in next statement to avoid int overflow on right hand side...
    virtualMemUsed += memInfo.totalswap - memInfo.freeswap;
    virtualMemUsed *= memInfo.mem_unit;
    cout<<"Memoria disponible : "<<totalVirtualMem<<endl;
    cout<<"Memoria usada : "<<virtualMemUsed<<endl;



    //Falta pasar a char y medir memoria :)

    return 0;
}
