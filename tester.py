#!/usr/bin/env python
# -*- coding: utf-8 -*-

# custom import
from py_utils import *
from config   import *

# other import
from subprocess import call
from sys import argv
from os import listdir

NOTHING_MESSAGE = """
 -- tester.py --

 This script can execute tests.

 To execute a single command please enter :
 python tester.py "[COMMAND]"

 To execute (a) test file(s) please enter :
 python tester.py [PATH] [PATH] ...

 Or you can combine the two ones.
 python tester.py [PATH] "[COMMAND]"

 """

def do_test(command):
	assert (command is not None), "do_test: command is None"

	print('\n' + command)

	try:
		call(command.split(" "))
		return command
	except:
		print("Command \"" + command + "\" is invalid")

	return None

def get_mesure_from_command(command):
	path_output = get_output_filename(command)
	return get_mesure_from_file(path_output)

def get_mesure_from_file(path_output_file):
	assert (path_output_file is not None), \
	"get_mesure_from_file: path_output_file is None"

	try:
		output_file = open(path_output_file, 'r')
		mesure = Mesure(output_file.read())
		output_file.close()

		return mesure
	except:
		print("get_mesure_from_file: No file at " + str(path))

	return None

def get_output_filename(command):
	assert (command is not None), "get_output_filename: command is None"

	command_args = command.split(" ")
	if OUTPUT_OPTION in command_args:
		index = command_args.index(OUTPUT_OPTION)
		if index < len(command_args) - 1:
			return command_args[index + 1]

	return None

def do_test_file(path):
	assert (path is not None), "do_test_file: path is None"

	try:
		test_file = open(path, "r")
	except:
		print("do_test_file: No file at " + str(path))
		return None

	commands = test_file.readlines()
	test_file.close()

	for command in commands:
		if command == '\n':
			pass
		elif command[-1:] == '\n':
			command = command[:-1]

		do_test(command)

def do_test_files(paths):
	assert (paths is not None), "do_test_files: paths is None"

	for path in paths:
		do_test_file(path)

def main():
	if not path.isfile(os.path.join(REP_ROOT, EXEC)):
		call(["make", "all"])

	if len(argv) > 1:
		for i in range(1, len(argv)):
			if path.isfile(argv[i]):
				do_test_file(argv[i])
			else:
				command = str(argv[i])
				do_test(command)
	else:
		print(NOTHING_MESSAGE)

if __name__ == "__main__":
	main()
