#! /usr/bin/env python3
# server che fornisce l'elenco dei primi in un dato intervallo 
# implementato usando la select solo per i socket in lettura
# in questo modo può gestire più di un client ma quando viene
# gestita una richiesta tutto il resto rimane in attesa
import sys, struct, socket, select, time

# valori di default per host e port
HOST = "127.0.0.1"  # interfaccia su cui mettersi in ascolto
PORT = 65432  # Port to listen on (non-privileged ports are > 1023)
 
 
def main(host=HOST,port=PORT):
  # creiamo il server socket
  with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server:
    try:  
      server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)      
      server.bind((host, port))
      server.listen()
      print("Entro in modalità select...")
      # lista dei socket da monitorare in input
      inputsk = [server]  
      # questo while non termina perchè il server rimane attivo
      while len(inputsk)>0:
        # attendo max 10 secondi [TIMEOUT] se ci sono nuovi dati
        # Nota: _ indica una variabile non mi interessa 
        inready, _ , _ = select.select(inputsk,[],[],10)
        # inready è una sottolista di inputsk contenente
        # i socket da cui sono arrivati dei dati
        if len(inready) == 0:
          # non ci sono stati arrivi di dati negli ultimi 10 secondi
          print("Mi annoio")
        else:
          # ci sono dati dal leggere
          for c in inready:
            if c is server:
              # il server socket ha ricevuto una richiesta di connessione
              conn, addr = server.accept()
              print(f"Contattato da {addr}")
              # non gestisco la connessione ma metto conn
              # tra i socket da monitorare
              inputsk.append(conn)
            else:
              # il socket di un client ha ricevuto dei dati
              inputsk.remove(c)        # non mi aspetto altri dati da questo socket
              gestisci_connessione(c)
      print("Errore: inputsk è vuoto") # il server viene mai cancellato dalla lista 
    except KeyboardInterrupt:
      print('Va bene smetto...')
    # shutdown del server (la close viene fatta dalla with)  
    server.shutdown(socket.SHUT_RDWR)



# gestisci una singola connessione
# con un client
def gestisci_connessione(conn): 
  # In questo esempio potrei usare direttamente conn
  # senza il blocco with, aggiungendo la close in fondo alla funzione
  # L'uso di with serve solo a garantire che
  # conn venga chiusa all'uscita del blocco
  # In generale il blocco with esegue automaticamente 
  # le necessarie inzializzazioni e il clean-up finale
  # Per approfondire: https://realpython.com/python-with-statement/
  with conn:  
    # ---- attendo due interi da 32 bit
    data = recv_all(conn,8)
    assert len(data)==8
    inizio  = struct.unpack("!i",data[:4])[0]
    fine = struct.unpack("!i",data[4:])[0]
    print(f"Ho ricevuto i valori", inizio,fine)
    # ---- calcolo elenco dei primi
    primi = elenco_primi(inizio, fine)
    # ---- invio risultato in formato preceduto da lunghezza
    conn.sendall(struct.pack("!i",len(primi))) #lunghezza
    for p in primi:
      conn.sendall(struct.pack("!i",p))      # invia singoli valori 
    print(f"Finito con {conn.getpeername()}")
 

# Riceve esattamente n byte dal socket conn e li restituisce
# il tipo restituto è "bytes": una sequenza immutabile di valori 0-255
# Questa funzione è analoga alla readn che abbiamo visto nel C
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