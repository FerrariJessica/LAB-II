#!/usr/bin/env python3
"""
Esempio di script per la navigazione del filesystem


Comandi per la gestione di file e directory

os.getcwd()       # restituisce directory corrente
os.chdir(path)    # cambia directory
os.listdir(path)  # elenca file (restituisce lista di stringhe)
os.access(path)   # verifica i permessi  
os.path.getsize(path) # dimensione file
os.path.exists(path)  # vero se il file/directory esiste  
os.path.isfile(path)  # vero se regular file
os.path.isdir(path)   # vero se directory
os.path.islink(path)  # vero se symbolic link
os.path.join(nome_dir,nome_file) # combina nome dir e file
os.path.abspath(path) # restituisce path assoluto
os.path.realpath(path) # restituisce nome canonico eliminando link simbolici

Lista completa dei comandi su:
  https://docs.python.org/3/library/os.html
  https://docs.python.org/3/library/os.path.html

Per informazioni sui permessi dei file o come 
cambiarli: `man chmod` 
"""
import os, os.path, sys


def main(nomedir):
	"""Lancia ricerca ricorsiva su nomedir"""
	if not os.path.exists(nomedir):
		print("Il nome che mi hai passato non esiste")
		exit(1)
	if not os.path.isdir(nomedir):
		print("Il nome che mi hai passato esiste, ma non è una directory")
		exit(1)
	nomeabs = os.path.abspath(nomedir)
	dim, nomeg = cerca_grande(nomeabs,set())
	print(f"Il file più grande è {nomeg} e ha {dim} bytes")
	return
		
# funzione ricorsiva per cercare il file più grande	
def cerca_grande(nome,giaesplorati):
	"""resituisce la coppia (dim,nome) che identifica il file
 più grande tra quelli nella directory nome e sottodirectory """
	
	assert os.path.isdir(nome), "Argomento deve essere una directory"
	print(f"Begin: {nome}",file=sys.stderr)
	# inizializzo i due parametri di output
	dimgrande = -1
	nomegrande = None
	# ottiene il contenuto della directory 
	listafile = os.listdir(nome)
	for f in listafile:
		nomecompleto = os.path.join(nome,f)
		# verifica se il file è accessibile
		if not os.access(nomecompleto,os.F_OK):
			print("!! Broken link:", nomecompleto, file=sys.stderr)
			continue
		# distinguo tra file normali e directory
		if not os.path.isdir(nomecompleto):
			nuovadim = os.path.getsize(nomecompleto)
			nuovonome = nomecompleto
		else:
			# nomecompleto è una directory possibile chiamata ricorsiva
			nomereal = os.path.realpath(nomecompleto)
			if nomereal in giaesplorati:
				print(f"!! Directory {nomereal} già esplorata",file=sys.stderr)
				continue
			else: giaesplorati.add(nomereal)
			if not os.access(nomecompleto, os.R_OK | os.X_OK):
				print(f"!! Directory {nomecompleto} non accessibile",file=sys.stderr)
				continue
			# directory nuova e accessibile: esegui ricorsione
			nuovadim, nuovonome = cerca_grande(nomecompleto,giaesplorati)
		# aggiorno dim se ho trovato file più grande
		if nuovadim > dimgrande:
			dimgrande = nuovadim
			nomegrande = nuovonome
	# ciclo for su i file di questa directory terminato			
	print(f"End: {nome}",file=sys.stderr)
	return (dimgrande, nomegrande)
	
	
# verifico argomenti sulla linea di comando
# e faccio parire la ricerca
if len(sys.argv) == 2:
    main(sys.argv[1])
else:
    print("Uso:", sys.argv[0], "nome_directory")


