all: fig1 fig2 fig3 fig3_poor1 fig3_poor2 pc_v1 fig6_broken fig8_broken2 fig10_finally fig1112_final fig13_covering

fig1: fig1.c
	gcc -pthread fig1.c -o fig1

fig2: fig2.c
	gcc -pthread fig2.c -o fig2

fig3: fig3.c
	gcc -pthread fig3.c -o fig3

fig3_poor1: fig3_poor1.c
	gcc -pthread fig3_poor1.c -o fig3_poor1

fig3_poor2: fig3_poor2.c
	gcc -pthread fig3_poor2.c -o fig3_poor2

pc_v1: pc_v1.c
	gcc -pthread pc_v1.c -o pc_v1

fig6_broken: fig6_broken.c
	gcc -pthread fig6_broken.c -o fig6_broken

fig8_broken2: fig8_broken2.c
	gcc -pthread fig8_broken2.c -o fig8_broken2

fig10_finally: fig10_finally.c
	gcc -pthread fig10_finally.c -o fig10_finally

fig1112_final: fig1112_final.c
	gcc -pthread fig1112_final.c -o fig1112_final

fig13_covering: fig13_covering.c
	gcc -pthread fig13_covering.c -o fig13_covering

clean:
	rm -f *.o *~ fig1 fig2 fig3 fig3_poor1 fig3_poor2 pc_v1 fig6_broken fig8_broken2 fig10_finally fig1112_final fig13_covering
