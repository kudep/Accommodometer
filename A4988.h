#ifndef A4988_H
#define A4988_H

#define FORWARD true
#define BACK false

#define DELAY_PULSE_US 3
#define DELAY_BETWEEN_US 180
#define STEPS 2

class A4988 {
	public:
		// Constructors:
		A4988(int En_n, int Dir, int Stp, int H_trlr, int F_trlr); //Eneble, Direct, Step, Home trailer, Foredg trailer
                
		//Init method:
                void init(void);
                
		//coord get method:
		long int get_coord(void);

		//bias get method:
		long int get_bias(void);

		//count get method:
		int get_count(void);

		//direct get method:
		bool get_direct(void);

		//count is clear method:
		void clr_count(void);

		//beas is clear method:
		void clr_bias(void);

		//go to home method:
		void home(void);

		//go to back method:
		void return_back(void);

		//go to the point method:
		void go_to(int st, bool _direct);
	private:
		//step method:
		void step(int count);
                
		//reverse method:
		void reverse(void);

		//forward method:
		void forward(void);

		//back method:
		void back(void);

		//home trailer method:
		bool h_trailer(void);

		//foredg trailer method:
		bool f_trailer(void);


                void comptable(bool direct);
		bool direct;        // Direction of rotation

		void clr_coord(void);
		
 		// motor pin numbers:
		int en_n;
		int dir;
		int stp;
		int h_trlr;
		int f_trlr;

 		// count:
		long int cnt;
		long int bias;
		long int coord;

};

#endif
