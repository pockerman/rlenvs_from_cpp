import numpy as np
from scipy.integrate import ode


class QuadrotorState:

    def __init__(self):
        self.position = np.zeros(3)
        self.velocity = np.zeros(3)
        self.omega = np.zeros(3)
        self.quaternion = np.zeros(4)
        self.vel_dot = np.zeros(3)
        self.omega_dot = np.zeros(3)
        self.acc = np.zeros(3)

        self.wMotor = np.array([self.state[13], self.state[15], self.state[17], self.state[19]])


class Quadrotor:

    def __init__(self, state: QuadrotorState, integrator=None):
        self._state = state
        self._integrator = integrator

    @property
    def state(self) -> QuadrotorState:
        return self._state

    def update(self, cmd, wind, t: float, dt: float) -> QuadrotorState:
        prev_vel = self.state.velocity
        prev_omega = self.state.omega

        self._integrator.set_f_params(cmd, wind)

        state = self._integrator.integrate(t, t + dt)

        self.state.position = state[0:3]
        self.state.quaternion = state[3:7]
        self.state.velocity = state[7:10]
        self.state.omega = state[10:13]
        self.state.wMotor = np.array([state[13], state[15], state[17], state[19]])

        self.state.vel_dot = (self.state.velocity - prev_vel) / dt
        self.state.omega_dot = (self.state.omega - prev_omega) / dt

        self.extended_state()
        self.compute_forces()

        return self.state

    def extended_state(self) -> None:
        pass

    def compute_forces(self) -> None:
        pass
