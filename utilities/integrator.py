import math
from scipy.integrate import ode
import argparse


def derivatives(t, y, p):
    """
        Defines the differential equations for the full system.
         t :  Time
         y :  Vector of the state variables:
                   y = [n_0, n_1, n_2, ..., n_N, n]
         p :  Dictionary of parameters:
                   rho_n, rho_l, N, s.
    """
    # Initialization
    n = y[-1]
    n_int = math.floor(n)
    x = sum([y[k] for k in range(1, n_int - 1)])
    z = sum([y[k] * (n_int - 1 - k) for k in range(1, n_int - 1)])
    # k_max = math.floor(n - 1)

    # Compute derivatives
    dy = [0] * len(y)

    # Special cases
    dy[0] = 0
    dy[1] = p['rho_n'] * (n - y[1]) -\
        p['rho_l'] * (y[1] + x * (n_int - 2) * y[1] / z)
    dy[n_int - 1] = y[n_int - 2] * (p['rho_n'] + p['rho_l'] * (1 + x / z)) -\
        y[n_int - 1] * p['rho_n']
    dy[n_int] = y[n_int - 1] * p['rho_n']
    # Normal compartments
    for k in range(2, n_int - 1):  # k_max + 1 is *not* computed
        dy[k] = p["rho_n"] * (y[k - 1] - y[k]) +\
            p["rho_l"] * (y[k - 1] - y[k]) +\
            p["rho_l"] * x * ((n_int - k) * y[k - 1] -
                              (n_int - k - 1) * y[k]) / z
    # Community size equation
    dy[-1] = n * p['rho_n']
    return dy


def main():
    prs = argparse.ArgumentParser(description='Solves SPA* ODEs.')
    prs.add_argument('-N', type=int, required=True,
                     help='Max size.')
    prs.add_argument('-s', type=int, required=True,
                     help='Base size.')
    prs.add_argument('--rho_n', '-n', type=float, required=True,
                     help='Node birth rate.')
    prs.add_argument('--rho_l', '-l', type=float, required=True,
                     help='Link birth rate.')
    prs.add_argument('--dt', type=float, default=0.0001,
                     help='Integrator time step.')
    args = prs.parse_args()

    # Parameters
    p = dict()
    p['rho_n'] = args.rho_n     # Node birth rate
    p['rho_l'] = args.rho_l     # Link birth rate
    p['N'] = args.N             # Maximal size
    p['s'] = args.s             # Basic size

    # Initial conditions (s <= 2: diad, s > 3: K_s with a dangling path)
    if args.s <= 2:
        y0 = [0, 2, 1] + (p["N"] - 2) * [0]
    else:
        y0 = [0, 1] + [0] * (p["s"] - 3) + [p["s"] - 1, 1] +\
             [0] * (p["N"] - p["s"])
    # Append the total size of the community to the initial conditions
    y0 = y0 + [int(sum(y0))]

    # Setup integrator
    absolute_tolerance = 1e-8
    relative_tolerance = 1e-8
    max_number_of_steps = 5000

    eq_system = ode(derivatives)
    eq_system.set_integrator('dopri5',
                             nsteps=max_number_of_steps,
                             first_step=1e-8,
                             atol=absolute_tolerance, rtol=relative_tolerance)
    eq_system.set_initial_value(y0, 0)
    eq_system.set_f_params(p)

    # Integration
    # count = 0
    while eq_system.successful() and eq_system.y[-1] < p["N"] + 1:
        eq_system.integrate(eq_system.t + args.dt)

    # Post processing
    norm = sum(eq_system.y[0:-1])
    for k, n_k in enumerate(eq_system.y[0:-1]):
        print(k, n_k / norm)
    # print(norm, eq_system.y[-1], p["N"] + 1)


if __name__ == '__main__':
    main()
