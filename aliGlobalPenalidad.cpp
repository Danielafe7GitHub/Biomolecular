#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>
using namespace std;

string cadena1 = "-"; string cadena2 = "-";
int dummy = -5000;
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
            //cout<<"Yo vengo de: "<<i<<j<<"  "<<cadena2[j-1]<<endl;
            aliment += cadena2[j];
            trace_back(i-1,j-1,aliment);
        }
    }

    aliment = temp;
    if(j>0)
    {
        if(secMatrix[i][j]  ==  secMatrix[i][j-1] - 2)
        {
            //cout<<"Yo secMatrix de: "<<i<<j<<"  <--"<<endl;
            aliment += "-";
            trace_back(i,j-1,aliment);
        }

    }
    aliment = temp;
    if(i>0)
    {
        if(secMatrix[i][j]  ==  secMatrix[i-1][j] - 2)
        {

            //cout<<"Yo vengo de: "<<i<<j<<"  |"<<endl;
            aliment += "-";
            trace_back(i-1,j,aliment);
        }

    }
}

//Ojo aquí las cadenas entran al revéz, para evitar problema de que no se agrega al vector si termina en -
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
    for(int i=0;i<cadenaFinal.size();i++)
    {
        penality(cadenaFinal[i]);
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
            reverse(cadenaFinal[j].begin() , cadenaFinal[j].end() );
            cout<<cadenaFinal[j].c_str()<<endl;
            num_ali_pen ++;
        }
    }
    cout<<"Num Alinemientos con menor Penalidad es: "<<num_ali_pen<<endl;
}

int main()
{
    clock_t t,t1;
    /*string cadena_1 = "ACTGATTCA";
    string cadena_2 = "ACGCATCA";*/
    /*string cadena_1 = "AAAC";
    string cadena_2 = "AGC";*/

    string cadena_1 = "ACGGTGCACAAGTTCACCAGTTGAACAAATTCGGTGCAGTCACCATAACGGTGCAGTCACCAGGCGGTGCAGTCACCCGGCGGTCGGTGCAGCATGCAAGCAACGGTGCAGTCACCAGGCACCAGTGCACAAGTTCACCAGTTTAACGAAGTGCAGTCACC";
    string cadena_2 = "ACCGTGCAGTTGAACATCGGTGCAGAATTCGGTGCAGTCACCATAACGGTCGGTGCAGTGCAGTCACCAGGCGGTGCAGTCACCCGGCGGTCGGTGCAGCATGCAAGCAACGGTGCAGTCACCAGGCACCATCAGTCACCAGGCACCACCAGCGGTGCAG";
    /*string cadena_1 = "ACGGTGCAGTCACCAGGCGGTGCAGTCACCATAACGGTGCAGTCACCAGGCGGTGCAGTCACCAGCAACGGTGCAGTCACCAGGCGAAGTGCAGTCACC";
    string cadena_2 = "ACGGTGCAGTCACCATTCGGTGCAGTCACCAAAAGGTGCATAACCAGGCGGTGCAGTCACCAGCAACTTTGCAGGGCAGGCGAAGTGCAGTCATT";
*/

    t = clock();
    secMatrix = init(cadena_1,cadena_2);
    t = clock() - t;

    //printMatrix(secMatrix);
    t1 = clock();
    trace_back(cadena_1.size(),cadena_2.size(),"");
    t1 = clock() - t1;
    /*cout<<cadena1<<endl;
    cout<<cadena2<<endl;*/

    cout<<"El score es: "<<secMatrix[cadena_1.size()][cadena_2.size()]<<endl;
    cout<<"La cantidad de Alineamientos Optimos es: "<<cantALin<<endl;
    printf ("Me tomo  (%f seconds) llenar la Matriz.\n",((float)t)/CLOCKS_PER_SEC);
    printf ("Me tomo  (%f seconds) el traceBack.\n",((float)t1)/CLOCKS_PER_SEC);

    penality_init();

    //Imprime Alineamientos
    /*for(int i = 0 ; i < cadenaFinal.size() ; ++i ){
        reverse(cadenaFinal[i].begin() , cadenaFinal[i].end() );
        printf("%s\n\n",cadenaFinal[i].c_str());
    }*/
    return 0;
}
