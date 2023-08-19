#!/usr/bin/env python3

import os, os.path, sys 


def main(src,dest):
	if not os.path.exists(src):
		print(f"{src} non esiste")
		exit(1)
	if not os.path.isdir(src):
		print(f"{src} non è una cartella")
		exit(1)
	os.makedirs(dest)
	
	
	
	
# invoca main 
if len(sys.argv) == 3:
		main(sys.argv[1], sys.argv[2])
else:
		print("Uso:", sys.argv[0], "src_dir dest_dir")
		exit(1)