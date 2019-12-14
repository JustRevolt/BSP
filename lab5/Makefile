CC=gcc

thr=-pthread

PT1=part1/
PT2=part2/
PT3=part3/

all: part1 part2 part3 
# ==========================================
part1: task1 task2 task3  

# ---- task 1 -----------------------------
task1: task1s task1c  
task1s: 
	$(CC) -o task1s $(PT1)task1_server.c
task1c: 
	$(CC) -o task1c $(PT1)task1_client.c

# --- task 2 ------------------------------
task2: task2s task2c 
task2s: 
	$(CC) -o task2s $(PT1)task2_server.c 
task2c: 
	$(CC) -o task2c $(PT1)task2_client.c

# --- task 3 ------------------------------ 
task3: task3s task3c 
task3s: 
	$(CC) -o task3s $(PT1)task3_server.c
task3c:
	$(CC) -o task3c $(PT1)task3_client.c

# ==========================================
part2: task4 task5 task6 task7 

# --- task 4 ------------------------------
task4: task4.o
	$(CC) $(thr) -o $@ task4.o
	rm task4.o
task4.o:
	$(CC) -c $(thr) -o $@ $(PT2)task4.c
 
# --- task 5 ------------------------------
task5: task5.o
	$(CC) $(thr) -o $@ $^ 
	rm task5.o

task5.o:
	$(CC) -c $(thr) -o $@ $(PT2)task5.c 

# --- task 6 -----------------------------
task6: task6.o
	$(CC) $(thr) -o $@ $^ 
	rm task6.o
task6.o:
	$(CC) -c $(thr) -o $@ $(PT2)task6.c

# --- task 7 -----------------------------
task7: task7.o
	$(CC) $(thr) -o $@ $^ 	
	rm $^
task7.o:
	$(CC) -c $(thr) -o $@ $(PT2)task7.c

#=========================================
part3: task8 task9 task10 

# --- task 8 -----------------------------
task8: task8s task8c

task8s: 
	$(CC) -o $@ $(PT3)task8_server.c 

task8c:
	$(CC) -o $@ $(PT3)task8_client.c

# --- task 9 -----------------------------
task9: 
	$(CC) -o $@ $(PT3)task9.c

# --- task 10 ---------------------------
task10: 
	$(CC) -o $@ $(PT3)task10.c
clean: 
	rm task* 
