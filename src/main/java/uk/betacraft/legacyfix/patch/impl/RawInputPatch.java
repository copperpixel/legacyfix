package uk.betacraft.legacyfix.patch.impl;

import java.lang.instrument.ClassDefinition;
import java.lang.instrument.Instrumentation;

import javassist.CtClass;
import javassist.CtMethod;
import uk.betacraft.legacyfix.patch.Patch;

public class RawInputPatch extends Patch {
    public RawInputPatch() {
        super("rawinput", "Enables raw input on Windows", false);
    }

    @Override
    public void apply(Instrumentation inst) throws Exception {
    }

    private class RawInputBridge
    {
        static {
            System.loadLibrary("LegacyFixNative");
        }
    }
}
