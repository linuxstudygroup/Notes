compare : main.o bubble_sort.o merge_sort.o
	gcc -o compare main.o bubble_sort.o merge_sort.o

main.o : main.c my_type.h bubble_sort.h merge_sort.h
	gcc -c main.c

bubble_sort.o : bubble_sort.c bubble_sort.h my_type.h
	gcc -c bubble_sort.c

merge_sort.o : merge_sort.c merge_sort.h my_type.h
	gcc -c merge_sort.c

clean :
	rm compare main.o bubble_sort.o merge_sort.o
