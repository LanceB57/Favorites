import math

def mod(num, exponent, modulo):
    number = 1
    for i in range(exponent):
        number *= num
        number = number % modulo
    return number


def mod_inv(num, modulo):
    for i in range(1, modulo):
        if (num * i) % modulo == 1:
            return i


keys = open("nums.txt", 'r')

p = int(keys.readline().strip())
q = int(keys.readline().strip())
N = p * q

totientN = (p - 1) * (q - 1)

e = 1

for i in range(2, totientN):
    if math.gcd(totientN, i) == 1 and math.gcd(N, i) == 1:
        e = i
        break

d = mod_inv(e, totientN)
print(d)
char_to_num = open("char_to_num.txt", 'r')
adict = {}
bdict = {}
adict[" "] = 34
bdict[34] = " "


for line in range(33):
    curr = char_to_num.readline().split()
    num = int(curr[0])
    char = curr[1]
    adict[char] = num
    bdict[num] = char

message = open("message.txt", 'r')
output = open("output.txt", 'w')

choice = input("Would you like to (E)ncrypt or (D)ecrypt? ")

if choice == "E":
    for line in message:
        curr = line.upper().strip('\n')
        curr = list(curr)
        encrypted = ""
        for char in curr:
            num = adict[char]
            num = mod(num, e, N)
            encrypted += str(num) + " "
        output.write(encrypted + '\n')
else:
    for line in message:
        curr = list(map(int, line.split()))
        decrypted = ""
        for num in curr:
            num = mod(num, d, N)
            decrypted += bdict[num]
        output.write(decrypted + '\n')