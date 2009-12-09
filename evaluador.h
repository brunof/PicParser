//EVALUADOR DE EXPRESIONES EN NOTACION POSTFIJA PARA MICROCONTROLADORES PIC
//Made by Bruno Fascendini!!! 2009 bfascendini@hotmail.com para uControl.
//Libre de uso y modificación siempre y cuando se mantengan estos créditos!

#include <stdlib.h>
#include <ERRNO.h>
#include "math.h"

//#DEFINE     DEBUG_EVALUADOR

//#CASE

char const EvaluadorVer[]="v0.1.0";
float const EPSILON = 2.71828182;

float* strEvaluar(char* strPos,float* FloatRAMBlock,float* x1, float* y1, float* z1){
   char Anterior,Res,Tmp;
   float a1,a2,fltRes;

   *FloatRAMBlock=0;    //Cargar primero operando con cero(por excepciones como si se tiene que analizar "1 -" y no hay siquiera dos operandos...
   FloatRAMBlock++;     //"PUSH"
   Anterior=Nada;       //Para comprobacion de errores(INCOMPLETO!)
   do{
      Res=GetNextTokenType(strPos);
      Tmp=*strPos;
    #ifdef DEBUG_EVALUADOR printf("Cadena:%s Largo:%u\r\n",strPos,Largo); #endif
      
    if(Tmp==ESPACIO){ strPos++; continue;}          //Si se trata de un espacio, esquivar
      
      switch(Res){
         case Numero:
            if(Tmp==PI){ *FloatRAMBlock=PI; FloatRAMBlock++; break;};
            if(Tmp==E){ *FloatRAMBlock=EPSILON;  FloatRAMBlock++; break;};
            
            //Numero float "explícito"
            *(strPos+Largo)='\0';            //"Engañar" al ATOF para que piense que en lugar del Espacio hay un fin de string
            *FloatRAMBlock=atof(strPos);
            fltRes=*FloatRAMBlock;
            #ifdef DEBUG_EVALUADOR printf("Encontrado numero: %f\r\n",fltRes);  #endif 
            FloatRAMBlock++;                              //"PUSH"
            *(strPos+Largo)=ESPACIO;         //Restaurarlo
         break;
      
         case Variable:
            if(Tmp==X) *FloatRAMBlock=*x1;
            if(Tmp==Y) *FloatRAMBlock=*y1;
            if(Tmp==Z) *FloatRAMBlock=*z1;
            FloatRAMBlock++;                              //"PUSH"
         break;

         case Prior0:
         case Prior1:
         case Prior2:         
            FloatRAMBlock--;           //Estas llevan 2 operandos...//"POP"
            a2=*FloatRAMBlock;
         case FuncionConParentesis:    //Estas son de sólo uno...
            FloatRAMBlock--;           //"POP"
            a1=*FloatRAMBlock;
            #ifdef DEBUG_EVALUADOR printf("El argumento a1=%f y el a2=%f\r\n",a1,a2);  #endif
   
            //Funciones con 2 operadores
            if(Tmp==POT) fltRes=pow(a1,a2);     //HAY UN ERROR DESCONOCIDO ACÁ! Al hacer pow(-32.0,2.0) da como resultado -1024! Ilógico!
            if(Tmp==POR) fltRes=a1*a2;
            if(Tmp==DIVI) fltRes=a1/a2;
            if(Tmp==MOD) fltRes=(int32)a1%(int32)a2;
            if(Tmp==MAS) fltRes=a1+a2;
            if(Tmp==MENOS) fltRes=a1-a2;
   
   
            //Funciones con 1 solo operador
            if(Tmp==LOGN) fltRes=log(a1);
            if(Tmp==LOGA) fltRes=log10(a1);
            if(Tmp==ABSOLUTO) fltRes=abs(a1);
            if(Tmp==ROUND) fltRes=floor(a1);
            if(Tmp==RAIZ) fltRes=sqrt(a1);
   
            if(Tmp==SENO) fltRes=sin(a1);
            if(Tmp==COSENO) fltRes=cos(a1);
            if(Tmp==TANGENTE) fltRes=tan(a1);
            if(Tmp==SECANTE) fltRes=1/sin(a1);
            if(Tmp==COSECANTE) fltRes=1/cos(a1);
            if(Tmp==COTANGENTE) fltRes=1/tan(a1);
   
            if(Tmp==SIGNO) if(a1>=0) fltRes=1; else fltRes=0;
   
            if(Tmp==ASENO) fltRes=asin(a1);
            if(Tmp==ACOSENO) fltRes=acos(a1);
            if(Tmp==ATANGENTE) fltRes=atan(a1);         
            if(Tmp==ASECANTE) fltRes=1/asin(a1);
            if(Tmp==ACOSECANTE) fltRes=1/acos(a1);         
            if(Tmp==ACOTANGENTE) fltRes=1/atan(a1);
   
            if(Tmp==SENOH) fltRes=sinh(a1);
            if(Tmp==COSENOH) fltRes=cosh(a1);
            if(Tmp==TANGENTEH) fltRes=tanh(a1);
            if(Tmp==SECANTEH) fltRes=1/sinh(a1);
            if(Tmp==COSECANTEH) fltRes=1/cosh(a1);
            if(Tmp==COTANGENTEH) fltRes=1/tanh(a1);
   
            //if(Tmp==ASENOH) fltRes=asinh(a1);
            //if(Tmp==ACOSENOH) fltRes=acosh(a1);
            //if(Tmp==ATANGENTEH) fltRes=atanh(a1);
            //if(Tmp==ASECANTEH) fltRes=1/asinh(a1);
            //if(Tmp==ACOSECANTEH) fltRes=1/acosh(a1);
            //if(Tmp==ACOTANGENTEH) fltRes=1/atanh(a1);
   
            *FloatRAMBlock=fltRes;                                      //Almacenar Resultado en STACK
            FloatRAMBlock++;                                            //"PUSH"
            #ifdef DEBUG_EVALUADOR printf("EL resultado de la operacion es: %f\r\n",fltRes);  #endif
         break;
      }
      strPos+=Largo;
   }while(Res!=Fin_Eq && Res!=Invalido);

   return --FloatRAMBlock;
}
