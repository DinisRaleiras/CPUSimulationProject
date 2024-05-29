/*

CPU-Mem architecture

AC 2023/24  LEI FCT/UNL



256 8-bit data words

256 16-bit data words

8 bit addresses (max memory 2^8)

1 acumulator

flags: zero, carry, overflow



16 bit instruction

    4 bits 15-12 opcode

    4 bits 11-8 specification of ALU operaation ALU 00 ADD 01 SUB

    8 bits signed value or address

*/

#include <stdio.h>





extern unsigned char Mem[];

extern unsigned short int Code[];

extern char ac;

extern unsigned char flagZero;

extern unsigned char flagCarry;

extern unsigned char flagOverflow;







void flags(unsigned char opALU, short value){

	if(opALU == 0x00){

		flagOverflow = (ac > 0 && value > 0 && ac + value < 0) 

              	|| (ac < 0 && value < 0 && ac + value > 0) ? 1 : 0;

              	flagCarry = (ac < 0 && value < 0) || ((ac < 0 || value < 0) && (ac + value >= 0)) ? 1 : 0;

              	flagZero = (ac + value == 0) ? 1 : 0;

	}else if(opALU == 0x01){

		flagOverflow = (ac > 0 && value < 0 && ac - value < 0) 

              	|| (ac < 0 && value > 0 && ac - value > 0) ? 1 : 0;

              	flagCarry = (ac < 0 && value > 0) || ((ac < 0 || value > 0) && (ac - value >= 0)) ? 1 : 0;

              	flagZero = (ac - value == 0) ? 1 : 0;

	}

}







void dorun(){

    unsigned short int pc;  // program counter or intruction pointer

    unsigned short int ir;  // instruction register



    unsigned char opcode;

	unsigned char opALU;

	short addressOrValue;

	

    pc = 0;

    while( 1 ) {

        ir = Code[pc]; // FETCH

        pc += 1;              

                         

        opcode = (unsigned char)(ir >> 12);             // DECODE

	opALU = (unsigned char)((ir & 0x0f00) >> 8);	

        addressOrValue =  ir & 0x00ff;

        

        

        switch( opcode ){              // EXECUTE

            case 0x0F:   /* HALT */

                printf("HALT instruction executed\n");

                return;



            case 0x00:   /* NOOP */

                break;

       

            

            case 0x01:  /* ADDI / SUBI */

              	if(opALU == 0x00){

              		flags(opALU, addressOrValue);

              		ac += addressOrValue;

              	}else if(opALU == 0x01){

              		flags(opALU, addressOrValue);

              		ac -= addressOrValue;

              	}else{

              		printf("Invalid operation\n");

              	}

                break;

            

            case 0x02: /* ADD, SUB */

                if(opALU == 0x00){

              		flags(opALU, Mem[addressOrValue]);

              		ac += Mem[addressOrValue];

              	}else if(opALU == 0x01){

              		flags(opALU, Mem[addressOrValue]);

              		ac -= Mem[addressOrValue];

              	}else{

              		printf("Invalid operation\n");

              	}          

                break;

            

            case 0x03: /* CLAC */

                ac = 0;

                flagZero = 1;

                break;

            

            case 0x04: /* STORE */

   		Mem[addressOrValue] = ac;

                break;

			

	      case 0x05: /* BEQZ */

 		if(flagZero == 1){

 			pc = addressOrValue;

 		}

                break;

	     

            default:

                printf("Invalid instruction!\n");

                return;

        }

    }

}

