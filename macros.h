/*!
  \file   macros.h
  \brief  Macros de pantalla
  \author Nicolás Luis Fernández García
  \date   17 de febrero de 2017
  \note  http://isthe.com/chongo/tech/comp/ansi_escapes.html
*/

#ifndef __MACROS_H__
#define __MACROS_H__

#include <stdio.h>

/*! LUGAR */
#define LUGAR(x,y)   printf("\033[%d;%dH",x,y) 

/*! Borra la pantalla */
#define CLEAR_SCREEN printf("\33[2J") 

/*! Borra hasta el final de línea */
#define CLEAR_REST_OF_LINE  printf("\33[K") 

/*! desactiva */
#define RESET	     printf("\e[0m") 

/*! intesidad */
#define INTENSITY  printf("\e[1m") 

/*! apenas visible */
#define FAINT      printf("\e[2m") 

/*! itálica, cursiva */
#define ITALIC     printf("\e[3m") 

/*! subrayado */
#define UNDERLINE  printf("\e[4m") 

/*! parpadeo */
#define BLINK      printf("\e[5m") 

/*! inverso */
#define INVERSE    printf("\e[7m") 


/*! 
  \name  Regular colors 
*/

/*! Black */
#define BLACK		printf("\e[0;30m")  

/*! Red*/
#define RED		  printf("\e[0;31m")  

/*! Green*/
#define GREEN		printf("\e[0;32m")  

/*! Yellow */
#define YELLOW	printf("\e[0;33m")  

/*! Blue */
#define BLUE		printf("\e[0;34m")  

/*! Purple */
#define PURPLE	printf("\e[0;35m")  

/*! Cyan */
#define CYAN		printf("\e[0;36m")  

/*! White */
#define WHITE		printf("\e[0;37m")  

/*! 
  \name   Bold 
*/

/*! Black */
#define BBLACK	printf("\e[1;30m")  

/*! Red */
#define BRED		printf("\e[1;31m")  

/*! Green */
#define BGREEN	printf("\e[1;32m")  

/*! Yellow */
#define BYELLOW	printf("\e[1;33m")  

/*! Blue */
#define BBLUE		printf("\e[1;34m")  

/*! Purple */
#define BPURPLE	printf("\e[1;35m")  

/*! Cyan */
#define BCYAN		printf("\e[1;36m")  

/*! White */
#define BWHITE	printf("\e[1;37m")  

/*! 
  \name   Underline 
*/

/*! Black */
#define UBLACK	printf("\e[4;30m")  

/*! Red */
#define URED		printf("\e[4;31m")  

/*! Green */
#define UGREEN	printf("\e[4;32m")  

/*! Yellow */
#define UYELLOW	printf("\e[4;33m")  

/*! Blue */
#define UBLUE		printf("\e[4;34m")  

/*! Purple */
#define UPURPLE	printf("\e[4;35m")  

/*! Cyan */
#define UCYAN		printf("\e[4;36m")  

/*! White */
#define UWHITE	printf("\e[4;37m")  

/*! 
  \name 	Background 
*/

/*! Black */
#define ONBLACK	 printf("\e[40m")	 

/*! Red */
#define ONRED		 printf("\e[41m")  

/*! Green */
#define ONGREEN	 printf("\e[42m")

/*! Yellow */
#define ONYELLOW printf("\e[43m")

/*! Blue */
#define ONBLUE	 printf("\e[44m")  

/*! Purple */
#define ONPURPLE printf("\e[45m")	 

/*! Cyan */
#define ONCYAN	 printf("\e[46m")  

/*! White */
#define ONWHITE	 printf("\e[47m")	 

/*! 
    \name  high intensity
*/

/*! Black */
#define IBLACK	 printf("\e[0;90m")  

/*! Red */
#define IRED		 printf("\e[0;91m")  

/*! Green */
#define IGREEN	 printf("\e[0;92m")  

/*! Yellow */
#define IYELLOW	 printf("\e[0;93m")  

/*! Blue */
#define IBLUE		 printf("\e[0;94m")  

/*! Purple */
#define IPURPLE	 printf("\e[0;95m")  

/*! Cyan */
#define ICYAN		 printf("\e[0;96m")  

/*! White */
#define IWHITE	 printf("\e[0;97m")  

/*! 
  \name 	Bold high intensity 
*/

/*! Black */
#define BIBLACK	 printf("\e[1;90m")  

/*! Red */
#define BIRED		 printf("\e[1;91m")  

/*! Green */
#define BIGREEN	 printf("\e[1;92m")  

/*! Yellow */
#define BIYELLOW printf("\e[1;93m")	 

/*! Blue */
#define BIBLUE	 printf("\e[1;94m")  

/*! Purple */
#define BIPURPLE printf("\e[1;95m")	 

/*! Cyan */
#define BICYAN	 printf("\e[1;96m")  

/*! White */
#define BIWHITE	 printf("\e[1;97m")  

/*! 
  \name   High intensity backgrounds 
*/

/*! Black */
#define ONIBLACK	printf("\e[0;100m")	

/*! Red */
#define ONIRED		printf("\e[0;101m")  

/*! Green */
#define ONIGREEN	printf("\e[0;102m")	 

/*! Yellow */
#define ONIYELLOW	printf("\e[0;103m")	 

/*! Blue */
#define ONIBLUE		printf("\e[0;104m")	 

/*! Purple */
#define ONIPURPLE	printf("\e[0;105m")	 

/*! Cyan */
#define ONICYAN		printf("\e[0;106m")	 

/*! White */
#define ONIWHITE	printf("\e[0;107m")


#endif

