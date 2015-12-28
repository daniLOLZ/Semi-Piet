// Semi-Piet.cpp : definisce il punto di ingresso dell'applicazione console.
//

#include "stdafx.h"
#include "CImg.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>


using namespace cimg_library;
std::ifstream in("input.txt", std::ios::binary);
std::ofstream out("output.txt");


int trova(char * mat, char c) {  //Trova l'indice del carattere di cin.get()

	int i = 0;
	bool trovato = false;

	while (!trovato) {
		if (mat[i] == c) {
			trovato = true;
		}
		++i;
	}
	if (trovato) {
		return i-1;
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

int main()
{

	std::streambuf *consIn, *consOut;
	consOut = std::cout.rdbuf(); 
	consIn = std::cin.rdbuf();

	char scelta;

	do {  // Scelgo se criptare o decrittare da console

		std::cout << "Vuoi criptare o decrittare un file? \n C / D (Crittare / Decrittare)\n";
		std::cin >> scelta;
		if ( toupper(scelta) != 'C' || toupper(scelta) != 'D') { // così che valgano anche gli input 'c' e 'd'
			std::cout << " Ripetere la scelta \n";
		}
	} while (scelta != 'C' || scelta != 'D');  


	std::cin.rdbuf(in.rdbuf());
	std::cout.rdbuf(out.rdbuf());

	int lato = sqrt(fileSizei(in) / 3) + 1;  /* Es. 1499 byte / 3 (tre caratteri per pixel) = 499.6666 = 499; sqrt(499) = 22.338 = 22; 22 + 1 (per sicurezza) = 23; */

	int X = lato;
	int Y = lato;

	CImg <unsigned char> test1(X, Y, 1, 3, 0);

	unsigned char blue[] = { 0, 0, 255 };
	char caratteri[] = "\n0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ,.-!\?/\\\'\"àèìòù()"; 

	CImgDisplay main(test1, "Semi-Piet");

	std::cout << fileSizei(in) << std::endl;

	in.close();										//chiudo il file di input
	std::ofstream on("input.txt", std::ios::app);   //lo faccio diventare file di output per scriverci un carattere di controllo finale
	std::cout << char(193);							//scrivo il carattere di controllo ( A accentata)
	on.close();										//chiudo il file di output
	std::ifstream in("input.txt");					//lo faccio diventare di nuovo di input
	
	char lol = in.get();	
	int i = 0, 
		l = 0,
		asdf = 0,
		channel_cycle = 0;

	while (lol == '\r') {
		lol = in.get();
	}
//	std::cout << char(lol + 180);

	while (lol != char(193) && lol != char(-1)) {	
		
		asdf = trova(&caratteri[0], lol); // Trovo l'indice del carattere nell'array 'caratteri'

		if (asdf == -1) { // Se trovo un carattere non valido scrivo "Errore" e salto alle ultime istruzioni
			break;		
		}
		
		test1(i, l, channel_cycle) = asdf;    


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

		main.display(test1);

		do {  

			lol = in.get();

		} while (lol == '\r'); /* Fino a che abbiamo il carattere '\r' lo lascio perdere e prendo un altro carattere */
		out << lol;
	}

	if (asdf == -1) {
		test1.draw_text(X / 2, Y / 2, "Errore", blue);
	}


	test1.display();

	while (!main.is_closed()) {

   }
	test1.save_png("test1");
	
    return 0;
}

