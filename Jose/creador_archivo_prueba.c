

#define FREC_MUESTREO	44100			//Frecuencia de muestreo del ADC por canal
#define FREC_CH1		1000			//Frecuencia de los diferentes canales
#define FREC_CH2		3000
#define FREC_CH3		5000
#define FREC_CH4		7000
#define TIEMPO_SONID	5				//Tiempo de audio a enviar	
#define CANT_CANALES	3				//Cantidad de canales a guardar en archivo
#define BITS_MUESTRA	16				//Tamaño de la muestra
#define	BUFFSIZE 		220500			//Tamaño del buffer =  TIEMPO_SONID * FREC_MUESTREO




int main (int argc, char *argv[ ])
{
	int senales[4][BUFFSIZE];
	int t;
	int i;

	for (i = 0; i < BUFFSIZE; i++)
	{
		senales[0] = 
		senales[1] = 
		senales[2] = 
		senales[3] = 
	}

}
