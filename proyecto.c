#include <stdio.h>
#include <gtk/gtk.h>

/*
Por:
Julio Antonio Castro Fallas B81832
Mariana Chung Garita B92250
Raymond Da Acosta A B62281

El objetivo de este codigo es replicar el juego de gato.
Por lo tanto, el programa presenta una cuadricula de 3x3
en la que las personas que juegan se turnan para escoger
donde colocar una X o O, dependiendo del jugador.
El programa guarda el valor escogido y cuando hay 3 en linea,
de la manera que sea, muestra una ventana que dice el jugador
que gano.
Previo a esto, se diseno un menu para que los jugadores puedan
decidir si jugar o salir. Adicionalmente, dentro del juego
se puede decidir si volve al menu, reiniciar o finalizar.
*/

// Se deben crear las variables globales.
GtkBuilder *builder;

// Se crea la primera interfaz que se le muestra al usuario
GtkWidget *ventana_menu; // La ventana que contiene el menu
GtkWidget *boton_jugar; // El boton que inicia el juego
GtkWidget *boton_salir; // El boton que hace que se salga del juego

// Se crea la ventana donde se juega propiamente
GtkWidget *ventana_juego; // Ventana de juego
// Se crea una etiqueta que dice a quien le toca el turno
GtkWidget *etiqueta_turno;
// Se crean los botones de la cuadricula
// a: arriba
// b: abajo
// m: medio
// i: izquierda
// d: derecha
GtkWidget *boton_ai;
GtkWidget *boton_mi;
GtkWidget *boton_bi;
GtkWidget *boton_am;
GtkWidget *boton_mm;
GtkWidget *boton_bm;
GtkWidget *boton_ad;
GtkWidget *boton_md;
GtkWidget *boton_bd;
GtkWidget *boton_menu; // Boton para volver al menu
GtkWidget *boton_reiniciar; // Boton para reiniciar

// Se crea la ventana pop-up, que aparece temporalmente
// cuando un jugador gana
GtkWidget *ventana_popup; // Ventana del mensaje pop-up
GtkWidget *etiqueta_popup; // Etiqueta que contiene el texto

// Se debe crear una funcion que ayude a determinar el ganador del juego
// Se inicializa una matriz de caracteres que almacene las movidas de los jugadores
static char *matriz[3][3];

// Ve a cual jugador le toca jugar.
static int jugador = 0;

// Indica cuando hay ganador
static int hay_ganador = 0;

static int ganador; // Se  crea una variable que indique cual jugador gano

static int es_empate = 0; // Se  crea una variable que indique cuando se empato

static int contador = 0; // Se  crea una variable que lleve el contador de turnos

// Se inicializa la funcion que determina el ganador
int analiza_ganador(){
	contador++; // Se aumenta el contador al iniciar
	// Primero se analiza el caso en que el jugador X puede ganar
	if(jugador == 0 && matriz[0][0] != NULL && matriz[0][0] == matriz[0][1] && matriz[0][0] == matriz[0][2]
	|| jugador == 0 && matriz[1][0] != NULL && matriz[1][0] == matriz[1][1] && matriz[1][0] == matriz[1][2]
	|| jugador == 0 && matriz[2][0] != NULL && matriz[2][0] == matriz[2][1] && matriz[2][0] == matriz[2][2]

	|| jugador == 0 && matriz[0][0] != NULL && matriz[0][0] == matriz[1][0] && matriz[0][0] == matriz[2][0]
	|| jugador == 0 && matriz[0][1] != NULL && matriz[0][1] == matriz[1][1] && matriz[0][1] == matriz[2][1]
	|| jugador == 0 && matriz[0][2] != NULL && matriz[0][2] == matriz[1][2] && matriz[0][2] == matriz[2][2]

	|| jugador == 0 && matriz[0][0] != NULL && matriz[0][0] == matriz[1][1] && matriz[0][0] == matriz[2][2]
	|| jugador == 0 && matriz[0][2] != NULL && matriz[0][2] == matriz[1][1] && matriz[0][2] == matriz[2][0]){
		ganador = 1; // Si se cumplen las condiciones se indica que gano X(1)
		hay_ganador = 1; // Se indica que hubo un ganador
		es_empate = 0; // Se indica que no se dio empate
		// Se muestra el resultado en la ventana pop-up
		gtk_label_set_text(GTK_LABEL(etiqueta_popup), (const gchar*) "Ganó X");
		gtk_widget_show(ventana_popup);

	}

// Luego se analiza el caso en que el jugador O puede ganar
	else if(jugador == 1 && matriz[0][0] != NULL && matriz[0][0] == matriz[0][1] && matriz[0][0] == matriz[0][2]
	|| jugador == 1 && matriz[1][0] != NULL && matriz[1][0] == matriz[1][1] && matriz[1][0] == matriz[1][2]
	|| jugador == 1 && matriz[2][0] != NULL && matriz[2][0] == matriz[2][1] && matriz[2][0] == matriz[2][2]

	|| jugador == 1 && matriz[0][0] != NULL && matriz[0][0] == matriz[1][0] && matriz[0][0] == matriz[2][0]
	|| jugador == 1 && matriz[0][1] != NULL && matriz[0][1] == matriz[1][1] && matriz[0][1] == matriz[2][1]
	|| jugador == 1 && matriz[0][2] != NULL && matriz[0][2] == matriz[1][2] && matriz[0][2] == matriz[2][2]

	|| jugador == 1 && matriz[0][0] != NULL && matriz[0][0] == matriz[1][1] && matriz[0][0] == matriz[2][2]
	|| jugador == 1 && matriz[0][2] != NULL && matriz[0][2] == matriz[1][1] && matriz[0][2] == matriz[2][0]){
		ganador = 0; // Si se cumplen las condiciones se indica que gano O(0)
		hay_ganador = 1; // Se indica que hubo un ganador
		es_empate = 0; // Se indica que no hubo un empate
		// Se muestra el resultado en la ventana pop-up
		gtk_label_set_text(GTK_LABEL(etiqueta_popup), (const gchar*) "Ganó O");
		gtk_widget_show(ventana_popup);
	}
	// Si el contador llega a 9 todas las casillas han sido ocupadas sin victoria
	else if (contador == 9) {
		// Se imprime empate en la ventana pop-up
		gtk_label_set_text(GTK_LABEL(etiqueta_popup), (const gchar*) "Empate!");
		gtk_widget_show(ventana_popup);
	}
	return 0;
}

// Se crea una funcion que muestra el menu (boton de menu)
void jugar(GtkButton *boton) {
	gtk_widget_show(ventana_juego);
	gtk_widget_hide(ventana_menu);
}

// Se crea una funcion para salir del menu (boton de salir)
void salir(GtkButton *boton) {
	gtk_widget_destroy(ventana_menu);
}

// Se crea una matriz inicial que esta llena de 0s en todas sus posiciones
static int boton_seleccionado[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

// Se crea una funcion que designe la imagen correspondiente al caracter digitado
// por el jugador en esa posicion
void accion_boton(int fila, int columna, GtkButton *boton) {
	printf("\a");
	// Se crea el caso en el que no se ha ganado ni se haya asignado un valor
	if (boton_seleccionado[fila][columna] == 0 && hay_ganador == 0 && es_empate == 0) {
		// Se define que la posicion fue llenada por una O cuando el jugador que actuo
		// fue 0
		if (jugador == 0) {
			// Se muestra en pantalla que el siguiente movimiento lo tendra X
			gtk_label_set_text(GTK_LABEL(etiqueta_turno), (const gchar*) "Turno de: X");
			matriz[fila][columna] = "O";
			// Se imprime la imagen
			gtk_button_set_image(GTK_BUTTON(boton), gtk_image_new_from_file("o.png"));
			jugador = 1; // Se pasa al jugador 1 (X)
			analiza_ganador();  // Se determina si en este movimiento hubo ganador
		}
		// Se define que la posicion fue llenada por una X cuando el jugador que actuo
		// fue 1
		else if (jugador == 1) {
			// Se muestra en pantalla que el siguiente movimiento lo tendra O
			gtk_label_set_text(GTK_LABEL(etiqueta_turno), (const gchar*) "Turno de: O");
			matriz[fila][columna] = "X";
			// Se imprime la imagen
			gtk_button_set_image(GTK_BUTTON(boton), gtk_image_new_from_file("x.png"));
			jugador = 0; // Se pasa al jugador 0 (O)
			analiza_ganador();  // Se determina si en este movimiento hubo ganador
		}
		boton_seleccionado[fila][columna] = 1;
	}
}


// Se deben crear las funciones correspondientes a accionar cada boton en
// cada posicion posible

// Arriba izquierda
void ai(GtkButton *boton) {
	printf("\a");
	accion_boton(0, 0, boton); // Se llama a la funcion que activa el boton
}

// Medio izquierda
void mi(GtkButton *boton) {
	accion_boton(1, 0, boton); // Se llama a la funcion que activa el boton
}

// Abajo izquierda
void bi(GtkButton *boton) {
	accion_boton(2, 0, boton); // Se llama a la funcion que activa el boton
}

// Arriba medio
void am(GtkButton *boton) {
	accion_boton(0, 1, boton); // Se llama a la funcion que activa el boton
}

// Medio medio
void mm(GtkButton *boton) {
	accion_boton(1, 1, boton); // Se llama a la funcion que activa el boton
}

// Abajo medio
void bm(GtkButton *boton) {
	accion_boton(2, 1, boton); // Se llama a la funcion que activa el boton
}

// Arriba derecha
void ad(GtkButton *boton) {
	accion_boton(0, 2, boton); // Se llama a la funcion que activa el boton
}

// Medio derecha
void md(GtkButton *boton) {
	accion_boton(1, 2, boton); // Se llama a la funcion que activa el boton
}

// Abajo derecha
void bd(GtkButton *boton) {
	accion_boton(2, 2, boton); // Se llama a la funcion que activa el boton
}

// Se crea una funcion que reinicie el juego
void reiniciar_juego(){
	int i;
	int j;
	jugador = 0; // Se vuelve a definir que el jugador a moverse sera 0

	// Se imprima una imagen vacia en cada boton para comenzar de nuevo
	gtk_button_set_image(GTK_BUTTON(boton_ai), gtk_image_new_from_file("empty.png"));
	gtk_button_set_image(GTK_BUTTON(boton_am), gtk_image_new_from_file("empty.png"));
	gtk_button_set_image(GTK_BUTTON(boton_ad), gtk_image_new_from_file("empty.png"));
	gtk_button_set_image(GTK_BUTTON(boton_mi), gtk_image_new_from_file("empty.png"));
	gtk_button_set_image(GTK_BUTTON(boton_mm), gtk_image_new_from_file("empty.png"));
	gtk_button_set_image(GTK_BUTTON(boton_md), gtk_image_new_from_file("empty.png"));
	gtk_button_set_image(GTK_BUTTON(boton_bi), gtk_image_new_from_file("empty.png"));
	gtk_button_set_image(GTK_BUTTON(boton_bm), gtk_image_new_from_file("empty.png"));
	gtk_button_set_image(GTK_BUTTON(boton_bd), gtk_image_new_from_file("empty.png"));

	// Como el jugador inicial era O, se vuelve a definir
	gtk_label_set_text(GTK_LABEL(etiqueta_turno), (const gchar*) "jugador: O");
	for(i=0; i<3; i++){
		for(j=0; j<3; j++){
			boton_seleccionado[i][j] = 0; // Se redefine que al inicio comienza O
			matriz[i][j] = NULL; // El resto de posiciones se vuelven nulas
		}
	}
	hay_ganador = 0; // Se define que no hay ganador
	ganador = 2; // El ganador no ha sido ni X ni O
	es_empate = 0; // No se ha dado empate
	contador = 0; // El contador de turnos se reinicia
}

// Se crea la funcion del boton que devuelve al usuario al menu
void menu(GtkButton *boton) {
	// Se muestra la ventana de menu original
	gtk_widget_show(ventana_menu);
	gtk_widget_hide(ventana_juego);
	reiniciar_juego(); // Se reinicia el juego
}

// Se crea la funcion del boton que reinicia el juego
void reiniciar(GtkButton *boton){
	reiniciar_juego(); // Se reinicia el juego
}

// Funcion main
int main(int argc, char* argv[]){

	// Se inicializa gtk
  gtk_init(&argc, &argv);

	// Se define de donde se extraera el glade
  builder = gtk_builder_new_from_file("interfaz_proyecto.glade");

	// Se define la ventana de glade de donde se extrae el menu
  ventana_menu = GTK_WIDGET(gtk_builder_get_object(builder, "ventana_menu"));

	// Se define la ventana de glade de donde se extrae el juego en si
	ventana_juego = GTK_WIDGET(gtk_builder_get_object(builder, "ventana_juego"));

	// Se define el boton de glade de donde se extrae el boton jugar
	boton_jugar = GTK_WIDGET(gtk_builder_get_object(builder, "boton_jugar"));

	// Se define el boton de glade de donde se extrae el boton salir
	boton_salir = GTK_WIDGET(gtk_builder_get_object(builder, "boton_salir"));

	// Se define el boton de glade de donde se extrae el boton ai
	boton_ai = GTK_WIDGET(gtk_builder_get_object(builder, "boton_ai"));

	// Se define el boton de glade de donde se extrae el boton am
	boton_am = GTK_WIDGET(gtk_builder_get_object(builder, "boton_am"));

	// Se define el boton de glade de donde se extrae el boton ad
	boton_ad = GTK_WIDGET(gtk_builder_get_object(builder, "boton_ad"));

	// Se define el boton de glade de donde se extrae el boton mi
	boton_mi = GTK_WIDGET(gtk_builder_get_object(builder, "boton_mi"));

	// Se define el boton de glade de donde se extrae el boton mm
	boton_mm = GTK_WIDGET(gtk_builder_get_object(builder, "boton_mm"));

	// Se define el boton de glade de donde se extrae el boton md
	boton_md = GTK_WIDGET(gtk_builder_get_object(builder, "boton_md"));

	// Se define el boton de glade de donde se extrae el boton bi
	boton_bi = GTK_WIDGET(gtk_builder_get_object(builder, "boton_bi"));

	// Se define el boton de glade de donde se extrae el boton bm
	boton_bm = GTK_WIDGET(gtk_builder_get_object(builder, "boton_bm"));

	// Se define el boton de glade de donde se extrae el boton bd
	boton_bd = GTK_WIDGET(gtk_builder_get_object(builder, "boton_bd"));

	// Se define el boton de glade de donde se extrae el boton reinciar
	boton_reiniciar = GTK_WIDGET(gtk_builder_get_object(builder, "boton_reiniciar"));

	// Se define el boton de glade de donde se extrae el boton de vuelta al menu
	boton_menu = GTK_WIDGET(gtk_builder_get_object(builder, "boton_menu"));

	// Se define la etiqueta de glade de donde se extrae la etiqueta de turno
	etiqueta_turno = GTK_WIDGET(gtk_builder_get_object(builder, "etiqueta_turno"));

	// Se define la etiqueta de glade de donde se extrae la etiqueta de pop-up
	etiqueta_popup = GTK_WIDGET(gtk_builder_get_object(builder, "etiqueta_popup"));

	// Se define la ventana de glade de donde se extrae la ventana de pop-up
	ventana_popup = GTK_WIDGET(gtk_builder_get_object(builder, "ventana_popup"));

	// Se conectan todas las senales definidas
	gtk_builder_connect_signals(builder, NULL);

	// Se define que cuando se vuelve al menu o se pasa al juego se termina lo
	// ocurrido hasta el momento
  g_signal_connect(ventana_menu, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(ventana_juego, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	// Se relacionan los botones a su respectiva accion
  g_signal_connect(boton_jugar, "clicked", G_CALLBACK(jugar), NULL);
  g_signal_connect(boton_salir, "clicked", G_CALLBACK(salir), NULL);
  g_signal_connect(boton_ai, "clicked", G_CALLBACK(ai), NULL);
  g_signal_connect(boton_am, "clicked", G_CALLBACK(am), NULL);
  g_signal_connect(boton_ad, "clicked", G_CALLBACK(ad), NULL);
  g_signal_connect(boton_mi, "clicked", G_CALLBACK(mi), NULL);
  g_signal_connect(boton_mm, "clicked", G_CALLBACK(mm), NULL);
  g_signal_connect(boton_md, "clicked", G_CALLBACK(md), NULL);
  g_signal_connect(boton_bi, "clicked", G_CALLBACK(bi), NULL);
	g_signal_connect(boton_bm, "clicked", G_CALLBACK(bm), NULL);
	g_signal_connect(boton_bd, "clicked", G_CALLBACK(bd), NULL);
	g_signal_connect(boton_reiniciar, "clicked", G_CALLBACK(reiniciar), NULL);
	g_signal_connect(boton_menu, "clicked", G_CALLBACK(menu), NULL);

	// Se muestra la ventana principal de menu
  gtk_widget_show_all(ventana_menu);

  gtk_main();

  return 0;
}
