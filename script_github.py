#!/usr/bin/env python3
import os

#script vai ser executado pelo script /root/.node-red/script_git.py
#sera executado apenas quando houver alteração nos arquivos do node-red
os.system("git add .")
os.system("git commit -a -m \"commit de script_git\"")
os.system("git push -u origin master")


