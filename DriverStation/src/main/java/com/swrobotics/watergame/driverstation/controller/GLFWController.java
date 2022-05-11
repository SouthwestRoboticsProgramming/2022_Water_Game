package com.swrobotics.watergame.driverstation.controller;

import imgui.ImVec4;
import imgui.flag.ImGuiCol;
import imgui.flag.ImGuiTableBgTarget;
import imgui.flag.ImGuiTableFlags;

import java.nio.ByteBuffer;
import java.nio.FloatBuffer;
import java.util.UUID;

import static imgui.ImGui.*;
import static org.lwjgl.glfw.GLFW.*;

public class GLFWController implements Controller {
    private final UUID uuid;
    private final int id;
    private final boolean[] buttons = new boolean[32];
    private final float[] axes = new float[8];

    public GLFWController(int id) {
        uuid = UUID.randomUUID();
        this.id = id;
    }

    @Override
    public UUID getUUID() {
        return uuid;
    }

    @Override
    public String getDescription() {
        return glfwGetJoystickName(id);
    }

    @Override
    public boolean needsUpdateDataToRobot() {
        return true;
    }

    @Override
    public void update() {
        ByteBuffer buttons = glfwGetJoystickButtons(id);
        if (buttons == null) return;
        for (int i = 0; i < Math.min(buttons.capacity(), this.buttons.length); i++) {
            this.buttons[i] = buttons.get(i) == GLFW_PRESS;
        }

        FloatBuffer axes = glfwGetJoystickAxes(id);
        if (axes == null) return;
        for (int i = 0; i < Math.min(axes.capacity(), this.axes.length); i++) {
            this.axes[i] = axes.get(i);
        }
    }

    @Override
    public int getAxisCount() {
        return 8;
    }

    @Override
    public float getAxis(int index) {
        return axes[index];
    }

    @Override
    public int getButtonCount() {
        return 32;
    }

    @Override
    public boolean getButton(int index) {
        return buttons[index];
    }

    @Override
    public void renderGuiContent() {
        text("Buttons");
        if (beginTable("buttons", 8, ImGuiTableFlags.BordersInner | ImGuiTableFlags.BordersOuter)) {
            ImVec4 vec = getStyle().getColor(ImGuiCol.ButtonHovered);
            int highlight = getColorU32(vec.x, vec.y, vec.z, vec.w);

            for (int i = 0; i < buttons.length; i++) {
                tableNextColumn();

                if (buttons[i])
                    tableSetBgColor(ImGuiTableBgTarget.CellBg, highlight);

                text(String.valueOf(i));
            }

            endTable();
        }

        spacing();

        text("Axes");
        if (beginTable("axes", 2)) {
            for (int i = 0; i < axes.length; i++) {
                float axis = axes[i];

                String label = String.format("%d: %.3f", i, axis);

                tableNextColumn();
                progressBar(axis * 0.5f + 0.5f, -Float.MIN_VALUE, 0, label);
            }
            endTable();
        }
    }
}
