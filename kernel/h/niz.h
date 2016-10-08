#ifndef _NIZ_H_
#define _NIZ_H_

template <class E>
class Niz
{
public:
	struct List
	{
		int id;
		List *next;
		List(int x) : id(x), next(0){}
	};

	int size;
	void expand();		//za prosirivanje niza, ako se javi potreba
	E **niz;			//pokazivac na niz pokazivaca na element E
	List *FreeID; 		//lista slobodnih id-a

	Niz();
	~Niz();

	int add(E *el);		//uzima prvi id iz liste slobodnoh i tu smesta pok. na novi el
	void remove(int id);	//brise el sa zadatim id-em iz niza i azurira listu
	E* get(int &id);
};

///////////////////////////////////////////////////////////////////////////////////


template<class E>
E* Niz<E>::get(int &id)
{
	return niz[id];
}


template <class E>
Niz<E>::Niz()
{
	niz = new E*[size = 10];
	List *tek, *pred;
	for (int i = 0; i < 10; i++)
	{
		niz[i] = 0;
		tek = new List(i);
		if (i == 0) pred = FreeID = tek;
		else{ pred->next = tek; pred = tek; }
	}
}

template <class E>
Niz<E>::~Niz()
{
	List *stari;
	while (FreeID != 0){ stari = FreeID; FreeID = FreeID->next; delete stari; }
	for (int i = 0; i < size; delete niz[i++]);
	delete niz;
}


template <class E>
void Niz<E>::expand()
{
	E **temp = niz;
	niz = new E*[size + 5];
	for (int j = 0; j<size; j++) niz[j] = temp[j];
	delete[] temp;

	List *tek, *pred;
	for (int i = size; i< size + 5; i++)	//prosirivanje niza
	{										//doslo je do prosirivanja => FreeID je prazno
		niz[i] = 0;
		tek = new List(i);
		if (i % size == 0) pred = FreeID = tek;
		else{ pred->next = tek; pred = tek; }
	}
	size += 5;
}


template <class E>
int Niz<E>::add(E *el)
{
	if (FreeID == 0) expand();
	int tmp = FreeID->id;
	niz[tmp] = el;
	FreeID = FreeID->next;
	return tmp;
}

template <class E>
void Niz<E>::remove(int id)
{
	if (niz[id] != 0)
	{
//		delete niz[id]; // OVDE JE UPADAO U REKURZIJU PA JE IZBACENO
		niz[id] = 0;
		List *novi = new List(id);
		novi->next = FreeID;	//nema razloga da lista bude uredjena
		FreeID = novi;	//pa nema potrebe trositi vreme na to => dodavanje na pocetak liste
	}
}



#endif
