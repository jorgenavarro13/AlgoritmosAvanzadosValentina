#include <bits/stdc++.h>
#include <filesystem>
using namespace std;

/*
    Jorge Arturo Montiel Navarro | A01278612
    Actividad 1.1
    Antes de correr o compilar este archivo asegurese que el cpp este dentro 
    de la carpeta con las instancias en txt a evaluar

    El comando a ejecutar para compilarlo y ejecutarlo son los siguientes

    g++ Actividad1_1.cpp -o ./Actividad1.exe 
    ./Actividad1.exe 
*/

namespace {  //Patrones y archivos base para comparar
    const string inputs_folder = "./instances";
    const string string_matching_patterns = "string-matching-Patrones.txt";
    const string string_matching_text = "string-matching-Texto.txt";
    const string input_dots_filename_pattern = "puntos-n";

    vector<int> current_array = {1, 2, 3, 4, 5};
    vector<int> maxSubarrayTest = {12, 4, -5, 3, 4, -8, 12, 14, 1, 10, 1, 5, 16};
}

struct closestPairResult{
    pair<float,float> p1;
    pair<float,float> p2;
    float distance;
};

/*
    readDotFile: 
        Funcion para leer los archivos de puntos, procesando el documento 
        y retornando un vector para el procesamiento.

    Complejidad temporal O(n)  -----> siendo n el numero de lineas a procesar
    Complejidad espacial O(n)  -----> ya que necesitamos guardar los pares en un arreglo de puntos
*/
vector<pair<float,float>> readDotFile(const string & file_name){
    ifstream file;
    file.open(file_name);
    int lines;
    file >> lines;
    vector<pair<float,float>> coordinates;
    for(int i=0; i<lines; i++){
        float x, y;
        file >> x; file>> y;
        coordinates.emplace_back(x,y);
    }
    file.close();
    return coordinates;
}

/*
    processFile: 
        Funcion de utilidad para procesar archivos de texto linea a linea y guardarlos en un arreglo

    Complejidad temporal O(n)  -----> siendo n el numero de lineas a procesar
    Complejidad espacial O(n)  -----> ya que necesitamos guardar los strings en un arreglo
*/
vector<string> processFile(const string & file_name){
    ifstream file;
    file.open(file_name);
    vector<string> patterns;
    string pattern;
    while (getline(file, pattern)) {
        patterns.push_back(pattern);
    }
    file.close();
    return patterns;
}


/*
    matchPatternOneLine: 
        Identifica un patron como substring de una linea y 
        retorna los indices en los que fue encontrado

    Complejidad temporal O(n*m)  -----> siendo n la longitud del string grande y m la longitud de patron
    Complejidad espacial O(1)  -----> solo necesitamos las variables de los indices de donde se encontró el string
*/
pair<int,int> matchPatternOneLine(const string & pattern, const string & line){
    int begin=0; int end = 0;
    for(int i=0;i<line.size();i++){
        for(int j=0;j<pattern.size();j++){
            if(i+j>=line.size() || line[i+j] != pattern[j] ) break;
            if(j+2==pattern.size()){  // To compensate the lack of removal of eof, we add 1 extra
                begin=i;
                end=j+i+1;
                break;
            }
        }
    }
    return make_pair(begin,end);
}

/*
    identifyMatchOfDotFile: 
        Utilidad para identificar y retornar archivos que cumplen con la nomeclatura correcta de
        archivo de puntos con base en un array de los archivos del directorio actual

    Complejidad temporal O(n*m)  -----> Utiliza la funcion de matchPatternOneLine
    Complejidad espacial O(m)  -----> Construye un substring con el nombre completo del archivo, siendo m la longitud de este archivo
*/
string identifyMatchOfDotFile(const string & file_name){
    pair<int,int> indexes = matchPatternOneLine(input_dots_filename_pattern, file_name);
    if(indexes.first == 0  &&  indexes.second == 0) return "";
    return file_name.substr(indexes.first);
}


/*
    closestPair: 
        Identifica de un vector de puntos en el plano cartesiano aquellos que estan a
        una distancia más corta y retorna sus indices y su distancia.

    Complejidad temporal O(n^2)  -----> Recorre el arreglo n veces y por cada una recorre los demás puntos, siendo un ciclo anidado de longitud n
    Complejidad espacial O(1)  -----> Solo se almacena la variable que incluye los puntos y distancia
*/
closestPairResult closestPair(vector<pair<float,float>> input){
    pair<float,float> cp1, cp2;

    float min_distance= INT_MAX;
    for(int base=0; base<input.size();base++){
        for(int compare = base+1; compare < input.size(); compare++ ){
            // ((x1-x2)² + (y1-y2)²) ^(1/2)
            float calculatedDistance = pow((pow((input[base].first - input[compare].first),2) + pow((input[base].second - input[compare].second),2)),0.5);
            if(calculatedDistance < min_distance){
                min_distance = calculatedDistance;
                cp1 = input[base]; cp2 = input[compare];
            }
        }
    }

    closestPairResult result; 
    result.p1 = cp1; result.p2 = cp2; result.distance=min_distance;
    return result;
}


/*
    reverseArrayPart: 
        Utilidad para voltear una seccion de un string

    Complejidad temporal O(n/2)  -----> Se recorre la mitad de el string haciendo swap con la otra mitad en un solo ciclo
    Complejidad espacial O(1)  -----> Solo se utiliza las variables temporales, modificaciones son in place
*/
void reverseArrayPart(vector<int> & items, int begin){
    int end=items.size()-1;
    
    while(begin < end){
        int temp = items[begin];
        items[begin] = items[end];
        items [end] = temp;
        begin++; end--;
    }
}   

/*
    generateNextPermutation:
    This is Narayana's algorithm, this gives the next permutation in lexicographic order
    1.- Find index i which items[i]< items[i+1]         ---> if is not finded, then you have the array sorted in decreasing order and no permutation would be generated
    2.- Find index j which items[j]> items[i]
    3.- Swap items[j] and items[i]
    3.- Reverse from items[i+1] to items[n-1]

    Complexity time O(n) -> The worst case is when we have the indexes i and j at the beggining of the array, 
    Space complexity O(n) -> We construct the next permutation on lexicographic order, with the size of the original input
    */
vector<int> generateNextPermutation(vector<int> items){
    for(int i=items.size()-2; i>=0; i--){
        if (items[i] < items[i+1]){  // Find i
            for(int j=items.size()-1; j>i; j--){
                if(items[j]>items[i]){ // Find j

                    // Swap items
                    int temp = items[j];
                    items[j]=items[i];
                    items[i]=temp;  
                
                    // Reverse part of array
                    reverseArrayPart(items, i+1);
                    return items;
                }
            }
        }
    }
    reverse(items.begin(),items.end());
    return items;
}

/*
    generateSubsets:
        This is an implementation of Binary Reflected Gray Code (BRGC), with the following steps
        1.-Start with 1 bit:[0, 1]
        2.-Reflect the items: ex-> [0, 1] becomes [0, 1, 1, 0]
        3-Add 0 to the first half, 1 to the second half
        4-Repeat

    Time Complexity O(2^n) ----> The number of subsets is multiplied each time by two, resulting in an exponential of 2
    Space Complexity O(2^n) ----> The number of subsets is stored on a vector
*/

vector<string> generateSubsets(int size_of_subset){
    if (size_of_subset == 0) return {};
    vector<string> subsets = {"0","1"};
    for(int i=0;i<size_of_subset;i++){
        int original_size=subsets.size();
        for(int i=0;i<original_size;i++){ // We do steps 2 and 3 in one pass
            string temp=subsets[i];
            subsets[i]+="0";
            reverse(temp.begin(),temp.end());
            temp+="1";
            subsets.push_back(temp);
        }
    }
    return subsets;
}


/*
    maxSubarray:
        This function returns the maximum sum of a subarray of ints, 

    Time Complexity O(n^2) ----> We have nested for loops of size n
    Space Complexity O(1) ----> Only temporal variables for the return value
*/
long long maxSubarray(vector<int> array){
    long long max_sum=0;
    long long current_sum=0;
    for(int i=0;i<array.size();i++){
        for(int j=i;j<array.size();j++){
            current_sum+=array[j];
            max_sum=max(current_sum,max_sum);
        }
        current_sum=0;
    }
    return max_sum;
}

int main(){
    cout<<"----------Distancia entre 2 puntos multi-archivo------"<<endl;
    vector<string> dotFiles;
    for( const auto & entry : filesystem::directory_iterator(inputs_folder)){
        string filtered_file = identifyMatchOfDotFile(entry.path());
        if(filtered_file != "") dotFiles.push_back(filtered_file);
    }
    
    
    for(string file_name : dotFiles){
        vector<pair<float,float>> coordinates = readDotFile(file_name);
        closestPairResult cp = closestPair(coordinates);
        cout<< "\n---"<<file_name<<"----"<<endl;
        cout<< "x1:"<<cp.p1.first<<" y1:"<<cp.p1.second<<", x2:"<<cp.p2.first<<" y2:"<<cp.p2.second<<endl;
        cout<< " Distance:"<<cp.distance<<endl;
    }
    
    cout<<"\n----Encontrar patrones(matchSubstring)-------"<<endl;
    vector<string> patterns = processFile(string_matching_patterns);
    vector<string> document = processFile(string_matching_text);
    for(int pattern=0; pattern<patterns.size();pattern++){
        bool found=false;
        for(int line=0; line<document.size();line++){
            pair<int,int> indexes = matchPatternOneLine(patterns[pattern], document[line]);
            if(indexes.first != 0 || indexes.second != 0) {
                string pattern = document[line].substr(indexes.first,indexes.second-indexes.first);
                cout<<"The pattern '"<<pattern<<"' first occurence was found on line "<<line+1<<" indexes ("<<indexes.first<<","<<indexes.second<<")"<<endl;
                found = true; 
                break;
            }
        }
        if(!found)cout<<"Not founded"<<endl;
    }
    
    cout<<"\n----------Generar permutaciones en orden lexicográfico------"<<endl;
    vector<int> nextPermutation = generateNextPermutation(current_array);
    for(int element : nextPermutation){cout<<element<<" ";}cout<<endl;

    while(current_array != nextPermutation){
        for(int element : nextPermutation){cout<<element<<" ";}cout<<endl;
        nextPermutation = generateNextPermutation(nextPermutation);    
    }

    cout<<"\n------Generar subsets----------"<<endl;
    vector<string> subsets = generateSubsets(nextPermutation.size());
    for(string subset : subsets){cout<<subset<<endl;} 

    cout<<"\n-------MaxSubarray-----------"<<endl;
    for(int num : maxSubarrayTest) cout<<num<<",";
    cout<<"The maximum sum of the array is:"<<maxSubarray(maxSubarrayTest)<<endl;
    return 0;
}


/*
    REFERENCIAS:
    https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
    https://es.stackoverflow.com/questions/576988/como-leer-archivos-en-c

    Ni chat ni claude le saben tanto, triste actividad hermosa con ayuda de stackoverflow y puro c++ de mis amores
*/