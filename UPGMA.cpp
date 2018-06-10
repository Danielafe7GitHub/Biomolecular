#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <math.h>
#include <time.h>
using namespace std;

vector <vector<string>> genes_matriz_string;
vector <vector<float>> genes_matriz;
vector <vector<float>> matriz_distancia;
vector <vector<float>> new_matrix;
vector<vector <string>> gen_nom;
vector<vector <int>> map_indxs;
vector<int> all_indxs;
vector<vector<int>> cluster_all_index; //Solo sirve para imprimir los clústers

int indx_m, indx_n,first,second;


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

//Optimizado: Solo calcula Triangulo Inferior (Se calcula una única vez)
void gen_distancia()
{
    int m = 832;
    int n = 832;
    float _min = 50000.0;
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

    }

}

//Ponemos nuestros índices como map
void mapeo_indices()
{
    vector<int> tmp;
    for(int i=0;i<matriz_distancia[0].size();i++)
    {
        tmp.push_back(i);
        map_indxs.push_back(tmp);
        tmp.clear();
    }
}

//Imprimir map
void imprimir_map()
{
    for(int i=0;i<map_indxs.size();i++)
    {
        for(int j=0;j<map_indxs[i].size();j++)
            cout<<map_indxs[i][j]<<"    ";
        cout<<endl;
    }
}



void UPGMA()
{
    //Hallamos la distancia con respecto al cluster (C-A)
    vector<float> new_row;
    float distancia;
    int num_elementos = map_indxs[first].size();
    std::vector<int>::iterator it;
    for(int i=0;i<matriz_distancia.size();i++) {
        if(i == first)
            new_row.push_back(0); //Añadiendo diagonal (0 )a la nueva fila
        it = find(all_indxs.begin(), all_indxs.end(), i);
        //Id qué indices son clustes y no los recorras
        if (it == all_indxs.end())
        {
            //Halla la distancia con respecto al cluster (Se unen en la pos [first]
            for(int j=0;j<map_indxs[first].size();j++)
            {
                distancia += matriz_distancia[i][map_indxs[first][j]];
            }
            new_row.push_back(distancia/num_elementos);
            distancia = 0;
        }
    }
    //Imprimientdo new_row
    /*for(int i=0;i<new_row.size();i++)
        cout<<new_row[i]<<"     ";
    cout<<endl;*/
    //Revisar, incluir en la nuw fila el 0 de la diagonal, crear una nueva matriz sin mod la anterior
    std::vector<int>::iterator it_2;
    vector<float> tmp_row;

    //Construyendo nueva matríz de Distancias
    //1. Elimanos la fila  columna [second] Ejempl. Todo C fila y columna
    new_matrix.erase(new_matrix.begin() + second);
   for(int i=0;i<new_matrix.size();i++)
   {
       new_matrix[i].erase(new_matrix[i].begin()+second);
   }

   //2. Añadimos new_row a la matriz. Ejemp. Añadimos AC en donde estaba A
    new_matrix[first] = new_row;
    for(int i=0;i<new_matrix.size();i++)
        new_matrix[i][first] = new_row[i];


   //Limpiamos
    new_row.clear();
    /*cout<<"Matriz"<<endl;
    int t_inf = 0;
    for(int i=0;i<new_matrix.size();i++)
    {
        for(int j=0;j<new_matrix[i].size();j++)
            cout<<new_matrix[i][j]<<"   ";
        cout<<endl;
        t_inf++;
    }*/

}


//Obtenemos los índices del menor elemento . Ejemp. C-A :7, menor elemento
void min_element()
{
    int t_inferior = 0;
    float _min = 50000.0;

    //Preguntamos solo al triángulo Inferior
    for(int i=0;i<new_matrix.size();i++)
    {
        for(int j=0;j<t_inferior;j++)
        {
            //cout<<new_matrix[i][j]<<"    ";
            if(new_matrix[i][j] < _min)
            {
                _min = new_matrix[i][j];
                indx_m = i; indx_n =j;
            }
        }
        t_inferior ++;
        //cout<<endl;
    }
    all_indxs.push_back(indx_m);all_indxs.push_back(indx_n); //Añadimos los índices del nuevo cluster para no recorrerlos
    //cout<<"El menor elemento es: "<<_min<<" en: "<<gen_nom[indx_m][0]<<" - "<<gen_nom[indx_n][0]<<endl;
    //imprimir_map();

    //Modificamos nuestro map
    first = min(indx_m,indx_n); //En esta posición se añade el nuevo índice
    second = max(indx_m,indx_n);//Esta posición deberá eliminarse de map
    //---cout<<"El menor elemento es: "<<_min<<" en: "<<first<<" - "<<second<<endl;
    for(int i=0;i<map_indxs[second].size();i++)
    {
        map_indxs[first].push_back(map_indxs[second][i]);
    }

    //Añadimos los grupos, para imrimirlos luego.
    cluster_all_index.push_back(map_indxs[first]);

    //--YA NO: map_indxs.erase(map_indxs.begin()+second);
    //imprimir_map();
    UPGMA();
}

void imprimir_cluster()
{
    cout<<"Tam: "<<cluster_all_index.size()<<endl;
    for(int i=0;i<cluster_all_index.size();i++)
    {
        cout<<"Cluster "<<i<<" : ";
        for(int j=0;j<cluster_all_index[i].size();j++)
        {
            cout<<gen_nom[cluster_all_index[i][j]][0]<<"   ";

        }
        cout<<endl;
    }
}

int main() {
    clock_t t;
    read_file();
    string_to_float();
    gen_distancia();

/*    matriz_distancia = {{0,8,7,12},{8,0,9,14},{7,9,0,11},{12,14,11,0}};
    gen_nom = {{"A"},{"B"},{"C"},{"D"}};*/


    new_matrix = matriz_distancia; //Ahora solo mod new_matrix y sacamos las distancias de la matriz original
    mapeo_indices();
    t = clock();
    for(int i =0;i<200;i++)
        min_element();
    t = clock() - t;
    printf ("Me tomo  (%f seconds) realizar el procedimiento UPGMA.\n",((float)t)/CLOCKS_PER_SEC);
    imprimir_cluster();

    return 0;
}
