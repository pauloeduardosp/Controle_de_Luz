#!/usr/bin/env python3
import os

#script vai ser executado pelo script /root/.node-red/script_git.py
#sera executado apenas quando houver alteração nos arquivos do node-red
#os.system("git add .")
#os.system("git commit -a -m \"commit de script_git\"")
#os.system("git push -u origin master")


pwd = os.popen('pwd').read()
print("\n send pwd\n")
print(pwd)

git_add = os.popen("git add .").read()
print("\n send git add\n")
print(git_add)

git_commit = os.popen('git commit -a -m \"commit de script_git\"').read()
print("\n send git commit\n")
print(git_commit)


git_push = os.popen('git push -u origin master').read()
print("\n send git push\n")
print(git_push)


###