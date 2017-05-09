# matplotlib import
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import matplotlib.path as path
from matplotlib import gridspec
from matplotlib import colors as mcolors

# custom import
from config import *

# other import
import os

# Colors of matplotlib
COLORS = mcolors.CSS4_COLORS
COLORS_NAMES = ['blue', 'gold', 'red', 'green', 'fuchsia', 'black',
                'dodgerblue', 'yellow', 'crimson', 'dimgray', 'red', 'darkkhaki',
                'darkmagenta', 'salmon', 'orange', 'coral', 'cyan', 'grey', 'olive',
                'darkviolet', 'seagreen', 'peru', 'brown', 'chocolate', 'midnightblue',
                'rebeccapurple']


def save_graphic_with_mesures_calls(mesures, output_filename, labels, title):
    assert(mesures is not None), "save_graphic_with_mesures: mesures is None"
    assert(len(mesures) == len(labels)), \
        "save_graphic_with_mesures: labels and mesures doesnt have the same length"

    plt.clf()
    plt.cla()
    plt.close()

    for i in range(len(mesures)):
        mesure = mesures[i]

        time_intervals = []
        calls_array = []

        for j in range(len(mesure.histogram)):
            time_interval = float(j - (len(mesure.histogram) / 2))
            calls = mesure.histogram[j][1]

            time_intervals.append(time_interval)
            calls_array.append(float(calls))

        if i > len(COLORS_NAMES) - 1:
            color_name = random.choice(list(COLORS.keys()))
        else:
            color_name = COLORS_NAMES[i]

        color = COLORS[color_name]

        plt.plot(time_intervals, calls_array, color=color, label=labels[i])

    plt.ylabel("Number of calls")
    plt.xlabel("Time intervals")

    plt.title(title)
    plt.grid(True)

    plt.legend(loc="upper left")

    plt.savefig(os.path.join(REP_ROOT, output_filename))


def save_graphic_with_mesures_percentage(mesures, output_filename, labels, title):
    assert(mesures is not None), "save_graphic_with_mesures: mesures is None"
    assert(len(mesures) == len(labels)), \
        "save_graphic_with_mesures: labels and mesures doesnt have the same length"

    plt.clf()
    plt.cla()
    plt.close()

    for i in range(len(mesures)):
        mesure = mesures[i]

        time_intervals = []
        calls_array = []
        percentages = []

        for j in range(len(mesure.histogram)):
            time_interval = float(j - (len(mesure.histogram) / 2))
            calls = mesure.histogram[j][1]

            time_intervals.append(time_interval)
            calls_array.append(float(calls))

        calls_total = sum(calls_array)
        for j in range(len(calls_array)):
            percentages.append((calls_array[j] / calls_total) * 100)

        if i > len(COLORS_NAMES) - 1:
            color_name = random.choice(list(COLORS.keys()))
        else:
            color_name = COLORS_NAMES[i]

        color = COLORS[color_name]

        plt.plot(time_intervals, percentages, color=color, label=labels[i])

    plt.ylabel("percentage")
    plt.xlabel("Time intervals")

    plt.title(title)
    plt.grid(True)

    plt.legend(loc="upper left")

    plt.savefig(os.path.join(REP_ROOT, output_filename))
