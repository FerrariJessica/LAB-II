#! /usr/bin/env python3
# server che fornisce l'elenco dei primi in un dato intervallo 
# implementato usando la select per i socket in lettura e scrittura
import sys, struct, socket, select, time


# valori di default per host e port
HOST = "127.0.0.1"  # interfaccia su cui mettersi in ascolto
PORT = 65432  # Port to listen on (non-privileged ports are > 1023)
# SE SI USA LA AMCCHINA DI LABORATORIO OCCHIO A SPECIFICARE L'HOST CORRETTO 

def main(host=HOST,port=PORT):
  # creiamo il server socket
  with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server:
    try:  
      server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1) 
      server.bind((host, port))
      server.listen()
      print("Entro in modalità select...")
      # lista dei socket da monitorare in input e output
			# all'inizio non avendo iniziato devo solo monitorare s0, il socket
			# da dove possono arrivare richieste di connessione per nuovi client
      inputsk = [server]; outputsk = []
      # dizionario che associa ad ogni client i dati da spedire 
      inuscita = {}  
      while len(inputsk)>0:
        # attendo max 10 secondi se ci sono nuovi dati
        # Nota: _ indica una variabile che non mi interessa 
        inready, outready , _ = select.select(inputsk,outputsk,[],10)
        # la select mi dice se c'è qualcosa da leggere o scrivere
				# inready e outready sono sottoinsiemi delle rispettive socket 
				# dove ci sono dati da leggere (in) o sono pronti per la scrittura (out) 
        if len(inready) + len(outready) == 0:
          print("Mi annoio")  # non ci sono socket pronti da leggere o scrivere
        else:
          for c in inready:
            if c is server:
              # il server socket ha ricevuto una richiesta di connessione
              conn, addr = server.accept()
              print(f"Contattato da {addr}")
              # non gestisco la connessione ma metto conn
              # tra i socket da monitorare !!!!!!
              inputsk.append(conn)
            else:
              # se arriva un socket di un client già collegato vanno
              # leggi i dati che ci ha inviato e calcola (ma non spedire) il risultato
              primi = gestisci_lettura(c) # legge i due interi inviati dal client e calcola
              # l'elenco dei primi tra i due interi, non sono necessarie altre lettura da c
              inputsk.remove(c)
              # invia solo la lunghezza del risultato 
              c.sendall(struct.pack("!i",len(primi)))
              if len(primi)>0:
                # ci sono altri dati da inviare 
                outputsk.append(c) # metto c nella lista dei socket da monitorare in output
                inuscita[c] = primi # salva i dati da spedire alla chiave c nel dizionario
              else:
                # nient'altro da fare
                print(f"Finito con {c.getpeername()}")
                c.close()
					# vado a gestire tutti i socket che si stanno aspettando da me dei dati
          for c in outready:
            assert not (c is server), "Non è previsto scrivere sul server"
						# se il socket non ha dati in uscita
            assert c in inuscita, f"no dati in uscita per {c.getpeername()}"
            print(f"Invio dati a {c.getpeername()}")
            primi = inuscita[c]   # lista primi ancora da spedire
            assert(len(primi)>0), "mancano i dati da scrivere"
            # se tutto va bene invia il primo valore della lista
            # time.sleep(1) emula una rete lenta 
            c.sendall(struct.pack("!i",primi[0]))
            if(len(primi)>1):
              inuscita[c] = primi[1:]  # altri dati andranno spediti, aggiorno il diz 
            else: # se era 'ultimo numero della lista
              del inuscita[c]      # elimina la chiave c dal dizionario
              outputsk.remove(c)   # non devo più scrivere su c
              print(f"Spediti tutti i primi per {c.getpeername()}")
              c.close()    
      print("Errore: inputsk è vuoto") # il server viene mai cancellato dalla lista 
    except KeyboardInterrupt:
      print('Va bene smetto...')
    # shutdown del server (la close viene fatta dalla with)  
    server.shutdown(socket.SHUT_RDWR)


# legge intervallo e calcola risultato
# e lo restituisce senza spedire nulla
def gestisci_lettura(conn): 
    # ---- attendo due interi da 32 bit
    data = recv_all(conn,8)
    assert len(data)==8
    inizio  = struct.unpack("!i",data[:4])[0]
    fine = struct.unpack("!i",data[4:])[0]
    print(f"Ho ricevuto i valori", inizio,fine)
    # ---- calcolo elenco dei primi
    primi = elenco_primi(inizio, fine)
    return primi


# riceve esattamente n byte e li restituisce in un array di byte
# il tipo restituto è "bytes": una sequenza immutabile di valori 0-255
# analoga alla readn che abbiamo visto nel C
def recv_all(conn,n):
  chunks = b''
  bytes_recd = 0
  while bytes_recd < n:
    chunk = conn.recv(min(n - bytes_recd, 1024))
    if len(chunk) == 0:
      raise RuntimeError("socket connection broken")
    chunks += chunk
    bytes_recd = bytes_recd + len(chunk)
  return chunks
 


# restituisce lista dei primi in [a,b]
def elenco_primi(a,b):
  ris = []
  for i in range(a,b+1):
    if primo(i):
      ris.append(i);
  return ris


# dato un intero n>0 restituisce True se n e' primo
# False altrimenti
def primo(n):
    assert n>0, "L'input deve essere positivo"
    if n==1:
        return False
    if n==2:
        return True
    if n%2 == 0:
        return False
    assert n>=3 and n%2==1, "C'e' qualcosa che non funziona"
    for i in range(3,n//2,2):
        if n%i==0:
            return False
        if i*i > n:
            break    
    return True


# invoca il main con i parametri passati sulla linea di comando 
if len(sys.argv)==1:
  main()
elif len(sys.argv)==2:
  main(sys.argv[1])
elif len(sys.argv)==3:
  main(sys.argv[1], int(sys.argv[2]))
else:
  print("Uso:\n\t %s [host] [port]" % sys.argv[0])