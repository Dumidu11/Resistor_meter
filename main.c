#include <avr/io.h>
#include <util/delay.h>

#define ADC_READ PORTA
#define ADC_READ_DDR DDRA

#define SSD_DATA PORTC
#define SSD_DATA_DDR DDRC

#define SSD_DIG_DDR DDRB
#define SSD_DIG PORTB

#define dig_delay _delay_ms(20)
#define dig_delay1 _delay_ms(10)
#define dig_delay2 _delay_ms(10)

#define RES_VAL_DDR DDRD
#define RES_VAL PORTD


#define DIG0 0
#define DIG1 1
#define DIG2 2
#define DIG3 3
#define DIG4 4
#define DIG5 5
#define DIG6 6
#define DIG7 7



void init_adc(void);
unsigned int read_adc(unsigned char cannel);
void init_ssd(void);
void ssd_display(void);
void split_data( unsigned num);
void decimal (void);
void decimal2 (void);
void decimal3 (void);


unsigned char ssd[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
unsigned char ssd_dig[]={0, 0, 0, 0};

int main(void){

	float ad_val;
	init_adc();
	init_ssd();
	
	
	
	
	while(1){
	        
	       
	    RES_VAL_DDR= ~(1<<DIG1 | 1<<DIG2); //first set to 10k path and pin1 and pin2 set to input  
		ad_val=read_adc(2);// channel 2 -> ADC2 // do something
        float volt_val=(ad_val/1023)*5;//reading of the ADC value convrt the Voltage
		float rest_val=(50000/volt_val)-10000;// voltage value convert to resistor value (using voltage deviding method) first cheack on 10k resistor
	
		if(rest_val<=100){
		
		     RES_VAL_DDR=~(1<<DIG0| 1<<DIG1);
			 RES_VAL=1<<DIG3;
			 
			 while(1){
			 
			    ad_val=read_adc(2);// channel 2 -> ADC2 // do something
                float volt_val=(ad_val/1023)*5;
		        float rest_val=(500/volt_val)-100; //  cheack on 100 resistor
			    dig_delay1;
				float num=rest_val;
				split_data(num);
				ssd_display();
			}
		
		}
		
		else if((rest_val>100) && (rest_val<1000)){
		
		     RES_VAL_DDR=~(1<<DIG0| 1<<DIG2);
			 RES_VAL=1<<DIG4;
			 
			 while(1){
			 
			    ad_val=read_adc(2);// channel 2 -> ADC2 // do something
                float volt_val=(ad_val/1023)*5;
		        float rest_val=(5000/volt_val)-1000; // cheack on 1000 resistor
			    dig_delay1;
				float num=rest_val;
				split_data(num);
				ssd_display();
				decimal3();
			}
		
		}
		 

        else if((rest_val>=1000) && (rest_val<10000)){
		
		     RES_VAL_DDR=~(1<<DIG1| 1<<DIG2);
			 RES_VAL=1<<DIG5;
			 
			 while(1){
			 
			    ad_val=read_adc(2);// channel 2 -> ADC2 // do something
                float volt_val=(ad_val/1023)*5;
		        float rest_val=(50000/volt_val)-10000; //  cheack on 10000 resistor
			    dig_delay1;
				float num=rest_val;
				split_data(num);				 
				ssd_display();
				decimal();
			}
		
		}
		
		
         

	}
	return 0;
}

void init_adc(void){
	ADMUX = 1<< REFS0;
	ADCSRA = 1<<ADEN | 1<<ADPS1 | 1<<ADPS0;
}
unsigned int read_adc(unsigned char channel){
	ADMUX = ADMUX | (channel & 0x0f);
	ADCSRA = ADCSRA | 1<<ADSC;
	while(ADCSRA & (1<<ADSC)){
	}
	return ADC;
}



void init_ssd(void){

    SSD_DATA=0x00;
	 ADC_READ_DDR=0x00;
    RES_VAL_DDR=0xff;
    SSD_DATA_DDR = 0xff;
	SSD_DATA &=~(1<<DIG0 | 1<<DIG1 | 1<<DIG2 | 1<<DIG3| 1<<DIG4| 1<<DIG5| 1<<DIG6);
    SSD_DIG_DDR = SSD_DIG_DDR | 1<<DIG0 | 1<<DIG1 | 1<<DIG2 | 1<<DIG3 | 1<<DIG4 | 1<<DIG5 | 1<<DIG6;
}
 

void ssd_display(void){

  

	SSD_DATA=ssd[ssd_dig[0]];
	//PORTA=ssd[5];
	SSD_DIG=1<<DIG0;
	dig_delay;
 
	SSD_DATA=ssd[ssd_dig[1]];
	//PORTA=ssd[4];
	SSD_DIG=1<<DIG1;
	 dig_delay;
	
	SSD_DATA=ssd[ssd_dig[2]];
	//PORTA=ssd[5];
	SSD_DIG=1<<DIG2;
	  dig_delay;
	 
	SSD_DATA=ssd[ssd_dig[3]];
	//PORTA=ssd[9];
	SSD_DIG=1<<DIG3;
	 dig_delay;
	 
	 

   
}

void split_data( unsigned int num) {
	ssd_dig[0]=num/1000;
	ssd_dig[3]=num%10;
	ssd_dig[1]=(num%100)/10;
	ssd_dig[2]=(num%10)/10;
 
}

void decimal (void){

  SSD_DIG= SSD_DIG | (1<<DIG0);
  SSD_DATA=  (1<<DIG7);
  SSD_DIG= SSD_DIG &~ (1<<DIG3);
  dig_delay2;


}

void decimal2 (void){

  SSD_DIG= SSD_DIG | (1<<DIG1);
  SSD_DATA=  (1<<DIG7);
  SSD_DIG= SSD_DIG &~ (1<<DIG3);
  dig_delay2;


}

void decimal3 (void){

  SSD_DIG= SSD_DIG | (1<<DIG2);
  SSD_DATA=  (1<<DIG7);
  SSD_DIG= SSD_DIG &~ (1<<DIG3);
  dig_delay2;


}
