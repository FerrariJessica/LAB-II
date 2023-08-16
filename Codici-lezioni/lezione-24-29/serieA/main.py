#! /usr/bin/env python3
"""
Esercizio che costruisce una classifica dati i 
risultati di un insieme di partite scritte in un file

Mantengo la classifica come dizionario che associa
  squadra -> punti
se la squadra non compare nel dizionario 
assumiamo abbia 0 punti

sorting howto:
https://docs.python.org/3/howto/sorting.html

with statement
https://realpython.com/python-with-statement/#managing-resources-in-python
"""

"""
Uso un dizionario per rappresentare la classifica
chiave  squadra, valore punteggio
convenzione se una squadra non e nel dizionario ha punteggio 0
"""

import sys 


def aggiungi_partita(c,s1,s2,g1,g2):
	"""Aggiorna la classifica c sulla base del risultato
 di una sigola partita"""
	# asegno il punteggio
	p1 = p2 = 1
	if g1>g2:
		p1 = 3
		p2 = 0
	elif g1 < g2:
		p1 = 0
		p2 = 3
	# assegno i punti
	if s1 in c:
		c[s1] = c[s1] + p1
	else:
		c[s1] = p1
	if s2 in c:
		c[s2] = c[s2] + p2
	else:
		c[s2] = p2


def aggiungi_partite_da_file(nomefile):
	"""Crea una classifica con i punti ricavati dai risultati
 risportati dentro il file nomefile"""
	cla = {}   # crea classifica vuota

	with open(nomefile,"r") as f:
		for linea in f:
			# verifico il formato
			a = linea.split()
			if len(a)==0:
				continue 
			if len(a)!=4:  
				raise RuntimeError(f"Linea non valida: {linea}")
			# estraggo il numero di gol come interi
			g1 = int(a[0])
			g2 = int(a[1])
			aggiungi_partita(cla,a[2],a[3],g1,g2)
	# restituisce la classifica ottenuta
	return cla

	
		
def main(nomefile):
		"""Prende in input il nome di un file e stampa
	la classifica leggibile ottenuta dai risultati"""
		c = aggiungi_partite_da_file(nomefile)
		squadre = list(c.keys())
		print(f"primo :{squadre}\n")
		squadre.sort()
		print(f"secondo :{squadre}\n")
		# def ordine(x):
		# 	return c[x]
		# squadre.sort(key=ordine, reverse=True)
		squadre.sort(key=lambda squadra: c[squadra], reverse=True)
		print(f"terzo :{squadre}\n")
		ris = "--- classifica ---"
		punteggio_precedente = -1
		for s in squadre:
			if(c[s]!=punteggio_precedente):
				ris += "\n"
				ris += f"{c[s]} {s} "
				punteggio_precedente = c[s]
			else:
				ris += f"{s} " 
		print(ris)
		print("---- fine classifica ------")


if len(sys.argv)==2:
	main(sys.argv[1])
else:
	print(f"Uso:\n\t{sys.argv[0]} nomefile")