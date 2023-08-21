#!/usr/bin/env python3

"""
esercizio sulle named pipe
  legge gli interi dal file di testo argv[1] e li
  scrive nella pipe somma_down 
  al termine, aspetta dalla pipe somma_up le somme dei primi 
  provenienti dai processi ausiliari
"""	
import sys, os, struct

"""
funzioni utili
  os.mkfifo(path)						# crea una named fifo
  os.unlink(path)						# cancella un file
  os.open(path,flags)       # analoga alla open dal C
  os.read(fd,n)             # leggedal file descriptor fd
	struct.pack(format,val)   # trasforma int -> sequenza byte
  struct.unpack(format,bs)  # trasforma sequenza byte -> int
"""


# Variabili globali con i nomi delle pipe da usare
Pipeup = "somma_up"
Pipedown = "somma_down"


def main(nomefile):
  """
    apre e legge gli interi da nomefile 
    e li scrive nella pipe somma_down 
    sotto forma di interi a 32 bit
    al termine della scrittura chiude somma_down 
    e apre somma_up in lettura 
    e legge da lì interi a 64 bit 
    che rappresentano le somme parziali 
    comunicate dai processi sommaprimi.c. 
    Terminata la lettura sommaprimi.py si calcola la somma totale 
    e si visualizza su stdout.
  """
  
  # apriamo pipe verso i programmi C
  fd = os.open(Pipedown,os.O_WRONLY)
  # apriamo il file di testo contenente gli interi
  tot = 0
  with open(nomefile,"r") as f:
    for linea in f:
			# converte linea in intero 
      intero = int(linea)
			# converto intero in sequenza di byte scrive nella pipe
      bs = struct.pack("<i",intero)
      os.write(fd,bs)
      tot += 1
  print(f"{tot} interi letti da file e scritti nella pipe")
  
  # chiudiamo il file di testo
  f.close()
  # chiudo la pipe in sccrittura
  os.close(fd)

  # apro la pipe in lettura
  fd = os.open(Pipeup,os.O_RDONLY)
  # leggo interi a 64 bit
  tot = 0
  somma = 0
  while True:
    # legge fino a 4 byte mettendoli in un bytarray
    bs = os.read(fd,4)
    if len(bs)==0:    # non c'e' nessuno che scrive: termina
      break
    tot +=1
    # converte i 4 byte letti in un intero e lo stampa
    valore = struct.unpack("<i",bs)[0] 
    print(f"=={os.getpid()}== {valore}")
    somma += valore
  
  print(f"=={os.getpid()}== Letti {tot} interi")
  print(f"=={os.getpid()}== Somma totale: {somma}")

  # chiudo la pipe in lettura
  os.close(fd)

	

	
# lancio di main()
if len(sys.argv)!=2:
  print("Uso:\n\t %s nome_file_interi" % sys.argv[0])
else:
  main(sys.argv[1])
 
