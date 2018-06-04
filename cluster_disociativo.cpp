#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <iterator>
#include <fstream>
#include <sstream>
using namespace std;

vector <vector<string>> genes_matriz_string;
vector <vector<float>> genes_matriz;
vector <vector<float>> matriz_distancia;
vector <string> gen_nom;
vector<float> min_values;
vector<float> row_matrix;
vector<int> cluster_max_index;
vector<int> clusters_index;
vector<vector<int>> cluster_all_index; //Solo sirve para imprimir los grupos

void read_file()
{
    vector<string> fila;
    string nombre;
    string line;
    ifstream myfile ("/home/ucsp/CLionProjects/genes.txt");
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            //cout << line << '\n';
            istringstream iss(line);

            copy(istream_iterator<string>(iss),
                 istream_iterator<string>(),
                 back_inserter(fila));
            nombre = fila[0];
            gen_nom.push_back(nombre);
            fila.erase(fila.begin());
            nombre.clear();
            genes_matriz_string.push_back(fila);
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
        genes_matriz.push_back(fila_float);
        fila_float.clear();
    }
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

                }
            }
        }

    }

}



void min_row_value(int diagonal)
{
    float _min = 50000;
    for(int i=0;i<row_matrix.size();i++)
    {
        if( i != diagonal && row_matrix[i] < _min)
            _min = row_matrix[i];
    }
    min_values.push_back(_min);
}

void init()
{
    float _max = -10;
    int _index = 0;
    //Le mandamos fila a fila para que escoja el menor valor
    for(int i=0;i<matriz_distancia.size();i++)
    {
        row_matrix = matriz_distancia[i];
        min_row_value(i);
    }
    //Escogemos el índice que contenga el mayor valor
    for(int i=0;i<min_values.size();i++)
    {
        if(min_values[i] > _max)
        {
            _max = min_values[i];
            _index = i;
        }
    }
    cluster_max_index.push_back(_index);
    clusters_index.push_back(_index);
}

int iteracion = 0;
int num_cluster = 224;
//150 CLUSTERS CON K = 168 //200 CLUSTERS CON K = 224 //100 CLUSTERS CON K = 113 //50 CLUSTERS CON K = 59
void disociativo()
{
    //cout<<"k: "<<iteracion<<endl;
    iteracion += 1;
    //Calculamos la distancia del resto de índices a max cluster
    float _min = 5000;
    int _index = 0;
    bool _add = 0;
    //No se toma en cuenta los clusters
    std::vector<int>::iterator it;
    for(int i=0;i<matriz_distancia.size();i++)
    {
        it = find (clusters_index.begin(), clusters_index.end(), i);
        if (it == clusters_index.end()) //Si NO lo encontró , evalúa ese índice
        {
            for(int j=0;j<cluster_max_index.size();j++)
            {
                //Amalgamiento Simple
                if(matriz_distancia[i][j] < _min)
                {
                    _min = matriz_distancia[i][j];
                    _index = cluster_max_index[j];
                }
            }
            //--cout<<min_values[i]<<"    "<<matriz_distancia[i][_index]<<endl;
            if( (min_values[i] - matriz_distancia[i][_index]) >= 0)
            {
                cluster_max_index.push_back(i);
                clusters_index.push_back(i);
                _add = 1;
            }
            _min = 5000;
            _index = 0;

        }
        // if(i != _index)

    }

    //Verificando si hay un nuevo cluster que unir
    if(_add == 1 && iteracion < num_cluster)
    {
        //disociativo();
        //cout<<"Se unio el cluster"<<endl;
    }
    else if (_add == 0)
    {
        //No se toma en cuenta los clusters
        cluster_all_index.push_back(cluster_max_index);
        //Escogemos un nuevo max
        int _index_ = 0;
        float _max = -10;
        std::vector<int>::iterator it;
        for(int i=0;i<min_values.size();i++)
        {
            it = find (clusters_index.begin(), clusters_index.end(), i);
            if (it == clusters_index.end()) //Si NO lo encontró , evalúa ese índice
            {
                if(min_values[i]>_max)
                {
                    _max = min_values[i];
                    _index_ = i;
                }
            }
        }
        cluster_max_index.clear();
        cluster_max_index.push_back(_index_);
        clusters_index.push_back(_index_);
        if(iteracion < num_cluster)
        {
            //disociativo();
            //---cout<<"Se unio el cluster"<<endl;
        }
    }
    else
    {
        cout<<"Pos ninguna "<<endl;
        //disociativo();
    }
}

int res = 0;
void resto()
{
    std::vector<int>::iterator it;
    cout<<"---------------"<<endl;
    for(int i=0;i<832;i++)
    {
        it = find(clusters_index.begin(), clusters_index.end(), i);
        if (it == clusters_index.end())
        {
            res++;
            cout<<gen_nom[i]<<"   ";
        }
    }
    cout<<endl;
    cout<<"all: "<<res+clusters_index.size()<<endl;
}
void imprimir_cluster()
{
    cout<<"Tam: "<<cluster_all_index.size()<<endl;
    for(int i=0;i<cluster_all_index.size();i++)
    {
        cout<<"Cluster "<<i<<" : ";
        for(int j=0;j<cluster_all_index[i].size();j++)
        {
            cout<<gen_nom[cluster_all_index[i][j]%831]<<"   ";
            //cout<<cluster_all_index[i][j]<<"   ";
        }
        cout<<endl;
    }
}


int main()
{
    //150 CLUSTERS CON K = 168 //200 CLUSTERS CON K = 224 //100 CLUSTERS CON K = 113 //50 CLUSTERS CON K = 59
    clock_t t;
    //matriz_distancia = {{0,2.15,0.7,1.07,0.85,1.16,1.56},{2.15,0,1.53,1.14,1.38,1.01,2.83},{0.7,1.53,0,0.43,0.21,0.51,1.86},{1.07,1.14,0.43,0,0.29,0.22,2.04},{0.85,1.38,0.21,0.29,0,0.41,2.02},{1.16,1.01,0.55,0.22,0.41,0,2.05},{1.56,2.83,1.86,2.04,2.02,2.05,0}};
    //gen_nom = {"A","B","C","D","E","F","G"};
    read_file();
    string_to_float();
    gen_distancia();
    init();
    t = clock();
    disociativo();
    for(int i= 0;i<222;i++)
        disociativo();
    t = clock() - t;
    printf ("Me tomo  (%f seconds) realizar el procedimiento de Algortimo Disociativo.\n",((float)t)/CLOCKS_PER_SEC);
    imprimir_cluster();
    resto();
    cout<<"Salio"<<endl;
    return 0;
}
