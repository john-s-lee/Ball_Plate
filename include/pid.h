typedef struct
{
	double kc;  //proportional
	double tauI;  //Integral
	double tauD;   //Derivative
	double tauF;  //Filter
	double u_D;   //Derivative Term
	double u_D_past;  //Past Derivative term u_D(t_i - delta_t)
	double pos_curr;  //Current Ball Position
	double pos_past;   //Past Ball Position
	double u_act;   //Control Signal
	double u_act_past;   //Past Control Signal
	double set_pt;  //Desire position of ball
	double error;  //current error r(t) - y(t)
}pid_params;

void stable_mode();

