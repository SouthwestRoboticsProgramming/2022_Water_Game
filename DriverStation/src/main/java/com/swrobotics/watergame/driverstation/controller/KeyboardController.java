package com.swrobotics.watergame.driverstation.controller;

import imgui.ImVec4;
import imgui.flag.ImGuiCol;
import imgui.flag.ImGuiTableBgTarget;
import imgui.flag.ImGuiTableColumnFlags;
import imgui.flag.ImGuiTableFlags;
import org.lwjgl.glfw.GLFWKeyCallback;

import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;
import java.util.Properties;
import java.util.UUID;

import static imgui.ImGui.*;
import static org.lwjgl.glfw.GLFW.*;

public class KeyboardController implements Controller {
    private static final Map<Integer, String> keyNames;
    private static final Map<String, Integer> keyLookup;
    private static final int NUM_BUTTONS = 32;

    private final long window;
    private final GLFWKeyCallback prevKeyCallback;

    private final UUID uuid;
    private final int[] mappings;

    private int targetButton = -1;

    public KeyboardController(long window) {
        this.window = window;
        uuid = UUID.randomUUID();

        mappings = new int[NUM_BUTTONS];
        Arrays.fill(mappings, -1);

        prevKeyCallback = glfwSetKeyCallback(window, this::keyCallback);
    }

    @Override
    public UUID getUUID() {
        return uuid;
    }

    @Override
    public String getDescription() {
        return "Keyboard";
    }

    @Override
    public boolean needsUpdateDataToRobot() {
        return true;
    }

    @Override
    public void update() {

    }

    @Override
    public int getAxisCount() {
        return 0;
    }

    @Override
    public float getAxis(int index) {
        return 0;
    }

    @Override
    public int getButtonCount() {
        return NUM_BUTTONS;
    }

    @Override
    public boolean getButton(int index) {
        int id = mappings[index];
        if (id == -1)
            return false;

        return glfwGetKey(window, id) == GLFW_PRESS;
    }

    @Override
    public void renderGuiContent() {
        if (beginTable("Keyboard keys", 8, ImGuiTableFlags.BordersOuter | ImGuiTableFlags.BordersInner)) {
            ImVec4 vec = getStyle().getColor(ImGuiCol.ButtonHovered);
            int highlight = getColorU32(vec.x, vec.y, vec.z, vec.w);

            for (int i = 0; i < 4; i++) {
                tableSetupColumn("id " + i, ImGuiTableColumnFlags.WidthFixed, 20);
                tableSetupColumn("map" + i, ImGuiTableColumnFlags.WidthStretch);
            }

            for (int i = 0; i < NUM_BUTTONS; i++) {
                tableNextColumn();
                if (getButton(i))
                    tableSetBgColor(ImGuiTableBgTarget.CellBg, highlight);
                text(String.valueOf(i));

                tableNextColumn();
                String label;
                if (mappings[i] >= 0)
                    label = keyNames.get(mappings[i]);
                else
                    label = "Unset";
                label += "###" + i;

                boolean recolor = i == targetButton;
                if (recolor) {
                    pushStyleColor(ImGuiCol.Button, highlight);
                }
                if (button(label, -Float.MIN_VALUE, 0.0f)) {
                    targetButton = i;
                }
                if (recolor)
                    popStyleColor();
            }
            endTable();
        }
    }

    private void keyCallback(long window, int key, int scancode, int action, int mods) {
        if (prevKeyCallback != null)
            prevKeyCallback.invoke(window, key, scancode, action, mods);

        if (window != this.window) return;
        if (targetButton < 0) return;
        if (action != GLFW_PRESS) return;

        // Allow exit with escape
        if (key == GLFW_KEY_ESCAPE) {
            targetButton = -1;
            return;
        }

        mappings[targetButton] = key;
        targetButton = -1;
    }

    @Override
    public void loadPreferences(Properties p) {
        for (int i = 0; i < mappings.length; i++) {
            mappings[i] = keyLookup.getOrDefault(p.getProperty("keyboard.button." + i), -1);
        }
    }

    @Override
    public void savePreferences(Properties p) {
        for (int i = 0; i < mappings.length; i++) {
            if (mappings[i] > 0) {
                p.setProperty("keyboard.button." + i, keyNames.get(mappings[i]));
            }
        }
    }

    private static void putKey(int code, String name) {
        keyNames.put(code, name);
        keyLookup.put(name, code);
    }

    static {
        keyNames = new HashMap<>();
        keyLookup = new HashMap<>();
        putKey(GLFW_KEY_SPACE, "SPACE");
        putKey(GLFW_KEY_APOSTROPHE, "APOSTROPHE");
        putKey(GLFW_KEY_COMMA, "COMMA");
        putKey(GLFW_KEY_MINUS, "MINUS");
        putKey(GLFW_KEY_PERIOD, "PERIOD");
        putKey(GLFW_KEY_SLASH, "SLASH");
        putKey(GLFW_KEY_0, "0");
        putKey(GLFW_KEY_1, "1");
        putKey(GLFW_KEY_2, "2");
        putKey(GLFW_KEY_3, "3");
        putKey(GLFW_KEY_4, "4");
        putKey(GLFW_KEY_5, "5");
        putKey(GLFW_KEY_6, "6");
        putKey(GLFW_KEY_7, "7");
        putKey(GLFW_KEY_8, "8");
        putKey(GLFW_KEY_9, "9");
        putKey(GLFW_KEY_SEMICOLON, "SEMICOLON");
        putKey(GLFW_KEY_EQUAL, "EQUAL");
        putKey(GLFW_KEY_A, "A");
        putKey(GLFW_KEY_B, "B");
        putKey(GLFW_KEY_C, "C");
        putKey(GLFW_KEY_D, "D");
        putKey(GLFW_KEY_E, "E");
        putKey(GLFW_KEY_F, "F");
        putKey(GLFW_KEY_G, "G");
        putKey(GLFW_KEY_H, "H");
        putKey(GLFW_KEY_I, "I");
        putKey(GLFW_KEY_J, "J");
        putKey(GLFW_KEY_K, "K");
        putKey(GLFW_KEY_L, "L");
        putKey(GLFW_KEY_M, "M");
        putKey(GLFW_KEY_N, "N");
        putKey(GLFW_KEY_O, "O");
        putKey(GLFW_KEY_P, "P");
        putKey(GLFW_KEY_Q, "Q");
        putKey(GLFW_KEY_R, "R");
        putKey(GLFW_KEY_S, "S");
        putKey(GLFW_KEY_T, "T");
        putKey(GLFW_KEY_U, "U");
        putKey(GLFW_KEY_V, "V");
        putKey(GLFW_KEY_W, "W");
        putKey(GLFW_KEY_X, "X");
        putKey(GLFW_KEY_Y, "Y");
        putKey(GLFW_KEY_Z, "Z");
        putKey(GLFW_KEY_LEFT_BRACKET, "LEFT_BRACKET");
        putKey(GLFW_KEY_BACKSLASH, "BACKSLASH");
        putKey(GLFW_KEY_RIGHT_BRACKET, "RIGHT_BRACKET");
        putKey(GLFW_KEY_GRAVE_ACCENT, "GRAVE_ACCENT");
        putKey(GLFW_KEY_WORLD_1, "WORLD_1");
        putKey(GLFW_KEY_WORLD_2, "WORLD_2");
        putKey(GLFW_KEY_ENTER, "ENTER");
        putKey(GLFW_KEY_TAB, "TAB");
        putKey(GLFW_KEY_BACKSPACE, "BACKSPACE");
        putKey(GLFW_KEY_INSERT, "INSERT");
        putKey(GLFW_KEY_DELETE, "DELETE");
        putKey(GLFW_KEY_RIGHT, "RIGHT");
        putKey(GLFW_KEY_LEFT, "LEFT");
        putKey(GLFW_KEY_DOWN, "DOWN");
        putKey(GLFW_KEY_UP, "UP");
        putKey(GLFW_KEY_PAGE_UP, "PAGE_UP");
        putKey(GLFW_KEY_PAGE_DOWN, "PAGE_DOWN");
        putKey(GLFW_KEY_HOME, "HOME");
        putKey(GLFW_KEY_END, "END");
        putKey(GLFW_KEY_CAPS_LOCK, "CAPS_LOCK");
        putKey(GLFW_KEY_SCROLL_LOCK, "SCROLL_LOCK");
        putKey(GLFW_KEY_NUM_LOCK, "NUM_LOCK");
        putKey(GLFW_KEY_PRINT_SCREEN, "PRINT_SCREEN");
        putKey(GLFW_KEY_PAUSE, "PAUSE");
        putKey(GLFW_KEY_F1, "F1");
        putKey(GLFW_KEY_F2, "F2");
        putKey(GLFW_KEY_F3, "F3");
        putKey(GLFW_KEY_F4, "F4");
        putKey(GLFW_KEY_F5, "F5");
        putKey(GLFW_KEY_F6, "F6");
        putKey(GLFW_KEY_F7, "F7");
        putKey(GLFW_KEY_F8, "F8");
        putKey(GLFW_KEY_F9, "F9");
        putKey(GLFW_KEY_F10, "F10");
        putKey(GLFW_KEY_F11, "F11");
        putKey(GLFW_KEY_F12, "F12");
        putKey(GLFW_KEY_F13, "F13");
        putKey(GLFW_KEY_F14, "F14");
        putKey(GLFW_KEY_F15, "F15");
        putKey(GLFW_KEY_F16, "F16");
        putKey(GLFW_KEY_F17, "F17");
        putKey(GLFW_KEY_F18, "F18");
        putKey(GLFW_KEY_F19, "F19");
        putKey(GLFW_KEY_F20, "F20");
        putKey(GLFW_KEY_F21, "F21");
        putKey(GLFW_KEY_F22, "F22");
        putKey(GLFW_KEY_F23, "F23");
        putKey(GLFW_KEY_F24, "F24");
        putKey(GLFW_KEY_F25, "F25");
        putKey(GLFW_KEY_KP_0, "KP_0");
        putKey(GLFW_KEY_KP_1, "KP_1");
        putKey(GLFW_KEY_KP_2, "KP_2");
        putKey(GLFW_KEY_KP_3, "KP_3");
        putKey(GLFW_KEY_KP_4, "KP_4");
        putKey(GLFW_KEY_KP_5, "KP_5");
        putKey(GLFW_KEY_KP_6, "KP_6");
        putKey(GLFW_KEY_KP_7, "KP_7");
        putKey(GLFW_KEY_KP_8, "KP_8");
        putKey(GLFW_KEY_KP_9, "KP_9");
        putKey(GLFW_KEY_KP_DECIMAL, "KP_DECIMAL");
        putKey(GLFW_KEY_KP_DIVIDE, "KP_DIVIDE");
        putKey(GLFW_KEY_KP_MULTIPLY, "KP_MULTIPLY");
        putKey(GLFW_KEY_KP_SUBTRACT, "KP_SUBTRACT");
        putKey(GLFW_KEY_KP_ADD, "KP_ADD");
        putKey(GLFW_KEY_KP_ENTER, "KP_ENTER");
        putKey(GLFW_KEY_KP_EQUAL, "KP_EQUAL");
        putKey(GLFW_KEY_LEFT_SHIFT, "LEFT_SHIFT");
        putKey(GLFW_KEY_LEFT_CONTROL, "LEFT_CONTROL");
        putKey(GLFW_KEY_LEFT_ALT, "LEFT_ALT");
        putKey(GLFW_KEY_LEFT_SUPER, "LEFT_SUPER");
        putKey(GLFW_KEY_RIGHT_SHIFT, "RIGHT_SHIFT");
        putKey(GLFW_KEY_RIGHT_CONTROL, "RIGHT_CONTROL");
        putKey(GLFW_KEY_RIGHT_ALT, "RIGHT_ALT");
        putKey(GLFW_KEY_RIGHT_SUPER, "RIGHT_SUPER");
        putKey(GLFW_KEY_MENU, "MENU");
    }
}
