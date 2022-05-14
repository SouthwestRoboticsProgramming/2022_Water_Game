package com.swrobotics.watergame.driverstation;

import static imgui.ImGui.*;

public class OperationWindow {
    private final TetheredConnection conn;
    private RobotState currentState;

    public OperationWindow(TetheredConnection conn) {
        this.conn = conn;
        currentState = RobotState.DISABLED;
    }

    public void showGui() {
        if (begin("Operation")) {
            beginDisabled(currentState == RobotState.TELEOP);
            if (button("Enable")) {
                currentState = RobotState.TELEOP;
                conn.updateState(currentState);
            }
            endDisabled();

            beginDisabled(currentState == RobotState.DISABLED);
            if (button("Disable")) {
                currentState = RobotState.DISABLED;
                conn.updateState(currentState);
            }
            endDisabled();
        }
        end();
    }
}
