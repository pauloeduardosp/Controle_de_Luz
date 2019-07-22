#!/usr/bin/env python3

import os

git_status = os.popen('git status').read()
lines = git_status.splitlines()

commit = False
changes_for_commit = False

arquivo_copy = []


for line in lines:        

	word = line.split()
	if len(word) > 0:
		if  word[0][:10] == "lib/flows/":        		#verifica se houve alteração nos arquivo de biblioteca de flow
			os.system("git add " + word[0])
			arquivo_copy.append(word[0])
			print("\t\t ### novo add lib/flow")
			commit = True


	if line == "Changes not staged for commit:":		#verifica se houve alteração no flow
		changes_for_commit = True	
		commit = True
		print("\t\t ### existe aquivo modificado a ser comitado")
	
	if changes_for_commit is True:						#executa o commit caso haja alterações
		if len(word) > 1:
			if word[0]=="modified:":
				arquivo_copy.append(word[1])
				print("########## CHEGOU AQUI ##########")	


if commit is True:										#executa o commit caso haja alterações
	os.system("git commit -a -m \"commit de script_git\"")
	print("executa commit")
	print("\n\n ######## arquivo_copy ############ \n")
	print(arquivo_copy)
	print("\n\n")


	for line in arquivo_copy:
		os.system("cp " + line + " /home/github/Server" + line)

	os.system("git -C \"/home/github\" add .")
	os.system("git -C \"/home/github\" commit -a -m \"commit remoto de script_git\"")
	os.system("git -C \"/home/github\" push -u origin master")
	
#teste envio arquiov 2
#git -C "/home/github" commit -a -m "commit remote"
#	word = line.split()
#	if len(word) > 1:
#		if word[0]=="modified:":
#			os.system("git add " + word[1])
