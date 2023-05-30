#include <iostream>
#include "String.cpp"

//Essa função recebe a linha de texto extraída do pdf e substitui os espaços entre cada informação por \t (Tab)
char *formatLine(char *line, char *crr_usc){
	char data[100];
	char ama[100];
	char nome[1000];
	char cns[100];
	bool proc = false;
	bool cons = false;
	
	
	int x = 0;
	while(line[x] != ' '){
		data[x] = line[x];
		++x;
	}
	data[x] = 0;
	
	while(line[x] == ' '){
		++x;
	}
	
	int y = 0;
	while(line[x] != ' '){
		ama[y] = line[x];
		++x;
		++y;
	}
	ama[y] = 0;
	
	x += 2;
	
	while(line[x] == ' '){
		++x;
	}
	
	y = 0;
	while(line[x] <= '0' || line[x] > '9'){
		nome[y] = line[x];
		++x;
		++y;
	}
	nome[y] = 0;
	
	y = 0;
	while(line[x] != ' '){
		cns[y] = line[x];
		++x;
		++y;
	}
	cns[y] = 0;
	
	//DATA	AMA	NOME	CNS	PROCEDIMENTO(1/0)	CONSULTA(1/0)
	
	char drop[3000];
	
	if(String(line).endsWith("PROCEDIMENTO\n")){
		sprintf(drop, "%s\t%s\t%s\t%s\t1\t0\t%s\n", data, ama, nome, cns, crr_usc);
	}
	else if(String(line).endsWith("CONSULTA\n")){
		sprintf(drop, "%s\t%s\t%s\t%s\t0\t1\t%s\n", data, ama, nome, cns, crr_usc);
	}
	
	return drop;
}

char *get_us_code(char *line){
	int x = 0;
	
	while(line[x] < '0' || line[x] > '9'){
		++x;
	}
	
	char us_code[3000];
	
	int y = 0;
	while(line[x] != ' '){
		us_code[y] = line[x];
		++y;
		++x;
	}
	us_code[y] = 0;
	
	return us_code;
}

int main(int argc, char** argv) {
	FILE *f = fopen("raw_data.txt", "r");
	fseek(f, 0, SEEK_END);
	unsigned long long int fsz = ftell(f);
	fseek(f, 0, SEEK_SET);
	
	
	FILE *f2 = fopen("drop.txt", "w");
	
	char buffer[5000];
	char us_code[3000];
	strcpy(us_code, "---");
	
	//Loop que percorre o arquivo raw_data.txt lendo uma linha por vez
	while(ftell(f) < fsz){
		fgets(buffer, 5000, f);
		
		if(String(buffer).startsWith("UNIDADE DE SA")){
			strcpy(us_code, get_us_code(buffer));
		}
		
		//Verifica se o terceiro caracter da ultima linha lida é '/'
		if(buffer[2] == '/'){
			
			//Caso seja é aplicada a função que formata o texto pra uso no 'excel'
			strcpy(buffer, formatLine(buffer, us_code));
			
			fprintf(f2, "%s", buffer, us_code);
		}
		
	}
	
	fclose(f2);
	fclose(f);
	return 0;
}
