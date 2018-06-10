#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <math.h>
#include <time.h>
using namespace std;


float _min;
int indx_m = 0; int indx_n = 0;
vector <vector<string>> genes_matriz_string;
vector <vector<float>> genes_matriz;
vector <vector<float >> matriz_distancia; //832 filas
vector <vector<float >> matriz_distancia_update;
vector<vector <string>> gen_nom; //Nombre de los genes
void read_file()
{
    vector<string> fila;
    vector<string> nombre;
    string line;
    ifstream myfile ("/home/danielafe7/CLionProjects/cluster_min/genes.txt");
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            //cout << line << '\n';
            istringstream iss(line);

            copy(istream_iterator<string>(iss),
                 istream_iterator<string>(),
                 back_inserter(fila));

            nombre.push_back(fila[0]);
            gen_nom.push_back(nombre);
            fila.erase(fila.begin());
            nombre.clear();
            genes_matriz_string.push_back(fila);
            /*for(int i=0;i<fila.size();i++)
                cout<<fila[i]<<"    ";
            cout<<endl;*/
            fila.clear();

        }

    }
    myfile.close();
    cout<<"Num lineas "<<genes_matriz_string.size()<<endl;
}

void string_to_float()
{
    int m = 832;
    int n = 7;
    vector<float> fila_float;
    float valor;
    for (int fila=0;fila<m;fila++)
    {
        for (int elemento=0;elemento<n;elemento++)
        {
            valor =strtof((genes_matriz_string[fila][elemento]).c_str(),0); // string to float
            fila_float.push_back(valor);
        }
        /*for(int i=0;i<fila_float.size();i++)
                cout<<fila_float[i]<<"    ";
        cout<<endl;*/
        genes_matriz.push_back(fila_float);
        fila_float.clear();
    }
    cout<<"Num lineas "<<genes_matriz.size()<<endl;
}

float euclides(vector<float>a,vector<float>b)
{
    double distancia = 0;
    for(int i=0;i<a.size();i++)
    {
        distancia += pow((a[i] - b[i]),2);
    }
    distancia = sqrt(distancia);
    return distancia;
}
void imprimir(vector <vector<float >> a)
{
    int t_inferior = 1;
    for(int i=0;i<a.size();i++)
    {
        cout<<i<<" : ";
        for(int j=0;j<t_inferior;j++)
        {
            cout<<a[i][j]<<"    ";

        }
        cout<<endl;
        t_inferior ++;
    }
    cout<<"----------------------------------------"<<endl;
}

//Optimizado: Solo calcula Triangulo Inferior (Se calcula una única vez)
void gen_distancia()
{
    int m = 832;
    int n = 832;
    _min = 50000.0;
    int t_inferior = 0;
    vector<float> tmp;
    for(int i=0;i<n;i++)
        tmp.push_back(0);
    for(int i=0;i<m;i++)
        matriz_distancia.push_back(tmp);

    cout<<"Filas: "<<matriz_distancia.size()<<" colu: "<<matriz_distancia[0].size()<<endl;
    for(int i=0;i<matriz_distancia.size();i++)
    {
        for(int j=0;j<matriz_distancia.size();j++)
        {
            if(i != j)
            {
                matriz_distancia[i][j] = euclides(genes_matriz[i],genes_matriz[j]);
                if(matriz_distancia[i][j] < _min)
                {
                    _min = matriz_distancia[i][j];
                    indx_m = i; indx_n =j;
                }
            }
        }

        //t_inferior ++;
    }
    cout<<"T: "<<t_inferior<<endl;

    //Copiamos a Triangulo Superior
    //imprimir(matriz_distancia);

}

void min_element()
{
    int t_inferior = 0;
    _min = 50000.0;

    //Preguntamos solo al triángulo Inferior
    for(int i=0;i<matriz_distancia.size();i++)
    {
        for(int j=0;j<t_inferior;j++)
        {
            //cout<<matriz_distancia[i][j]<<"    ";
            if(matriz_distancia[i][j] < _min)
            {
                _min = matriz_distancia[i][j];
                indx_m = i; indx_n =j;
            }
        }
        t_inferior ++;
        //cout<<endl;
    }
}
void min_cluster()
{
    vector<float> union_clus;
    float value = 0;
    //matriz_distancia = {{0,2.15,0.7,1.07,0.85,1.16,1.56},{2.15,0,1.53,1.14,1.38,1.01,2.83},{0.7,1.53,0,0.43,0.21,0.51,1.86},{1.07,1.14,0.43,0,0.29,0.22,2.04},{0.85,1.38,0.21,0.29,0,0.41,2.02},{1.16,1.01,0.55,0.22,0.41,0,2.05},{1.56,2.83,1.86,2.04,2.02,2.05,0}};
    //gen_nom = {{"A"},{"B"},{"C"},{"D"},{"E"},{"F"},{"G"}};
    //imprimir(matriz_distancia);
    vector<float> tmp;


    int m = matriz_distancia.size();
    int n = matriz_distancia[0].size();
    //Unimos menor distancia (debajo de la diagonal)
    //min_element();

    for(int i=0;i<50;i++)//831|6
    {
        int first = min(indx_m,indx_n);
        int second = max(indx_m,indx_n);
        //cout<<"K = "<<i+1<<" Min Value: "<<_min<<" Unimos: "<<first<<" y "<<second<<endl;
         cout<<"Cluster : "<<i<<"   ";
         //cout<<"Unimos los genes: ";
         for(int k=0;k<gen_nom[first].size();k++)
             cout<<gen_nom[first][k]<<"    ";
         //cout<<"con  ";
         for(int k=0;k<gen_nom[second].size();k++)
             cout<<gen_nom[second][k]<<"    ";
         cout<<endl;
        //Actualizamos los nombres
        for(int k=0;k<gen_nom[second].size();k++)
            gen_nom[first].push_back(gen_nom[second][k]);
        gen_nom.erase(gen_nom.begin() + second);

        //Calculamos la nueva fila min (C vs A,B,D,F,G E vs A,B,D,F,G)
        for(int j=0;j<matriz_distancia.size();j++)
        {
            if(j == first)
                union_clus.push_back(0);
            else if(j != indx_n and j != indx_m)
            {
                value = min((matriz_distancia[j][indx_n]),(matriz_distancia[j][indx_m]));
                union_clus.push_back(value);
            }
        }

        /*for(int u=0;u<union_clus.size();u++)
            cout<<union_clus[u]<<"  ";
        cout<<endl;*/

        //Actualizamos la matriz de Distancias

        int pos = 0;
        for(int h=0;h<matriz_distancia.size();h++)
        {

            if(h == first)
            {
                /*tmp.push_back(union_clus[pos]);
                pos++;*/
                matriz_distancia_update.push_back(union_clus);
                pos++;
            }
            else if (h != first and h != second)
            {
                for(int j=0;j<matriz_distancia.size();j++)
                {

                    if(j == first)
                    {
                        tmp.push_back(union_clus[pos]);
                        pos++;
                    }
                    if(j != first and j != second)
                    {
                        tmp.push_back(matriz_distancia[h][j]);
                    }
                }
                matriz_distancia_update.push_back(tmp);
                tmp.clear();

            }
        }
        union_clus.clear();
        matriz_distancia.clear();
        matriz_distancia = matriz_distancia_update;
        matriz_distancia_update.clear();
        //-----------cout<<"Num filas "<<matriz_distancia.size()<<" Num col "<<matriz_distancia[0].size()<<endl;


        //Hallar el nuevo Min para unir
        //-----imprimir(matriz_distancia);
        min_element();

        //Imprimiendo Uniones
        /*for(int k=0;k<gen_nom.size();k++)
        {
            for(int j=0;j<gen_nom[k].size();j++)
            {
                cout<<gen_nom[k][j]<<"     ";
            }
            cout<<endl;
        }
        cout<<"---------------------------------------"<<endl;*/



    }


}

int main() {
    clock_t t;
    read_file();
    /*for(int i=0;i<gen_nom.size();i++)
        for (int j=0;j<gen_nom[i].size();j++)
            cout<<"n : "<<i<<"  "<<gen_nom[i][j]<<endl;*/

    string_to_float();
    gen_distancia();
    t = clock();
    min_cluster();
    t = clock() - t;
    printf ("Me tomo  (%f seconds) realizar el procedimiento de Amalgamiento simple.\n",((float)t)/CLOCKS_PER_SEC);
    //imprimir(matriz_distancia);
    return 0;
}
