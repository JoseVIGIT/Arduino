/*
   LED Blink en C
   enciende y apaga el LED13 (en placa arduino) cada 3 segundos (4 iter.)
 */
 
#include <termios.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main () {

   #define LED_ON "1"
   #define LED_OFF "2"
   #define LED_ESTADO "3"
   
   struct termios config;
   
   /*
      device = puerto del dispositivo
      msg = mensaje enviado al puerto ( 1=enciende led ; 2=apaga led)
      resp ? respuesta de Arduino (1=encendido; 2=apagado)
      led = texto a mostrar con estado actual del led
      fd = descriptor de fichero/puerto que se usa
      cont = contador de iteraciones realizadas en el blink del led
      comparacion = valor resultado de comparar estado actual con "encendido"
    */
   const char device [12] = "/dev/ttyACM0";
   char* msg = malloc(1);
   char* resp = malloc(11);
   char* led = malloc (9);
   int fd = 0;
   int cont = 1;
   int comparacion = 0;
   
   
   /*
      abrir puerto y comprobar posibles fallos, si es serial y obtener su
      configuracion
    */
   // fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
   // Sin NDELAY para que read espere datos en el buffer
   fd = open(device, O_RDWR | O_NOCTTY); 
   
   printf ("\n");   
   if(fd == -1) {
      printf( "Puerto no abierto");
      return 1;     
   } else {
      printf ("Puerto abierto (%s). ", device);
   }
      
   if (!isatty(fd)) {
      printf ("No es serial");         
      return 2;
   } else {
      printf ("Es serial");
   }   
   printf ("\n");
   
   if(tcgetattr(fd, &config) < 0) {
      printf ("Lectura de config incorrecta");
      return 3;
   }
      
   printf ("Configurando puerto y esperando reset de Arduino\n"); 
   
   cfsetospeed(&config, B9600); // velocidad de salida
   cfsetispeed(&config, B9600); // velocidad de entrada     

   config.c_cflag &= ~PARENB; // sin paridad
   config.c_cflag &= ~CSTOPB; // sin bit de stop
   config.c_cflag &= ~CSIZE;
   config.c_cflag |= CS8; // 8bits
   config.c_cflag &= ~CRTSCTS; // sin control de fljo por hw   
   config.c_cflag |= CREAD | CLOCAL; // enable receiver, ignore status lines   
   config.c_iflag &= ~(IXON | IXOFF | IXANY); // disable input/output flow control, disable restart chars
   config.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG); // disable canonical input, dis. echo, dis. visually erase chars, dis. terminal-generated signals
   config.c_oflag &= ~OPOST; // disable output processing
   config.c_cc[VMIN] = 1; // wait for characters to come in before read returns
   config.c_cc[VTIME] = 0; // minimum time to wait before read returns 
   
   tcsetattr(fd, TCSANOW, &config); // commit the options
   
   sleep(3); // Wait for the Arduino to reset   
   
   tcflush(fd, TCIOFLUSH); // Flush anything already in the serial buffer
   
   printf ("\n");

   /*
      cuatro iteraciones
      se comprueba si esta encendido o apagado y se cambia su estado
      se escribe en el puerto y se espera
    */
   while (cont < 5) {    
      printf ("Iteracion %i\n", cont);   
   
      // Solicitar estado actual del LED para averiguar a qué cambiar
      strcpy(resp, "*");
      write(fd, LED_ESTADO, 1);      
      read(fd, resp, 1); 
      comparacion = strcmp(resp, LED_ON);       
      if (comparacion==0) {
         strcpy (led, "encendido");
      } else {
         strcpy (led, "apagado");
      }      
      printf ("Estado previo     (arduino): %s (%s) \n", resp, led);
      
      // Solicitar cambio de estado en LED
      if (comparacion == 0) { // resp = 1
         strcpy(msg, LED_OFF);
         strcpy(led, "apagado"); 
      } else {
         strcpy(msg, LED_ON);
         strcpy(led, "encendido"); 
      }
      write(fd, msg, sizeof(msg));
      printf ("Estado solicitado (pc)     : %s (%s) \n", msg, led);
      
      // Solicitar estado actual del LED para confirmar el cambio
      strcpy(resp, "*");
      write(fd, LED_ESTADO, 1);      
      read(fd, resp, 1);      
      printf ("Estado confirmado (arduino): %s (%s) \n", resp, led); 

      printf ("\n");      
      fflush(stdout);
      cont++;
      comparacion = 0;

      sleep(3);
   }

   close(fd);
   printf( "\nAcabado\n\n" );
   return 0;
}

