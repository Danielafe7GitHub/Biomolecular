#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


vector <vector<float>> matriz_distancia;
vector <vector<float>> new_matrix;
vector<vector <string>> gen_nom;
vector<vector <int>> map_indxs;
vector<int> all_indxs;

int indx_m, indx_n,first,second;

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
//Obtenemos los índices del menor elemento . Ejemp. C-A :7, menor elemento
void min_element()
{
    int t_inferior = 0;
    float _min = 50000.0;

    //Preguntamos solo al triángulo Inferior
    for(int i=0;i<matriz_distancia.size();i++)
    {
        for(int j=0;j<t_inferior;j++)
        {
            cout<<matriz_distancia[i][j]<<"    ";
            if(matriz_distancia[i][j] < _min)
            {
                _min = matriz_distancia[i][j];
                indx_m = i; indx_n =j;
            }
        }
        t_inferior ++;
        cout<<endl;
    }
    all_indxs.push_back(indx_m);all_indxs.push_back(indx_n); //Añadimos los índices del nuevo cluster para no recorrerlos
    cout<<"El menor elemento es: "<<_min<<" en: "<<gen_nom[indx_m][0]<<" - "<<gen_nom[indx_n][0]<<endl;
    imprimir_map();

    //Modificamos nuestro map
    first = min(indx_m,indx_n); //En esta posición se añade el nuevo índice
    second = max(indx_m,indx_n);//Esta posición deberá eliminarse de map
    cout<<"El menor elemento es: "<<_min<<" en: "<<first<<" - "<<second<<endl;
    for(int i=0;i<map_indxs[second].size();i++)
    {
        map_indxs[first].push_back(map_indxs[second][i]);
    }

    map_indxs.erase(map_indxs.begin()+second);
    imprimir_map();
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
                cout<<"Pos: "<<matriz_distancia[i][map_indxs[first][j]]<<endl;
                distancia += matriz_distancia[i][map_indxs[first][j]];
            }
            new_row.push_back(distancia/num_elementos);
            distancia = 0;
        }
    }
    for(int i=0;i<new_row.size();i++)
        cout<<new_row[i]<<"     ";
    cout<<endl;
    //Revisar, incluir en la nuw fila el 0 de la diagonal, crear una nueva matriz sin mod la anterior
    std::vector<int>::iterator it_2;
    vector<float> tmp_row;


    cout<<"Matriz"<<endl;
    int t_inf = 0;
    for(int i=0;i<new_matrix.size();i++)
    {
        for(int j=0;j<new_matrix[i].size();j++)
            cout<<new_matrix[i][j]<<"   ";
        cout<<endl;
        t_inf++;
    }

}

int main() {

    matriz_distancia = {{0,8,7,12},{8,0,9,14},{7,9,0,11},{12,14,11,0}};
    gen_nom = {{"A"},{"B"},{"C"},{"D"}};
    mapeo_indices();
    min_element();
    UPGMA();
    cout<<"Salio"<<endl;
    return 0;
}
