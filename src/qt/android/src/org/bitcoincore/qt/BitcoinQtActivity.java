package org.gamcoincore.qt;

import android.os.Bundle;
import android.system.ErrnoException;
import android.system.Os;

import org.qtproject.qt5.android.bindings.QtActivity;

import java.io.File;

public class GamcoinQtActivity extends QtActivity
{
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        final File gamcoinDir = new File(getFilesDir().getAbsolutePath() + "/.gamcoin");
        if (!gamcoinDir.exists()) {
            gamcoinDir.mkdir();
        }

        super.onCreate(savedInstanceState);
    }
}
