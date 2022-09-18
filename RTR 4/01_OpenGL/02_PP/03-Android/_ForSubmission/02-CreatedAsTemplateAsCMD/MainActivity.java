package com.pratik.window;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) { // OnResume OnSuspend OnStart OnRestart OnStop OnDestroy
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

}