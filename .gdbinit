add-auto-load-safe-path /home/obirukov/Desktop/myproject/codam/.gdbinit
set confirm off
define calc
	b 207
	r $(cat arg)
	b sort
	n
	display test(s)
	display sum
	display *s->rotations@2
	display ft_atoi((char *) cur->content, s)
	end
