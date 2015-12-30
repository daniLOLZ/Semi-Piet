// Cripta e decritta file colorati :D

#include "stdafx.h"
#include "CImg.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

#ifndef __linux										//
#include <io.h>										// Vede se esiste un file...
#define access    _access_s							//
#else												//
#include <unistd.h>									//
#endif												// Abbastanza semplice no?
													//
bool FileExists(const std::string &Filename)		//
{													//
	return access(Filename.c_str(), 0) == 0;		//
}													//

#define controllo 193
using namespace cimg_library;

struct chosen_file {
	std::string percorso;
	HANDLE handle;
};
chosen_file scegli_file_txt() {  //Crea la finestra di scelta file
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[260];       // buffer for file name
	HWND hwnd = NULL;       // owner window
	HANDLE hf;              // file handle

							// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "*.txt";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 

	chosen_file file1;		// Mi restituisce sia il percorso del file, sia l'handle nel caso mi servisse

	if (GetOpenFileName(&ofn) == TRUE) {
		hf = CreateFile(ofn.lpstrFile,
			GENERIC_READ,
			0,
			(LPSECURITY_ATTRIBUTES)NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			(HANDLE)NULL);

		file1.handle = hf;
		file1.percorso = szFile;   // Copio il percorso salvato in szFile nel membro 'percorso' di 'file1'
	}
	return file1;
}
int trova(char * mat, char c) {  //Trova l'indice del carattere di cin.get()

	int i = 0;
	bool trovato = false;
	if (c == '\r') c = '\n';

	do {
		if (mat[i] == c) {
			trovato = true;
		}
		if (!trovato) ++i;  //così nel caso che sia il primo carattere del dizionario non mi incrementa di uno
	} while (!trovato);

	if (trovato) {
		return i;
	}
	else return -1;
}
std::streamoff fileSizei(std::ifstream &input) {

	std::streampos inizio, fine;
	inizio = input.tellg();
	input.seekg(0, std::ios::end);
	fine = input.tellg();

	return fine - inizio;
}

void Cripta(char* dizionario, std::ifstream &in2) { //Bisogna usare una chiamata 'per riferimento' (ifstream &in2) 
	
	std::cin.rdbuf(in2.rdbuf());

	int lato = sqrt(fileSizei(in2) / 3) + 1;  /* Es. 1499 byte / 3 (tre caratteri per pixel) = 499.6666 = 499; sqrt(499) = 22.338 = 22; 22 + 1 (per sicurezza) = 23; */

	int X = lato;
	int Y = lato;

	CImg <unsigned char> testC(X, Y, 1, 3, 0);

	unsigned char blue[] = { 0, 0, 255 };

	CImgDisplay main(testC, "Semi-Piet");

	in2.close();									//chiudo il file di input
/*	std::ofstream on("input.txt", std::ios::app);   //lo faccio diventare file di output per scriverci un carattere di controllo finale (app = scrivi alla fine del file)
	std::cout.rdbuf(on.rdbuf());					// Mannaia la puttana dovevo cambiare lo stream
	std::cout << char(controllo);					//scrivo il carattere di controllo ( A accentata )
	on.close();										//chiudo il file di output
	std::ofstream on2("input.txt", std::ios::app | std::ios::beg);	 //
	std::cout.rdbuf(on2.rdbuf());									 // Mannaia la puttana dovevo cambiare lo stream x 2
	std::cout << "\n";												 // Ci metto anche un ritorno a capo all'inizio perchè mi si sfotte il primo carattere nella decrittazione
	on2.close();													 // a-richiudo il file di output
*/	
	std::ifstream in("Semi-Piet.txt");					 //lo faccio diventare di nuovo di input
	std::cin.rdbuf(in.rdbuf());						 // Mannaia la puttana dovevo cambiare lo stream x 3

	char lol = in.get();
	int i = 0,
		l = 0,
		asdf = 0,
		channel_cycle = 0;

		lol = in.get();

	while (!in.eof() && lol != char(-1)) {

		asdf = trova(&dizionario[0], lol); // Trovo l'indice del carattere nell'array 'caratteri'

		if (asdf == -1) { // Se trovo un carattere non valido scrivo "Errore" e salto alle ultime istruzioni
			break;
		}

		testC(i, l, channel_cycle) = asdf;


		if (channel_cycle == 2) { // aumento 'i' solo dopo 3 passaggi di channel_cycle
			++i %= X; // incremento 'i' di 1, e poi trovo il modulo di base X (ciclo 0,1,2...,X-2,X-1)
			if (i == 0) {
				++l; /* se siamo alla situazione di x == 29 && channel_cycle == 2, dopo il primo if di questo ciclo sarà x == 0 && channel_cycle == 2
					 (per ora non mi interessa, si sistema subito dopo a 0) e quindi il secondo if aumenterà di 1 'l'
					 non sarà un problema nemmeno all'inizio perchè la primissima situazione dove il primo if agirà sara in x == 0 &&
					 channel_cycle == 2, che prima si trasformerà in 1;2, e quindi non influenza il secondo if
					 */
			}
		}
		++channel_cycle %= 3; // incremento channel_cycle di 1, e poi trovo il modulo di base 3 (ciclo 0,1,2)

		main.display(testC);

			lol = in.get();
	}

	if (asdf == -1) {
		testC.draw_text(X / 2, Y / 2, "Errore", blue);
	}

//	while (!main.is_closed()) {
//
//	}
	testC.save_bmp("Semi-Piet.bmp");

	
}

void Decritta( char* dizionario, std::ifstream &in2) {

	CImg <unsigned char> testD("Semi-Piet.bmp");
	char ahboh;

	long int f_size = 0, contatore = 0;
	f_size = fileSizei(in2);		//Potrei usare il carattere eof per vedere se la fine del file è stata raggiunta 
	bool minchia = true;

	in2.close();
	
	std::ofstream out("Semi-Piet output.txt");
	std::cout.rdbuf(out.rdbuf());

	for (int i = 0; i < testD.height() && minchia; ++i ) {
		for (int l = 0; l < testD.width() && minchia; ++l) {
			for (int channel = 0; channel < 3 && minchia; ++channel) {
				ahboh = dizionario[testD(l, i, channel)];
				contatore++;
				if (contatore < f_size) {
					std::cout << ahboh;
					// Scrive il carattere in "dizionario" con l'indice pari al valore dei 3 canali di colore di quel pixel
				}
				else {
					minchia = false; 
				} // quando finisco i caratteri buoni faccio in modo che non scriva più niente
			}
		}
	}
}

int main()
{
	std::ifstream in2("Semi-Piet.txt", std::ios::binary);
	std::streambuf *consIn, *consOut;
	consOut = std::cout.rdbuf(); 
	consIn = std::cin.rdbuf();

	char scelta;
	char caratteri[] = " 0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\n,.-!\?/\\\'\"àèìòù()";
	do {  // Scelgo se criptare o decrittare da console

		std::cout << "\n Vuoi criptare o decrittare un file? \n C / D (Crittare / Decrittare)\n ";
		std::cin >> scelta;

		if ( toupper(scelta) != 'C' && toupper(scelta) != 'D') { // così che valgano anche gli input 'c' e 'd' (minuscoli)
			std::cout << " Ripetere la scelta \n";
		}
	} while (toupper(scelta) != 'C' && toupper(scelta) != 'D');  

	if (toupper(scelta) == 'C') {

		std::cout << " Creando il file bitmap... \n";

		if(FileExists("Semi-piet.bmp"))	remove("Semi-Piet.bmp");

		Cripta( &caratteri[0] , in2);

		std::cin.rdbuf(consIn);		//resetta lo stream di input 
		std::cout.rdbuf(consOut);	//resetta lo stream di output

		std::cout << "\n File bitmap creato con successo \n\n";
	}


	else if (toupper(scelta) == 'D') {
		std::cout << " Decrittando l'immagine... ";
		
		Decritta( &caratteri[0] , in2);

		std::cout.rdbuf(consOut);	//resetta lo stream di output
		std::cin.rdbuf(consIn);		//resetta lo stream di input

		std::cout << "\n Immagine decrittata con successo \n\n";
	}
	
//	system("PAUSE");
    return 0;
}
