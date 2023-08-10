#!/usr/bin/env python3
"""
Esempio di script per la ricerca di file duplicati
I file che risultano copie di uno già esistente vengono rinominati
con estensione .dup


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
import os, os.path, sys, subprocess


def main(nomedir):
	"""Lancia ricerca ricorsiva su nomedir"""
	if not os.path.exists(nomedir):
		print("Il nome che mi hai passato non esiste")
		exit(1)
	if not os.path.isdir(nomedir):
		print("Il nome che mi hai passato esiste, ma non è una directory")
		exit(1)
	nomeabs = os.path.abspath(nomedir)
	diz = {}
	cerca_duplicati(nomeabs,diz,set())
	for dim in sorted(diz):
		if len(diz[dim])>1:
			print(f"Dimensione {dim}:")
			for f in diz[dim]:
				print("  ",f)
			# cerca duplivati nell'insieme diz[dim]
			controlla_uguali(diz[dim])
	return



def controlla_uguali(lista):
	"""Dato un insieme di file verifica quali di loro sono dei duplicati e li 
 rinomina con estensione .dup"""
	
	assert len(lista)>1, "Devo essere chiamato con lista con + di 1 elemento"
	# indice dei file che sono risultati dei duplicati
	duplicati = set()
	for i in range(len(lista)):
		if i in duplicati:   # non considerare un file che ho già visto
			continue					 # essere un duplicato
		for j in range(i+1,len(lista)):
			# esegue il confronto fra lista[i] e lista[j] con in comando cmp
			comando = ["cmp", lista[i], lista[j]]
			r = esegui(comando)
			if r.returncode == 0:
				# cmp ristituisce 0 -> i file sono uguali 
				print("    DUPLICATO:", lista[j])
				# rinomina lista[j]
				os.rename(lista[j],lista[j]+".dup")
				# marca come duplicato e non considerare in futuro
				# necessario dato che il nome è cambiato!
				duplicati.add(j)
			


def cerca_duplicati(nome,diz,giaesplorati):
	"""Associa in diz ad ogni lunghezza di file i nomi di tutti i file
 di quella lunghezza tra quelli nella directory nome e sottodirectory """
	
	assert os.path.isdir(nome), "Argomento deve essere una directory"
	print(f"Begin: {nome}",file=sys.stderr)
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
			if nuovadim in diz: # esiste chiave nuovadim in diz?
				diz[nuovadim].append(nomecompleto)
			else:
				diz[nuovadim] = [nomecompleto]
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
			cerca_duplicati(nomecompleto,diz,giaesplorati)
	# ciclo for su i file di questa directory terminato			
	print(f"End: {nome}",file=sys.stderr)
	return


#  esegue un comando della shell in un sottoprocesso 
def esegui(comando):
  """Esegui comando e restituisci il corrispondente CompletedProcess"""
  return subprocess.run(comando, stdout=subprocess.PIPE, 
                    stderr=subprocess.PIPE,encoding="utf-8")

	
# verifico argomenti sulla linea di comando
# e faccio parire la ricerca dei duplicati
if len(sys.argv) == 2:
    main(sys.argv[1])
else:
    print("Uso:", sys.argv[0], "nome_directory")


