#!/bin/python3
import os
import math
import datetime
import argparse
import subprocess


def exec_command(fd, command):
    compressed = " ".join(command)
    print(compressed)
    output = subprocess.check_output(command, universal_newlines=True)
    fd.write("{}\n".format(output))


def main(args):
    info = "len: {}, seed: {}, max_proc: {}, machinefile: {}".format(args.len, args.ran, args.max, args.mf)
    command = {"command": "make",
        "type": "serial",
        "seed": "SEED={}".format(args.ran),
        "length": "LEN={}".format(args.len),
        "procs": "NCO={}".format(args.max),
        "grain": "GRAIN={}".format(2**30),
        "machinefile": "MF={}".format(args.mf)
    }
    parallel_types = ["pthread_divetimpera", "pthread_partitioning", "mpi_divetimpera", "mpi_partitioning"]

    print(info)
    print(command)
    logs_directory = "logs"
    if not os.path.exists(logs_directory):
        os.makedirs(logs_directory)

    with open("{}/exec_{}.log".format(logs_directory, datetime.datetime.now()), "w") as fd:
        fd.write("{}\n".format(info))
        exec_command(fd, ["make", "env"])
        exec_command(fd, ["make", "clean"])
        exec_command(fd, command.values())
        for type in parallel_types:
            command["type"] = type
            for num in range(2, args.max + 1, 2):
                command["procs"] = "NCO={}".format(num)
                exec_command(fd, command.values())

if __name__ == "__main__":
    try:
        parser = argparse.ArgumentParser()
        parser.add_argument("-len", help="length array", type=int, default=134217728)
        parser.add_argument("-ran", help="Random seed", type=int, default=123456789)
        parser.add_argument("-np", help="Max number of processor", type=int, default=4)
        parser.add_argument("-mf", help="Machinefile", type=str, default="machinefile")
        args = parser.parse_args()

        main(args)
    except KeyboardInterrupt:
        print()
