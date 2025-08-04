package uk.betacraft.legacyfix;

import com.sun.jna.Native;
import com.sun.jna.Pointer;
import com.sun.jna.win32.StdCallLibrary;

public interface INativeBridge extends StdCallLibrary {
    INativeBridge m_instance = Native.load("LegacyFixNative", INativeBridge.class);

    // RawInput
    void InitRawInputPatch();
    int GetRawDeltaX();
    int GetRawDeltaY();
    boolean BIsRawInputAvailable();
}