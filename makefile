FLAGS = -Wall -pedantic-errors

cl:
	gcc  $(FLAGS) -o head head.c
	gcc  $(FLAGS) -o tail tail.c
	gcc  $(FLAGS) -o wc wc.c
	gcc  $(FLAGS) -o true true.c
	gcc  $(FLAGS) -o false false.c
	gcc  $(FLAGS) -o env env.c

clean:
	rm -f head tail wc true false env
