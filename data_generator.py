import random

num_count = int(input("Quanti numeri?"))
min_val = int(input("Valore minimo?"))
max_val = int(input("Valore massimo?"))

with open("File_to_order.txt", "w") as f:
    f.write(f"{num_count}\n")

    for i in range(1, num_count):
        val = round(random.randint(min_val, max_val), None)
        f.write(f"{val}\n")
