

typedef struct {
    float K_p;
    float K_i;
    float T;
    int error;
    int prev_error;
    int max_u;
} PID_DATA;


void pid_init(float k_p, float k_i, float timestep, int max_u) {
    PID.K_p = k_p;
    PID.K_i = k_i;

    PID.error = 0;
    PID.prev_error = 0;
    PID.T = timestep;
    PID.max_u = max_u;
}
int pid_controler(int y, int u) {
    int e = y - u;
    PID.error += e;

    int u_p = PID.K_p * e;
    int u_i = PID.T * PID.K_i * PID.error;
    int u = u_p + u_i;

    PID.prev_error = e;

    if (u > PID.max_u) {
        u = PID.max_u;
    }
    else if (u < -PID.max_u) {
        u = -PID.max_u;
    }

    return u;
}
#define EN PD0
#define DIR PD1
#define RST PD3
#define SEL PD2

void reset_motor() {}

void motor_init() {}
