import  random, threading, subprocess
from tqdm import tqdm
from concurrent.futures import ThreadPoolExecutor

class test:
	def __init__(self, n, restrict, len):
		self.n = n
		self.total = 0
		self.min = restrict**2
		self.max = restrict / 2
		self.restrict = restrict 
		self.len = len
	def gen_num(self):
		res = random.sample(range(-1000, 1000), self.len)
		rand_num = " ".join(map(str, res))
		return (rand_num)
	def test(self):
		for i in tqdm(range(0, self.n)):
			s = self.gen_num()
			f = open("help_kit/arg", "w")
			f.write(s)
			f.close()
			out = subprocess.check_output("./push_swap {} | wc -l".format(s), shell=True, text=True)
			if int(out) >= self.restrict:
				print("{}. - {}:\n".format(i+1, int(out)))
				exit(1)
			if self.min > int(out):
				self.min = int(out)
			if self.max < int(out):
				self.max = int(out)
			self.total += int(out)
		print("avg: {} min: {} max: {}".format(self.total/self.n, self.min, self.max))

if __name__ == "__main__":
	print("<!> Specify variables for usage.")
	n = input("<?> How many times you want to test it -> ")
	limit = input("<?> The limit of operation -> ")
	len_stack = input("<?> The len of stack A -> ")
	if n == "" or limit == "" or len_stack == "":
		exit(1)
	test = test(int(n), int(limit), int(len_stack))
	test.test()
	print("Test passed !")
