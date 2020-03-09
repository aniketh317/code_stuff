buf = input()
n = int(buf)
buf = input()
l = buf.split(" ")

data = [-2]
for i in range(0,n):
	data.append(int(l[i]))

class node:
	def __init__(self):
		self.adj = []
		self.num = 0 #number of children(with 0) as root
		self.v = False

def dfs_init(a):
	G[a].v = True
	G[a].num = data[a]
	for b in G[a].adj:
		if not G[b].v:
			G[a].num += dfs_init(b)
			
	return G[a].num
		
Max = 100000000	
G = [node()]

for i in range(1,n+1):
	G.append(node()) 
	G[i].num = data[i]
	
	
for _ in range(0,n-1):
	buf = input()
	l = buf.split(" ")
	u = int(l[0])
	v = int(l[1])
	G[u].adj.append(v)
	G[v].adj.append(u)

li = []
li.append([1,0])
G[1].v = True
ret = 0
i = 0
while len(li)!=0:
	G[li[-1][0]].num = G[li[-1][0]].num + ret
	i = 0
	for i in range(li[-1][1],len(G[li[-1][0]].adj)):
		if not G[G[li[-1][0]].adj[i]].v:
			li[-1][1] = i+1
			G[G[li[-1][0]].adj[i]].v = True
			li.append([G[li[-1][0]].adj[i],0])
			i = -1
			ret = 0
			break		
	if i!=-1:
		ret = G[li[-1][0]].num
		li.pop()

mi = Max
tot = G[1].num
for i in range(2,n+1):
	z = tot-2*G[i].num
	if z<0:
		z = -z
	if mi>z:
		mi = z

print(mi)	
