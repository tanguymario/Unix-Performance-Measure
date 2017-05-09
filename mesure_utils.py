class Mesure:
    def __str__(self):
        string = (
         self.ram + '\n'
         + self.sysname + '\n'
         + self.nodename + '\n'
         + self.release + '\n'
         + self.machine + '\n'
         + self.pid + '\n'
         + self.action + '\n'
         + self.nice + '\n'
         + self.time + '\n'
         + self.cores + '\n'
         + self.delay + '\n'
         + self.nbExec + '\n'
         + self.output + '\n')

        string += "\n - HISTOGRAM \n"
        for i in range(len(self.histogram)):
            for j in range(len(self.histogram[i])):
                string += (str(i) + " " + str(j) + " :  "
                    + str(self.histogram[i][j]) + '\n')

        return string

    def __init__(self, content):
        lines = content.split('\n')

        # Get information
        self.ram      = lines[4]
        self.sysname  = lines[5]
        self.nodename = lines[6]
        self.release  = lines[7]
        self.machine  = lines[8]

        self.pid    = lines[10]
        self.action = lines[11]
        self.nice   = lines[12]
        self.time   = lines[13]
        self.cores  = lines[14]
        self.delay  = lines[15]
        self.nbExec = lines[16]
        self.output = lines[17]

        # Histogram is a double dimension array
        self.histogram = []

        startHistogramIndex = 25

        for i in range(int((len(lines) - startHistogramIndex - 2) / 3)):
            lineIndex = startHistogramIndex + i * 3
            line = lines[lineIndex]
            if len(line) < 2:
                pass

            nextLine = lines[lineIndex + 1]

            histoItem = [line, nextLine]

            self.histogram.append(histoItem)
