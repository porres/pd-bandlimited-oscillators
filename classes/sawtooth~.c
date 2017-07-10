#include "m_pd.h"

static t_class *sawtooth_class;

typedef struct _sawtooth
{
    t_object x_obj;
    double  x_phase;
    t_float  x_freq;
    t_outlet *x_outlet;
    t_float x_sr;
} t_sawtooth;

static t_int *sawtooth_perform(t_int *w)
{
    t_sawtooth *x = (t_sawtooth *)(w[1]);
    int nblock = (t_int)(w[2]);
    t_float *in1 = (t_float *)(w[3]); // freq
    t_float *out = (t_float *)(w[4]);
    double phase = x->x_phase;
    while (nblock--)
        {
        float hz = *in1++;
        double phase_step = (double)(hz / x->x_sr); // phase_step
        if (phase <= 0)
            phase = phase + 1.; // wrap deviated phase
        if(phase >= 1)
            phase = phase - 1.; // wrap deviated phase
        *out++ = phase * -2 + 1;
        phase += phase_step; // next phase
        }
    x->x_phase = phase;
    return (w + 5);
}

static void sawtooth_dsp(t_sawtooth *x, t_signal **sp)
{
    dsp_add(sawtooth_perform, 4, x, sp[0]->s_n,
            sp[0]->s_vec, sp[1]->s_vec);
}

static void *sawtooth_new(t_floatarg f1)
{
    t_sawtooth *x = (t_sawtooth *)pd_new(sawtooth_class);
    x->x_freq = f1;
    x->x_sr = sys_getsr(); // sample rate
    x->x_outlet = outlet_new(&x->x_obj, &s_signal);
    return (x);
}

void sawtooth_tilde_setup(void)
{
    sawtooth_class = class_new(gensym("sawtooth~"),
        (t_newmethod)sawtooth_new, 0,
        sizeof(t_sawtooth), CLASS_DEFAULT, A_DEFFLOAT, 0);
    CLASS_MAINSIGNALIN(sawtooth_class, t_sawtooth, x_freq);
    class_addmethod(sawtooth_class, (t_method)sawtooth_dsp, gensym("dsp"), A_CANT, 0);
}
