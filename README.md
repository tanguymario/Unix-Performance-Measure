# Unix-Performance-Measure

C program that can measure perfomance of Unix functions.

-----------------------------------

 Université Claude Bernard Lyon 1

 Programmation Système et Temps Réel

 TP Mesure

 Auteur : Tanguy Cesaratto p1305380

-----------------------------------

 REMARQUES :

  Code python écrit avec python3.6, fonctionne avec python2.7
  (Ne fonctionne pas avec Windows-cygwin et manjaro ARM)

  Dans le fichier sys_info.c, la fonction :

  "size_t get_memory_size(void)"

  a été récupérée sur le web afin de pouvoir afficher la RAM physique de la
  machine. Cette fonction a été publié "Licence Creative Commons" :

  "http://nadeausoftware.com/articles/2012/09/c_c_tip_how_get_physical_memory_size_system"

-----------------------------------

 UTILISATION :

 Pour compiler le projet écrit en C :

 - Executer la commande 'make' dans le repértoire du projet

 ( Pour les autres taches 'make' possibles, voir le makefile )

 - Si la compilation a fait un executable (pas d'erreur),
  lancer la commande './mesure'

 - Sans arguments l'executable affiche une liste des différentes fonctionnalités

 - Les options concatenees telles "-apd" ne fonctionnent pas (faire "-a -p -d")

 Pour effectuer les tests avec python :

 - Executer la commande 'python tester.py' ou './tester.py'

 - Les instructions sont affichées quand il n'y a pas d'arguments

 - Certains dépendances python sont requises (numpy, matplolib)

 - Si le projet C n'est pas compilé, le script python compilera le projet

 - Si aucun argument n'est passé à 'setup.py', alors ce dernier effectuera
  tous les tests présents dans le dossier 'tests'

 Pour créer un graphique à partir des sorties des tests avec python :

 - Executer la command 'python graphics.py' ou './graphics.py'

 - Les instructions sont affichées quand il n'y a pas d'arguments

-----------------------------------

 ORGANISATION

 Les fichiers sources C et python sont dans le repértoire courant.

 Le dossier 'tests' contient des fichiers au format '.txt' qui sont des
 commandes à passer à l'éxecutable 'mesure'

 Le dossier 'outputs' est utilisé pour récupérer les résultats de l'executable
 mesure

 Le dossier 'graphics' est utilisé pour contenir les graphiques résultant du
 script python 'graphics.py'

 Les mesures faites sur les plateformes :
 - Windows avec cygwin
 - Raspberry Pi (Manjaro ARM)
 n'ont pas pu soit - installer les dépendences avec python-pip
                   - utiliser ces dernières

 Les tests étant faits indépendamment des graphiques,
 cela ne posait pas de problème

-----------------------------------
