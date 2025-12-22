import  random, threading, subprocess
from tqdm import tqdm
from concurrent.futures import ThreadPoolExecutor

n = 1000

class test:
	def __init__(self, n):
		self.n = n
	def gen_num(self):
		res = random.sample(range(-10, 10), 20)
		rand_num = " ".join(map(str, res))
		return (rand_num)
	def test(self):
		for i in tqdm(range(0, self.n)):
			s = self.gen_num()
			out = subprocess.check_output("./a.out {} | wc -l".format(s), shell=True, text=True)
			if int(out) >= 5500:
				print("{}. - {}:\n".format(i+1, int(out)))
				exit(1)


num = test(0)
print(num.gen_num())
# first = test(int(n/3))
# second = test(int(n/3))
# third = test(int(n/3))
# with ThreadPoolExecutor(max_workers=3) as executer:
# 	executer.submit(first.test())
# 	executer.submit(second.test())
# 	executer.submit(third.test())
