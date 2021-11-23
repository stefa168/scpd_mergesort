#!/usr/bin/python3
import re
import sys
import argparse
import matplotlib.pyplot as plt


class MyData():
    def __init__(self, num_proc):
        self.num_proc = num_proc
        self.time = None

class MySpeedup():
    def __init__(self, speedup, num_proc):
        self.num_proc = num_proc
        self.speedup = speedup

def get_type(line, exec, pos_type, pos_proc):
    splitted = line.split(" ")
    current_type = splitted[pos_type]
    current_type = current_type.split("/")[-1]
    if current_type in exec:
        exec[current_type].append(MyData(splitted[pos_proc]))
    else:
        exec[current_type] = [MyData(splitted[pos_proc])]
    return current_type


def extract_data(filepath):
    with open(filepath, "r") as file:
        lines = file.readlines()

    type = re.compile(r'(Run )(\s)*', re.M)
    type2 = re.compile(r'(mpirun -f machinefile -np )(\s)*', re.M)
    type_time = re.compile(r'(Sorting... Done. Elapsed_time: )(\s)*', re.M)

    exec = {}
    current_type = None
    for line in lines:
        if(type.findall(line)):
            current_type = get_type(line, exec, 1, -2)

        if(type2.findall(line)):
            current_type = get_type(line, exec, 5, 4)

        if(type_time.findall(line)):
            splitted = line.split(" ")
            exec[current_type][-1].time = splitted[-2]
    return exec

def plot(to_plot, max_num_proc, fig_max_speedup):
    fig, ax = plt.subplots()
    ax.set_ylim(ymin=0)
    ax.set_xlim(xmin=0)
    y = range(0, fig_max_speedup)
    x = range(0, fig_max_speedup)
    ax.plot(x, y, label="Ideal speedup", linestyle='--')

    for key in to_plot.keys():
        y = list(map(lambda x: x.speedup, to_plot[key]))
        x = list(map(lambda x: x.num_proc, to_plot[key]))
        ax.plot(x, y, label=key)
    ax.set_yticks(range(0, fig_max_speedup))
    ax.set_xticks(range(0, max_num_proc + 2, 2))
    ax.set_title("Speedup factor")
    ax.grid(linewidth=.5, linestyle='--', axis="x", zorder=0)
    ax.grid(linewidth=.5, linestyle='--', axis="y", zorder=0)
    plt.legend(loc='upper right', fancybox=True, shadow=True, prop=dict(size=10))
    plt.ylabel("Speedup", fontsize=9)
    plt.xlabel("Number of processes/threads", fontsize=9)
    fig.tight_layout()
    plt.show()
#    fig.savefig("{}/{}.pdf".format(out_dir, title), format="pdf")

def main(args):
    print(args.file)
    data = extract_data(args.file)

    for key, value in data.items():
        for elem in value:
            print("{}:\n\ttime: {}, nproc: {}".format(key, elem.time, elem.num_proc))


    serial_time = None
    for type, value in data.items():
        if type == "serial":
            serial_time = float(value[0].time)
            break

    to_plot = {}
    max_num_proc = 2
    for type, value in data.items():
        if type != "serial":
            for elem in value:
                if type not in to_plot:
                    to_plot[type] = [MySpeedup(0, 0)]
                tmp_proc = int(elem.num_proc)
                if tmp_proc > max_num_proc:
                    max_num_proc = tmp_proc
                print("t_s: {}, t_p: {}, t_s/t_p: {}".format(serial_time, float(elem.time), serial_time / float(elem.time)))
                to_plot[type].append(MySpeedup(serial_time / float(elem.time), tmp_proc))

    for type, value in to_plot.items():
        to_plot[type] = sorted(value, key=lambda x: (x.num_proc, x.speedup))

    plot(to_plot, max_num_proc, 10)


if __name__ == "__main__":
    try:
        parser = argparse.ArgumentParser()
        parser.add_argument("-file", help="file path to plot", type=str, required=True)
        args = parser.parse_args()

        main(args)
    except KeyboardInterrupt:
        print()
