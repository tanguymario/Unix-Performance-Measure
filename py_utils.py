import os
import signal
import sys

def quit():
	print ("\n\nbye...\n")
	sys.exit(0)

def signal_handler(signal, frame):
	quit()

def clear():
	os.system('cls' if os.name == 'nt' else 'clear')

def get_filename(path):
	pathSlashSplitted = path.split('/')
	filename = pathSlashSplitted[len(pathSlashSplitted) - 1]
	filename = remove_extension(filename)
	return filename

def remove_extension(filename):
	i = len(filename) - 1
	while i >= 0:
		if filename[i] == '.':
			return filename[:i]

		i -= 1

	return filename

signal.signal(signal.SIGINT, signal_handler)
