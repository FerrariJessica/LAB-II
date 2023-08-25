#! /usr/bin/env python3
# echo-server.py

# esempio di un server che si mette in attesa 
# su di un socket e quando inizia una connessione
# si limita a rispedire ogni messaggio che ha ricevuto

import socket

# Definisco le variabili globali 
# specifica da dove accettare le connessioni, l'host e la porta
HOST = "127.0.0.1"  # Standard loopback interface address (localhost)
PORT = 65432  # Port to listen on (non-privileged ports are > 1023)

# creazione del server socket
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s: # creiamo il server socket, sia s il socket fino a quando non viene uscito 
    s.bind((HOST, PORT)) # assegno l'host e la porta per il collegamento
    s.listen()   #attende connessioni
    while True:
      print(f"In attesa di un client su porta {PORT}...")
      # mi metto in attesa di una connessione
      conn, addr = s.accept() # accetto una connessione, conn è un oggetto connessione che uso per le comunicazioni, mentre addr contiene l'indirizzo del client
      # lavoro con la connessione appena ricevuta 
      with conn:  # quando esco da qui si chiude la connessione e ritorno ad essere in attesa di una nuova connessione
        print(f"Contattato da {addr}")
        while True:
            data = conn.recv(64) # leggo fino a 64 bytes
            print(f"Ricevuti {len(data)} bytes") 
            if not data:           # se ricevo 0 bytes 
                break              # la connessione è terminata
            conn.sendall(data)     # altrimenti echo
            #la funzione send differisce dalla sendall 
            #send restituisce il numero di byte inviati
            #sendall invia tutti i byte
            #se arriva 0 allora la connessione è terminata
        print("Connessione terminata")
