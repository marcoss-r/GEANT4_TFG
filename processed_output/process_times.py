import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

# Datos
t_n = np.array([8.48, 10.10, 25.66, 182.70])
t_n_max = np.array([19.25, 112.84, 1057.38, 10269])
n = np.array([100, 1000, 10000, 100000])

t_e = np.array([8.48, 8.53, 9.7, 19.25])
t_e_max = np.array([182.7, 273.17, 1386.36, 10269])
e = np.array([5, 50, 500, 5000])


# Modelo con offset
def model(x, A, alpha, B):
    return A * x**alpha + B


def fit_and_plot(ax, x, y, title, xlabel, ylabel):
    # Ajuste
    popt, _ = curve_fit(model, x, y, p0=(1, 1, 1))
    A, alpha, B = popt

    y_pred = model(x, A, alpha, B)

    # R^2
    ss_res = np.sum((y - y_pred)**2)
    ss_tot = np.sum((y - np.mean(y))**2)
    r2 = 1 - ss_res / ss_tot

    # Plot
    ax.plot(x, y, 'o-', label='Datos')
    ax.plot(x, y_pred, '--', label='Ajuste propuesto')

    ax.set_xscale('log')
    ax.set_yscale('log')

    ax.set_title(title, fontsize=10)  # 👈 título más pequeño
    ax.set_xlabel(xlabel)
    ax.set_ylabel(ylabel)
    ax.grid(True, which="both")

    # 👇 Añadimos A también
    ax.text(0.05, 0.95,
            f'$R^2 = {r2:.4f}$\n'
            f'$A = {A:.4f}$\n'
            f'$\\alpha = {alpha:.4f}$\n'
            f'$B = {B:.2f}$',
            transform=ax.transAxes,
            verticalalignment='top')

    ax.legend()


# Subplots
fig, axs = plt.subplots(2, 2, figsize=(12, 10), constrained_layout=True)

fit_and_plot(axs[0, 0], n, t_n,
             'Escalado del tiempo de ejecución\nen función del número de partículas a 5 MeV',
             'Número de partículas',
             'Tiempo [s]')

fit_and_plot(axs[0, 1], n, t_n_max,
             'Escalado del tiempo de ejecución\nen función del número de partículas a 5 GeV',
             'Número de partículas',
             'Tiempo [s]')

fit_and_plot(axs[1, 0], e, t_e,
             'Escalado del tiempo de ejecución\nen función de la energía para 100 partículas',
             'Energía [MeV]',
             'Tiempo [s]')

fit_and_plot(axs[1, 1], e, t_e_max,
             'Escalado del tiempo de ejecución\nen función de la energía para 100000 partículas',
             'Energía [MeV]',
             'Tiempo [s]')

plt.show()