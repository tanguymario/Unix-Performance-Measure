#!/usr/bin/env python
# -*- coding: utf-8 -*-

# custom import
from mesure_utils import *
from config import *
from graphic_helper import *

# other import
from sys import argv
import os

NOTHING_MESSAGE = """
 -- graphics.py --

 This script can create graphics from tests.

 To create a graphic, we need :
  - one or more source files (tests)
  - one output file

 An example :
 python graphics.py test1.txt graphic2

 """


def get_mesure(path):
    full_path = os.path.join(REP_ROOT, path)
    try:
        test_file = open(full_path, 'r')
    except:
        print("get_mesures: File does not exists at " + full_path)
        pass

    mesure = Mesure(test_file.read())
    test_file.close()

    return mesure


def get_mesures(paths):
    mesures = []

    for path in paths:
        mesure = get_mesure(path)
        if mesure is None:
            print("Mesure could not be found")
        else:
            mesures.append(mesure)

    return mesures


def main():
    if len(argv) > 2:
        output_files = argv[1:-1]

        graphic_output_file = argv[len(argv) - 1]

        mesures = get_mesures(output_files)
        labels = []

        assert(len(output_files) == len(mesures)), "Mesures are corrupted"

        print("\n------------------\n")

        for i in range(len(output_files)):
            output_file = output_files[i]
            mesure = mesures[i]

            print(output_file)
            labels.append(input("Label : "))
            print("\n")

        title = input("Title : ")

        choices = ['calls', 'percentage']

        choice = ''
        while choice not in choices:
            print('\n')
            print(choices)
            choice = input("Choice : ").lower()

        if choice == choices[0]:
            save_graphic_with_mesures_calls(mesures, graphic_output_file, labels, title)
        else:
            save_graphic_with_mesures_percentage(mesures, graphic_output_file, labels, title)
    else:
        print(NOTHING_MESSAGE)

if __name__ == "__main__":
    main()
