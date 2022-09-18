package com.pratik.window;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import android.graphics.Color;

import android.content.pm.ActivityInfo;

// For Full Screen
import andoidx.core.view.WindowCompat;

public class MainActivity extends AppCompatActivity {

    private GLESView glesView;

    @Override
    protected void onCreate(Bundle savedInstanceState) { // OnResume OnPause OnStart OnRestart OnStop OnDestroy
        super.onCreate(savedInstanceState);

        // Hide Action Bar and Title
        getSupportActionBar().hide();

        // FullScreen
        WindowCompat.setDecorFitsSystemWindows(getWindow(), false);

        // Hiddning Systrem BAR and IME(INPUT METHOD TERITORY)
        WindowInsetsCotrolerCompat windowInsetsCotrolerCompat;

        windowInsetsCotrolerCompat = viewCompat.getWindowInsetsControler(getWindow.getDecorView);

        windowInsetsCotrolerCompat.hide(windowInsetsCotrolerCompat.Type.systemBar | windowInsetsCotrolerCompat.Ine);

        // Forced Landscape Orientation
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);

        getWindow().getDecorView().setBackgroundColor(Color.rgb(0, 0, 0));

        glesView = new GLESView(this);

        setContentView(glesView); // Telling Android to set parameter view as window view
    }

    @Override
    protected void onPause() {
        super.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
    }

}
