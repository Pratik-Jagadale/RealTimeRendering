package com.pratik.window;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import android.graphics.Color;

public class MainActivity extends AppCompatActivity {

    private MyView myView;

    @Override
    protected void onCreate(Bundle savedInstanceState) { // OnResume OnPause OnStart OnRestart OnStop OnDestroy
        super.onCreate(savedInstanceState);

        getWindow().getDecorView().setBackgroundColor(Color.rgb(255, 255, 255));

        myView = new MyView(this);

        setContentView(myView); // Telling Android to set parameter view as window view
    }
}
