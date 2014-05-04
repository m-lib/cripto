#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

#define PI 3.14159265
#define beta 14.28
#define alfa 9.0
#define dt 0.01

int k;
double x1t, x2t, x3t;

void inicializar() {
     x1t = 0.6;
     x2t = 0.2;
     x3t = 0.7001;
}

double f(double x1) {
       double a, b, c, ret;
       a = 2.0/7.0; b = -1.0/7.0; c = 1.0;
       ret = (b * x1) + ((0.5 * (a - b)) * (sqrt((x1 + c) * (x1 + c)) - sqrt((x1 - c) * (x1 - c))));
       
       return ret;
}

int chave(double x1, double x2, double x3) {
    double numero = 0.0;
    int cont = 0, chave = 0;
    
    //logica de geracao de chaves criptograficas
    numero = (sin(x1*PI) + cos(x2*PI) + tan(x3*PI));
        
    //converte a chave para um valor inteiro
    chave = int(numero * 80000000);
        
    //garante que a chave nao passe de 10.000.000
    chave = (chave % 10000000);
        
    //pega o valor absoluto da chave gerada, o que garante que as chaves nao sejam negativas
    chave = abs(chave);
        
    //algumas chaves podem ser menores que 1.000.000 e sao descartadas
    if (chave > 1000000) {
       return chave;//retorna a chave
    } else if (chave > 100000) {
       return (chave * 10);
    } else if (chave > 10000) {
       return (chave * 100);
    } else if (chave > 1000) {
       return (chave * 1000);
    } else if (chave > 100) {
       return (chave * 10000);
    } else if (chave > 10) {
       return (chave * 100000);
    } 

}

int chua(double x1, double x2, double x3) {
    x1t = x1 + ((alfa * (x2 - f(x1))) * dt);
    x2t = x2 + ((x1 - x2 + x3) * dt);
    x3t = x3 + ((-beta * x2) * dt);
         
    x1 = x1t;
    x2 = x2t;
    x3 = x3t;
         
    return chave(x1, x2, x3);
}

//versao de algoritimo de atkin utilizado para encontrar numeros primos de forma mais eficiente
int primo (int n) {
    int k, m, limite;  
  
    if ((n == 2) || (n == 3)) {
       return 1;
    } if ((n < 2) || ((n % 2) == 0) || ((n % 3) == 0)) {
         return 0;
    }
    
    limite = (int) ceil(sqrt (n));
    
    for (k = 1; ; k++) {  
        m = (6 * k) - 1;  
        if (m > limite) {
           break;
        } if ((n % m) == 0) {
             return 0;
        }
        
        m = (6 * k) + 1;  
        if (m > limite) {
           break;
        } if ((n % m) == 0) {
             return 0;
        }
    } return 1;  
}

int main(int num, char *parametro[]) {

	ifstream::pos_type size;
	char *memblock;

	inicializar();

	if (num < 3) {
		cout << "ERRO DE SINTAXE!\n";
		cout << "EXEMPLO:\n";
		cout << "cripto -c <arquivo.txt>\n";
	} else {

		string opcao = parametro[1];

		if (opcao.compare("-c") == 0) {

			ifstream file (parametro[2], ios::in|ios::binary|ios::ate);
			if (file.is_open()) {
    			size = file.tellg();
		    	memblock = new char[size];
    			file.seekg (0, ios::beg);
				file.read (memblock, size);

				for (int i = 0; i < size; i++) {
					k = chua(x1t, x2t, x3t);
    		    	if (primo(k)) {
						memblock[i] = (int(memblock[i]) - k) % 256;
					} else {
						memblock[i] = (int(memblock[i]) + k) % 256;
					}
				}
				
    			file.close();
	
			}
	
			ofstream out (parametro[2], ios::out|ios::binary|ios::trunc);
			out.write(memblock, size);
			out.close();
	
			delete[] memblock;
    	
			cout << "CIFRAGEM COMPLETA!\n";
	
		} else if (opcao.compare("-d") == 0) {
    	
		  	ifstream file (parametro[2], ios::in|ios::binary|ios::ate);
			if (file.is_open()) {
		    	size = file.tellg();
		    	memblock = new char[size];
		    	file.seekg (0, ios::beg);
				file.read (memblock, size);
		
				for (int i = 0; i < size; i++) {
					k = chua(x1t, x2t, x3t);
		        	if (primo(k)) {
						memblock[i] = int(memblock[i]) + (k % 256);
					} else {
						memblock[i] = int(memblock[i]) - (k % 256);
					}
				}
		
		    	file.close();
	
			}
	
			ofstream out (parametro[2], ios::out|ios::binary|ios::trunc);
			out.write(memblock, size);
			out.close();
	
			delete[] memblock;
	
			cout << "DECIFRAGEM COMPLETA!\n";
	
		} else {
			cout << "ERRO DE SINTAXE!\n";
			cout << "EXEMPLO:\n";
			cout << "cripto -c <arquivo.txt>\n";
		}
	
	}
	
   	return 0;
}
