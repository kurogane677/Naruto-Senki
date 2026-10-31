package net.zakume.game;

import android.app.Activity;
import android.app.AlertDialog;

import dev.axmol.lib.AxmolEngine;

/**
 * Called from native code (StartMenu::keyBackClicked) via JniHelper.
 * Replaces the original 2.x Activity helper of the same name.
 */
public class DialogUtils {
    public static void showTipDialog(final String title, final String msg) {
        final Activity activity = AxmolEngine.getActivity();
        if (activity == null) {
            return;
        }
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                new AlertDialog.Builder(activity)
                    .setTitle(title)
                    .setMessage(msg)
                    .setPositiveButton(android.R.string.ok, (dialog, which) -> {
                        activity.finishAffinity();
                        System.exit(0);
                    })
                    .setNegativeButton(android.R.string.cancel, null)
                    .show();
            }
        });
    }
}
