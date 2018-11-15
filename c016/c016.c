// Nikolaj Vorobiev

/* c016.c: **********************************************************}
{* Téma:  Tabulka s Rozptýlenými Položkami
**                      První implementace: Petr Přikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      Úpravy: Karel Masařík, říjen 2014
**                              Radek Hranický, 2014-2018
**								Nikolaj Vorobiev - zpracovani 2018
**
** Vytvořete abstraktní datový typ
** TRP (Tabulka s Rozptýlenými Položkami = Hash table)
** s explicitně řetězenými synonymy. Tabulka je implementována polem
** lineárních seznamů synonym.
**
** Implementujte následující procedury a funkce.
**
**  HTInit ....... inicializuje tabulku před prvním použitím
**  HTInsert ..... vložení prvku
**  HTSearch ..... zjištění přítomnosti prvku v tabulce
**  HTDelete ..... zrušení prvku
**  HTRead ....... přečtení hodnoty prvku
**  HTClearAll ... zrušení obsahu celé tabulky (inicializace tabulky
**                 poté, co již byla použita)
**
** Definici typů naleznete v souboru c016.h.
**
** Tabulka je reprezentována datovou strukturou typu tHTable,
** která se skládá z ukazatelů na položky, jež obsahují složky
** klíče 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na další synonymum 'ptrnext'. Při implementaci funkcí
** uvažujte maximální rozměr pole HTSIZE.
**
** U všech procedur využívejte rozptylovou funkci hashCode.  Povšimněte si
** způsobu předávání parametrů a zamyslete se nad tím, zda je možné parametry
** předávat jiným způsobem (hodnotou/odkazem) a v případě, že jsou obě
** možnosti funkčně přípustné, jaké jsou výhody či nevýhody toho či onoho
** způsobu.
**
** V příkladech jsou použity položky, kde klíčem je řetězec, ke kterému
** je přidán obsah - reálné číslo.
*/

#include "c016.h"

int HTSIZE = MAX_HTSIZE;
int solved;

/*          -------
** Rozptylovací funkce - jejím úkolem je zpracovat zadaný klíč a přidělit
** mu index v rozmezí 0..HTSize-1.  V ideálním případě by mělo dojít
** k rovnoměrnému rozptýlení těchto klíčů po celé tabulce.  V rámci
** pokusů se můžete zamyslet nad kvalitou této funkce.  (Funkce nebyla
** volena s ohledem na maximální kvalitu výsledku). }
*/

int hashCode ( tKey key ) {
	int retval = 1;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % HTSIZE );
}

/*
** Inicializace tabulky s explicitně zřetězenými synonymy.  Tato procedura
** se volá pouze před prvním použitím tabulky.
*/

void htInit ( tHTable* ptrht ) {
	for (int i = 0; i < HTSIZE; i++) {
		(*ptrht)[i] = NULL;
	}
}

/* TRP s explicitně zřetězenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíče key.  Pokud je
** daný prvek nalezen, vrací se ukazatel na daný prvek. Pokud prvek nalezen není, 
** vrací se hodnota NULL.
**
*/

tHTItem* htSearch ( tHTable* ptrht, tKey key ) {
	if(ptrht != NULL) { 								//kdyz tabulka neni prazdna
		int hash = hashCode(key);						//vememe hash
		tHTItem *item= (*ptrht)[hash]; 					//najdeme tu polozku v nase tabulce
		for (; item != NULL; item = item -> ptrnext) {  //prochazime synonymy
			if(!strcmp(item -> key , key)) {			//kdyz jsme nasli tak to mame
				return item;
			}
		}
	}
	return NULL; 										//kdyz ne tak mame smulu(
}

/* 
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vkládá do tabulky ptrht položku s klíčem key a s daty
** data.  Protože jde o vyhledávací tabulku, nemůže být prvek se stejným
** klíčem uložen v tabulce více než jedenkrát.  Pokud se vkládá prvek,
** jehož klíč se již v tabulce nachází, aktualizujte jeho datovou část.
**
** Využijte dříve vytvořenou funkci htSearch.  Při vkládání nového
** prvku do seznamu synonym použijte co nejefektivnější způsob,
** tedy proveďte.vložení prvku na začátek seznamu.
**/
#include <stdio.h>
void htInsert ( tHTable* ptrht, tKey key, tData data ) {
	tHTItem *item = htSearch(ptrht, key); 		//skusime vyhledat
	if(item == NULL) { 							//neobsahuje
		int hash = hashCode(key);				//najdeme hash
		if( (item = malloc(sizeof(tHTItem)))== NULL ) {
			exit(-1);
		}
		item -> data = data;					//vlozime data
		item -> key = key;						//vlozime klic
		item -> ptrnext = (*ptrht)[hash];		//kdyz na tom hashi jsou itemy next->item[1]
		(*ptrht)[hash] = item;					//a ten nas item[0] vlozime na hash
	}
	else if (item != NULL ) {   				//obsahuje
		item -> data = data; 					//je alokovane tak jen zmenime data
	}
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato funkce zjišťuje hodnotu datové části položky zadané klíčem.
** Pokud je položka nalezena, vrací funkce ukazatel na položku
** Pokud položka nalezena nebyla, vrací se funkční hodnota NULL
**
** Využijte dříve vytvořenou funkci HTSearch.
*/

tData* htRead ( tHTable* ptrht, tKey key ) {
	if(htSearch(ptrht, key))
		return &((htSearch(ptrht, key))->data); 	//htSearch nam vraci bud' polozku, nebo
	else											//NULL
		return NULL;
}
/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vyjme položku s klíčem key z tabulky
** ptrht.  Uvolněnou položku korektně zrušte.  Pokud položka s uvedeným
** klíčem neexistuje, dělejte, jako kdyby se nic nestalo (tj. nedělejte
** nic).
**
** V tomto případě NEVYUŽÍVEJTE dříve vytvořenou funkci HTSearch.
*/

void htDelete ( tHTable* ptrht, tKey key ) {
	int hash = hashCode(key);				//najdeme hash
	tHTItem *uk = (*ptrht)[hash];			//vytvorime uk
	if(!(*ptrht)[hash])						//kdyz je v tabulce s timto hash item/y
		return;
	if(!strcmp(uk->key, key)) {				//nasli sme klic na 1 miste
		(*ptrht)[hash] = uk->ptrnext;		//2 item bude 1
		free(uk);							//uvolmine ten 1(uk)
	}
	else {
		for(;uk->ptrnext != NULL ; uk = uk->ptrnext) {
			if(!strcmp(uk->ptrnext->key, key)) {		//nasli sme na 2 nebo na konci	
				tHTItem *mazany = uk->ptrnext;
				uk->ptrnext = mazany->ptrnext; 			//bud NULL nebo dalsi item
				free(mazany);
				return;
			}
		}
	}
}

/* TRP s explicitně zřetězenými synonymy.
** Tato procedura zruší všechny položky tabulky, korektně uvolní prostor,
** který tyto položky zabíraly, a uvede tabulku do počátečního stavu.
*/

void htClearAll ( tHTable* ptrht ) {
	tHTItem *uk = NULL; 		//v budoucnu bude uk na hash
	tHTItem *mazany = NULL;		//v budoucny bude uk na item[i] ktery smazeme
	if(ptrht == NULL) {
		return;
	}
	else {
		for(int i = 0; i < HTSIZE; i++) {
			uk = (*ptrht)[i];
			if(uk) { 		//kdyz hash neni prazdny
				for(;uk != NULL; ) { 		//   uk 		##toto je postup mazani
					mazany = uk;			//	 |
					uk = uk->ptrnext;		//	item[0] item[1] .. item[n] 
					free(mazany);			//
				}							//	mazany   uk
			}								//   |		 |
		}									//  item[0] item[1] .. item[n]
	}
	htInit(ptrht); 	//init
}
