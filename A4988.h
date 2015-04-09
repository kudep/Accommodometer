/* ver 0.7 
03.04.2015
*/
#ifndef A4988_H
#define A4988_H

#define FORWARD true
#define BACK false

#define DELAY_PULSE_US 3
#define DELAY_BETWEEN_US 180

class A4988 {
	public:
		// constructors:
		A4988(int En_n, int Dir, int Stp); //Eneble, Direct, Step
                
		//Init method:
                void init(void);
                
		//reverse method:
		void reverse(void);

		//forward method:
		void forward(void);

		//back method:
		void back(void);

		//step method:
		void step(int count);

		//count get method:
		int get_count(void);

		//direct get method:
		bool get_direct(void);

		//count is clear method:
		void clr_count(void);
	private:
                void comptable(bool direct);
		bool direct;        // Direction of rotation
		
 		// motor pin numbers:
		int en_n;
		int dir;
		int stp;

 		// count:
		int cnt;

};

#endif
