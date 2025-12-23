import  random, threading, subprocess
from tqdm import tqdm
from concurrent.futures import ThreadPoolExecutor

global total
n = 10000
restrict = 1000

class test:
	def __init__(self, n, restrict):
		self.n = n
		self.total = 0
		self.min = restrict**2
		self.max = restrict / 2
		self.restrict = restrict 
	def gen_num(self):
		res = random.sample(range(-1000, 1000), 4)
		rand_num = " ".join(map(str, res))
		return (rand_num)
	def test(self):
		for i in tqdm(range(0, self.n)):
			s = self.gen_num()
			f = open("arg", "w")
			f.write(s)
			f.close()
			out = subprocess.check_output("./a.out {} | wc -l".format(s), shell=True, text=True)
			if int(out) >= self.restrict:
				print("{}. - {}:\n".format(i+1, int(out)))
				exit(1)
			if self.min > int(out):
				self.min = int(out)
			if self.max < int(out):
				self.max = int(out)
			self.total += int(out)
		print("avg: {} min: {} max: {}".format(self.total/self.n, self.min, self.max))

		



# num = test(n, restrict)
# print(num.gen_num())
first = test(int(n), restrict)
first.test()
# with ThreadPoolExecutor(max_workers=3) as executer:
# 	executer.submit(first.test())
# 	executer.submit(second.test())
# 	executer.submit(third.test())
