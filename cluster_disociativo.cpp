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
float _max = -0.001;

vector <vector<string>> genes_matriz_string;
vector <vector<float>> genes_matriz;
vector <vector<float >> matriz_distancia_original;
vector <vector<float >> matriz_distancia; //832 filas
vector<float > min_values;
vector<float > min_val_ori;
vector<float > min_values_update;
vector<float > new_distance;
vector<int> all_index;
void read_file()
{
    vector<string> fila;
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
            fila.erase(fila.begin());
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
void imprimir(vector<float > a)
{
    for(int i=0;i<a.size();i++)
        cout<<i<<" : "<<a[i]<<endl;
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

                }
            }
        }

        //t_inferior ++;
    }
    cout<<"T: "<<t_inferior<<endl;

    //Copiamos a Triangulo Superior
    //imprimir(matriz_distancia);

}

void min_element(int indx, vector<float> a)
{
    _min = 50000.0;
    for(int i=0;i<a.size();i++)
    {
        if ( i!= indx)
        {
            if ( a[i] < _min)
            {
                _min = a[i];
            }
        }
    }
    min_values.push_back(_min);


}

int k = 0;
void disociativo()
{

    int _indx = 0;
    int _indx_o = 0;
    vector<float> clusters;

    //Paso 2. Eligimos el elemento de mayor valor que comenzará la división Ejemp. G
    for(int i=0;i<min_values.size();i++)
    {
        if(min_values[i] > _max)
        {
            _max = min_values[i];
            _indx_o = i;
        }
    }

    //----cout<<"El menor valor es: "<<_max<<endl;

    //Buscamos a que índice esta asociado el mayor valor
    bool flag = 0;
    for(int i=0;i<min_val_ori.size();i++)
        if(flag == 0 &&_max == min_val_ori[i])
        {   _indx = i;
            flag = 1;
        }

    all_index.push_back(_indx);

    //Añadimos el índice de Ejemp. G al clúster
    clusters.push_back(_indx);


    //Paso 3. Calculamos las nuevas distancias con respecto a _indx Ejemp. G

    for(int i=0;i<matriz_distancia.size();i++)
    {
        if( i != _indx_o)
            new_distance.push_back(matriz_distancia[i][_indx]);
    }

    //Removemos Ejemp. G de min_values
    //cout<<"Indice: "<<_indx<<endl;
    min_values.erase (min_values.begin()+ _indx_o);
    //----cout<<"Distancia Principal sin Cluster"<<endl;
    //----imprimir(min_values);
    //----cout<<"Distancia con respecto al Nodo"<<endl;
    //----imprimir(new_distance);

    //Paso 4. Calculamos Dist Prin - Dist Nueva
    for(int i=0;i<min_values.size();i++)
    {
        //cout<<"resta: "<< min_values[i] <<"     "<<new_distance[i]<<endl;
        if ( min_values[i] - new_distance[i] >= 0)
        {
            //cout<<"i: "<<i<<endl;
            clusters.push_back(i + _indx); //Se añade otro elemento al clúster
            //Añade al all index
            all_index.push_back(i + _indx);
        }
        else
        {
            min_values_update.push_back(min_values[i]); //Para actualizar la matriz de dis principales
        }
    }


    //Actualizmos la matriz de dis principales
    cout<<"Los cluster en K = "<<k+1<<" son:"<<endl;
    imprimir(clusters);
    min_values.clear();
    min_values = min_values_update;
    //imprimir(min_values);

    //Actualizamos la Matriz de Distancias Eliminando las filas del cluster
    matriz_distancia.clear();
    for(int i=0;i<matriz_distancia_original.size();i++)
    {
        std::vector<int>::iterator it;
        it = find (all_index.begin(), all_index.end(), i);
        if (it == all_index.end()) //Si NO lo encontró , agrega esa fila
        {
            matriz_distancia.push_back(matriz_distancia_original[i]);
        }

    }
    /*cout<<"-----------Matriz-----------------"<<endl;
    int t_inf = 0;
    for(int i=0;i<matriz_distancia.size();i++)
    {
        for(int j=0;j<matriz_distancia.size();j++)
        {
            cout<<matriz_distancia[i][j]<<"     ";
        }
        cout<<endl;
        t_inf++;

    }
    cout<<"----------------------------"<<endl;*/
    //matriz_distancia.erase (matriz_distancia.begin() + _indx_o);

    //Limpiamos
    _max = -0.001;
    min_values_update.clear();
    new_distance.clear();
    clusters.clear();

    //Repetimos el procedimiento
    k++;
    if( min_values.size() > 50)
        disociativo();

    //Falta : Nombre de cluster correcto o al menos indices y ver que sucede > 1 ...e
}

void resto_indices()
{
    int m = 832;
    k += 1;
    cout<<"Los cluster en k = "<<k<<" son:"<<endl;
    for(int i=0;i<m;i++)
    {
        std::vector<int>::iterator it;
        it = find (all_index.begin(), all_index.end(), i);
        if (it == all_index.end()) //Si NO lo encontró , agrega esa fila
        {
            cout<<"i: "<<i<<endl;
        }

    }

}

void init()
{

}
int main() {
    //clock_t t;
    read_file();
    string_to_float();
    gen_distancia();
    //t = clock();


    //matriz_distancia = {{0,2.15,0.7,1.07,0.85,1.16,1.56},{2.15,0,1.53,1.14,1.38,1.01,2.83},{0.7,1.53,0,0.43,0.21,0.51,1.86},{1.07,1.14,0.43,0,0.29,0.22,2.04},{0.85,1.38,0.21,0.29,0,0.41,2.02},{1.16,1.01,0.55,0.22,0.41,0,2.05},{1.56,2.83,1.86,2.04,2.02,2.05,0}};
    matriz_distancia_original = matriz_distancia;
    //Paso 1. Vector de min distancias
    for(int i=0;i<matriz_distancia.size();i++)
    {
        min_element(i,matriz_distancia[i]);
    }
    min_val_ori = min_values;
    //imprimir(min_val_ori);
    disociativo();
    resto_indices();
    //t = clock() - t;
//    printf ("Me tomo  (%f seconds) realizar el procedimiento de Amalgamiento simple.\n",((float)t)/CLOCKS_PER_SEC);

    return 0;
}
