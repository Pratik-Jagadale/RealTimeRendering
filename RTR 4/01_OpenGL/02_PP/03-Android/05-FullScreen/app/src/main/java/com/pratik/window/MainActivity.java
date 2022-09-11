package com.pratik.window;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import android.graphics.Color;

import android.view.View;

import android.view.Window;

public class MainActivity extends AppCompatActivity {

    private MyView myView;

    @Override
    protected void onCreate(Bundle savedInstanceState) { // OnResume OnPause OnStart OnRestart OnStop OnDestroy
        super.onCreate(savedInstanceState);

        // Doing Fullscreen
        getWindow().getDecorView().setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_FULLSCREEN |
                        View.SYSTEM_UI_FLAG_HIDE_NAVIGATION |
                        View.SYSTEM_UI_FLAG_IMMERSIVE);

        supportRequestWindowFeature(Window.FEATURE_NO_TITLE);

        getWindow().getDecorView().setBackgroundColor(Color.rgb(0, 0, 0));

        myView = new MyView(this);

        setContentView(myView); // Telling Android to set parameter view as window view
    }
}
