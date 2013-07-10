all:
	gcc src/main.c -c -o obj/main.o -lbluetooth -lcwiid
	gcc src/playsound.c -c -o obj/playsound.o -lpthread
	gcc src/manual_mode.c -c -o obj/manual_mode.o -lbluetooth -lcwiid -lm
	gcc src/micro_maestro.c -c -o obj/micro_maestro.o	
	gcc obj/main.o obj/playsound.o  obj/manual_mode.o obj/micro_maestro.o -o ball_plate -lbluetooth -lcwiid -lpthread -lm
	
main:
	gcc src/main.c -c -o obj/main.o -lbluetooth -lcwiid
	gcc obj/main.o obj/playsound.o  obj/manual_mode.o obj/micro_maestro.o -o ball_plate -lbluetooth -lcwiid -lpthread -lm
	
playsound:
	gcc src/playsound.c -c -o obj/playsound.o -lpthread
	gcc obj/main.o obj/playsound.o  obj/manual_mode.o obj/micro_maestro.o -o ball_plate -lbluetooth -lcwiid -lpthread -lm
	
manual_mode:
	gcc src/manual_mode.c -c -o obj/manual_mode.o -lbluetooth -lcwiid -lm
	gcc obj/main.o obj/playsound.o  obj/manual_mode.o obj/micro_maestro.o -o ball_plate -lbluetooth -lcwiid -lpthread -lm

micro_maestro:
	gcc src/micro_maestro.c -c -o obj/micro_maestro.o	
	gcc obj/main.o obj/playsound.o  obj/manual_mode.o obj/micro_maestro.o -o ball_plate -lbluetooth -lcwiid -lpthread -lm
	
clean:
	rm obj/*.o
