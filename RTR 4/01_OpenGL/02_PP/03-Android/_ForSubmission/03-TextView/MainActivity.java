package com.pratik.window;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

// Text View Related Packages
import androidx.appcompat.widget.AppCompatTextView;
import android.graphics.Color;
import android.view.Gravity;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) { // OnResume OnPause OnStart OnRestart OnStop OnDestroy
        super.onCreate(savedInstanceState);

        getWindow().getDecorView().setBackgroundColor(Color.BLACK);

        // setContentView(R.layout.activity_main);
        AppCompatTextView myView = new AppCompatTextView(this);
        myView.setTextSize(64);
        myView.setTextColor(Color.GREEN); // Color.rgb(1,1,1);
        myView.setGravity(Gravity.CENTER);
        myView.setText("Hello World!!!");

        setContentView(myView);

    }

}