#!/bin/python3
import os
import math
import time
import random
import datetime
import argparse
import subprocess


def exec_command(fd, command):
    compressed = " ".join(command)
    print(compressed)
    output = subprocess.check_output(command, universal_newlines=True)
    fd.write("{}\n".format(output))
    fd.flush()


def main(args):
    info = "type: {}, len: {}, seed: {}, max_proc: {}, machinefile: {}".format(args.type, args.length, args.rand, args.np, args.mf)
    command = {"command": "make",
        "type": args.type,
        "seed": "SEED={}".format(args.rand),
        "length": "LEN={}".format(args.length),
        "procs": "NCO={}".format(args.np),
        "grain": "GRAIN={}".format(args.grn),
        "machinefile": "MF={}".format(args.mf)
    }

    print(info)
    print(command)
    logs_directory = "logs"
    if not os.path.exists(logs_directory):
        os.makedirs(logs_directory)

    with open("{}/exec_{}_{}.log".format(logs_directory, args.type, time.time()), "w") as fd:
        fd.write("{}\n".format(info))
        exec_command(fd, ["make", "env"])
        exec_command(fd, ["make", "clean"])
        for num in range(2, args.np + 1, 2):
            command["procs"] = "NCO={}".format(num)
            exec_command(fd, command.values())

if __name__ == "__main__":
    try:
        parser = argparse.ArgumentParser()
        parser.add_argument("-type", help="Type", type=str, choices=["serial", "pthread_divetimpera", "pthread_partitioning", "mpi_divetimpera", "mpi_partitioning"], required=True)
        parser.add_argument("-length", help="Length array", type=int, default=134217728)
        random.seed(time.time())
        parser.add_argument("-rand", help="Random seed", type=int, default=random.randint(1, 10**7))
        parser.add_argument("-np", help="Max number of processor", type=int, default=4)
        parser.add_argument("-mf", help="Machinefile", type=str, default="machinefile")
        parser.add_argument("-grn", help="Grain", type=int, default=2**30)
        args = parser.parse_args()

        main(args)
    except KeyboardInterrupt:
        print()
