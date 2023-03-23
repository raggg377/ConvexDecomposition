with open("for_input.txt") as file:
    lines = [line.rstrip() for line in file]

b =lines[1].strip(" ")
print(b[0],b[1])