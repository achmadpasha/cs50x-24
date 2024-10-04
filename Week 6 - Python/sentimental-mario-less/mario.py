import cs50

height = 0
while height < 1 or height > 8:
    height = cs50.get_int("Height: ")

x = 1
for i in range(height):
    for _ in range(height - x):
        print(" ", end="")
    for j in range(x):
        print("#", end="")
    x += 1
    print("")
