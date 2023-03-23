polygons = []
with open('inpscr.txt', 'r') as f:
    for line in f:
        l = list(map(int, line.split()))
        polygons.append("("+str(l[0])+", "+str(l[1])+")")
print(polygons)
        