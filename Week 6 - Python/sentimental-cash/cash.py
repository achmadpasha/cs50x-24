from cs50 import get_float

while True:
    dollars = get_float("Change: ")
    # Dollars must be greater than or equal to zero
    if dollars >= 0:
        break

total = 0
coins = [25, 10, 5, 1]
# Rounding dollars to cents
cents = int((dollars * 100) + 0.5)

for coin in coins:
    while cents >= coin:
        total += 1
        cents -= coin

print(total)
