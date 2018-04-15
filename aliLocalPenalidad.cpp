#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>

using namespace std;

int dummy = -500;
int score = -1;
int cantALin = 0;
vector<pair<int,int>> scores;
vector<vector<int> > tabla;
vector<string> alineamientos;
vector<string> alineamientos_penality;
string cadena1 = "-"; string cadena2 = "-";

int maximo(int a , int b , int c , int d)
{
    return std::max(std::max(a,b),std::max(c,d));
}

void iterativo(vector<vector<int> > &matrix, int m, int n)
{
    //Llenando espacios Prederminados de la Matriz
    matrix[0][0] = 0;
    for (int i = 1; i < n; i++) {
        matrix[0][i] = 0;
    }

    for (int j = 1; j < m; j++) {
        matrix[j][0] = 0;
    }

    for(int i=1;i<m;i++)
    {
        for(int j=1;j<n;j++)
        {
            matrix[i][j] = maximo(matrix[i][j-1] -2, matrix[i-1][j] -2, (cadena1[i] == cadena2[j] ? matrix[i - 1][j - 1] + 1 : matrix[i - 1][j - 1] - 1),0);
            if(matrix[i][j] >= score)
            {
                score = matrix[i][j];
            }
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
        matrix[i].resize(n,dummy);


    cadena1 += cadena_1; cadena2 += cadena_2;

    iterativo(matrix,m,n);
    //recursive(matrix,m-1,n-1);

    return matrix;
}

void  printAlineamientos()
{
    for(int i=0;i<alineamientos.size();i++)
    {
        reverse(alineamientos[i].begin() , alineamientos[i].end() );
        alineamientos_penality.push_back(alineamientos[i]);
        //cout<<alineamientos[i].c_str()<<endl;
    }
    alineamientos.clear();
}

void trace_back_1(int i, int j, string alineamiento) //Rompemos si '0' o '-'
{
    if(tabla[i][j] == 0) //Si llega a un '0' la cadena es correcta, si nos encontramos con - la cadena se descarta
    {
        cantALin += 1;
        alineamientos.push_back(alineamiento);
        alineamiento.clear();
        return;
    }
    if(i>0 && j>0)
    {
        if(tabla[i][j] == tabla[i-1][j-1] + (cadena1[i]==cadena2[j] ? 1 : -1) )
        {
            alineamiento += cadena2[j];
            trace_back_1(i-1,j-1,alineamiento);
        }
    }
    if(j>0)
    {
        if(tabla[i][j-1] -2 == tabla[i][j])
        {
            alineamiento.clear();
            return; //Porque vino de la izq y es gap
            //Aqui tu tmp lo limpias, la cadena se descarta ok.
        }

    }
    if(i>0)
    {
        if(tabla[i-1][j] -2 == tabla[i][j])
        {
            alineamiento.clear();
            return; //Porque vino de arriba y es gap
        }
    }
}

void trace_back_2(int i, int j, string alineamiento) //Aquí rompemos en (0,0)
{
    if(tabla[i][j] == 0)
    {
        cantALin += 1;
        alineamientos.push_back(alineamiento);
        alineamiento.clear();
        return;
    }
    if(j>0)
    {
        if(tabla[i][j-1] -2 == tabla[i][j])
        {
            alineamiento += "-";
            trace_back_2(i,j-1,alineamiento);
        }

    }
    if(i>0)
    {
        if(tabla[i-1][j] -2 == tabla[i][j])
        {
            alineamiento += "-";
            trace_back_2(i-1,j,alineamiento);
        }
    }
    if(i>0 && j>0)
    {
        if(tabla[i][j] == tabla[i-1][j-1] + (cadena1[i]==cadena2[j] ? 1 : -1) )
        {
            alineamiento += cadena2[j];
            trace_back_2(i-1,j-1, alineamiento);
        }
    }
}


void search_score()
{
    unsigned long m = cadena1.size();
    unsigned long n = cadena2.size();
    pair<int,int> pos;

    //Busco el score(s) mayores, y realizamos trace_back

    for(int i=0;i<m; i++)
    {
        for(int j=0;j<n;j++)
        {
            if(tabla[i][j] == score)
            {
                pos.first = i; pos.second = j;
                //cout<<"Pos score: "<<i<<"   "<<j<<endl;
                scores.push_back(pos);
            }

        }
    }

    for(int i=0; i<scores.size();i++)
    {
        //trace_back_1(scores[i].first,scores[i].second,"");
        trace_back_2(scores[i].first,scores[i].second,"");
        printAlineamientos();
       /* cout<<"-----------------------------------"<<endl;
        cout<<endl;*/
    }
}

vector<int>faults;
void penality(string cadena)
{
    int k = -1;
    for(int i=0;i<cadena.size();i++)
    {
        if(cadena[i]=='-')
        {
            k++;
            if(cadena [i+1] && cadena[i+1]!='-')
            {
                //cout<<"K: "<<k<<endl;
                faults.push_back(k);
                k = -1;
            }
        }

    }

}

int p = 2; int q = 1;
int penality_score()
{
    int tmp = 0;
    for(int i=0;i<faults.size();i++)
    {
        tmp += p + q*faults[i];
    }
    faults.clear();
    return tmp;
}

vector<int> faul_position;
void penality_init()
{
    int p_score; int num_ali_pen = 0;
    int menor_p_score = -dummy;
    for(int i=0;i<alineamientos_penality.size();i++)
    {
        penality(alineamientos_penality[i]);
        p_score = penality_score();
        //cout<<"score_p: "<<p_score<<endl;
        faul_position.push_back(p_score); //Se añade score de cadena 1 y asú sucesivamente
        if(p_score < menor_p_score)
            menor_p_score = p_score;
    }
    cout<<"La menor penalidad es: "<<menor_p_score<<endl;
    for(int j=0;j<faul_position.size();j++)
    {
        if(faul_position[j] == menor_p_score)
        {
            reverse(alineamientos_penality[j].begin() , alineamientos_penality[j].end() );
            cout<<alineamientos_penality[j].c_str()<<endl;
            num_ali_pen ++;
        }
    }
    cout<<"Num Alinemientos con menor Penalidad es: "<<num_ali_pen<<endl;
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

int main()
{
    clock_t t,t1;
   /* string c1 = "AGCT";
    string c2 = "GCA";*/
   /* string c1 = "ATGACGTGT";
    string c2 = "ATGTACTC";*/

    /*string c1 = "ACGGTGCAGTCACCAGGCGGTGCAGTCACCATAACGGTGCAGTCACCAGGCGGTGCAGTCACCAGCAACGGTGCAGTCACCAGGCGAAGTGCAGTCACC";
    string c2 = "ACGGTGCAGTCACCATTCGGTGCAGTCACCAAAAGGTGCATAACCAGGCGGTGCAGTCACCAGCAACTTTGCAGGGCAGGCGAAGTGCAGTCATT";*/
    string c1 = "ACGGTGCACAAGTTCACCAGTTGAACAAATTCGGTGCAGTCACCATAACGGTGCAGTCACCAGGCGGTGCAGTCACCCGGCGGTCGGTGCAGCATGCAAGCAACGGTGCAGTCACCAGGCACCAGTGCACAAGTTCACCAGTTTAACGAAGTGCAGTCACC";
    string c2 ="ACCGTGCAGTTGAACATCGGTGCAGAATTCGGTGCAGTCACCATAACGGTCGGTGCAGTGCAGTCACCAGGCGGTGCAGTCACCCGGCGGTCGGTGCAGCATGCAAGCAACGGTGCAGTCACCAGGCACCATCAGTCACCAGGCACCACCAGCGGTGCAG";
    t = clock();
    tabla = init(c1,c2);
    t = clock() - t;

    //printMatrix(tabla);

    t1 = clock();
    search_score(); //Comenta los print
    t1 = clock() - t1;

    cout<<"La cantidad de Alineamientos Optimos es: "<<cantALin<<endl;
    cout<<"El maximo score es: "<<score<<endl;
    printf ("Me tomo  (%f seconds) llenar la Matriz.\n",((float)t)/CLOCKS_PER_SEC);
    printf ("Me tomo  (%f seconds) el traceBack.\n",((float)t1)/CLOCKS_PER_SEC);


    penality_init();

    return 0;
}
